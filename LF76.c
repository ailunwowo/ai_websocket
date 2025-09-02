// simple_voice_dialog.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <curl/curl.h>
#include <jansson.h>
#include <libwebsockets.h>
#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif
#include "cfg.h"

#define OTA_URL "https://xrobo.qiniuapi.com/v1/ota/"
#define MAC "D4:06:06:B6:A9:FB"
#define UUID "85bcfde7-44df-41da-b450-3bf7fdf1d4dd"
#define OPUS_FILE "audio.opus"  // 要发送的opus文件名

static char g_session_id[128] = {0};
static char g_ws_token[512] = {0};
static char g_ws_url[512]   = {0};

static struct lws *g_ws_client = NULL;
static int g_connected = 0;
static int g_shaked = 0;

// Opus文件头结构
typedef struct {
    uint32_t protocol_version;
    uint32_t sample_rate;
    uint32_t channels;
    uint32_t frame_duration_ms;
    uint32_t data_size;
} opus_header_t;

/* 音频发送队列：从opus文件读取数据，排队后由WS二进制发送 */
typedef struct audio_node {
    size_t len;
    unsigned char *data;
    struct audio_node *next;
} audio_node_t;

static audio_node_t *g_audio_head = NULL;
static audio_node_t *g_audio_tail = NULL;
static pthread_mutex_t g_audio_mtx = PTHREAD_MUTEX_INITIALIZER;
static volatile int g_listen_active = 0;
static volatile int g_audio_thread_ready = 0;
static size_t g_audio_queue_bytes = 0;
static const size_t k_audio_queue_bytes_limit = 1024 * 1024;

static void audio_clear_queue(void) {
    pthread_mutex_lock(&g_audio_mtx);
    audio_node_t *node = g_audio_head;
    g_audio_head = g_audio_tail = NULL;
    g_audio_queue_bytes = 0;
    pthread_mutex_unlock(&g_audio_mtx);
    while (node) {
        audio_node_t *next = node->next;
        free(node->data);
        free(node);
        node = next;
    }
}

static void audio_enqueue(const unsigned char *data, size_t len) {
    if (!data || len == 0) return;
    audio_node_t *node = (audio_node_t*)malloc(sizeof(audio_node_t));
    if (!node) return;
    node->data = (unsigned char*)malloc(len);
    if (!node->data) { free(node); return; }
    memcpy(node->data, data, len);
    node->len = len;
    node->next = NULL;
    
    pthread_mutex_lock(&g_audio_mtx);
    if (g_audio_queue_bytes + len > k_audio_queue_bytes_limit) {
        pthread_mutex_unlock(&g_audio_mtx);
        free(node->data);
        free(node);
        return;
    }
    
    if (g_audio_tail) {
        g_audio_tail->next = node;
        g_audio_tail = node;
    } else {
        g_audio_head = g_audio_tail = node;
    }
    g_audio_queue_bytes += len;
    pthread_mutex_unlock(&g_audio_mtx);
}

static size_t audio_dequeue(unsigned char *out, size_t maxlen) {
    pthread_mutex_lock(&g_audio_mtx);
    audio_node_t *node = g_audio_head;
    if (!node) { pthread_mutex_unlock(&g_audio_mtx); return 0; }
    g_audio_head = node->next;
    if (!g_audio_head) g_audio_tail = NULL;
    pthread_mutex_unlock(&g_audio_mtx);
    
    size_t n = 0;
    if (out && maxlen) {
        n = node->len <= maxlen ? node->len : maxlen;
        memcpy(out, node->data, n);
    }
    
    pthread_mutex_lock(&g_audio_mtx);
    if (g_audio_queue_bytes >= node->len) 
        g_audio_queue_bytes -= node->len; 
    else 
        g_audio_queue_bytes = 0;
    pthread_mutex_unlock(&g_audio_mtx);
    
    free(node->data);
    free(node);
    return n;
}

// 修改opus文件读取线程，确保在发送音频前发送start命令
static void *opus_file_reader_thread(void *arg) {
    // 等待WebSocket连接就绪
    printf("等待WebSocket连接就绪...\n");
    while (!g_connected || !g_shaked) {
        usleep(100000);
    }
    
    FILE *file = fopen(OPUS_FILE, "rb");
    if (!file) {
        perror("无法打开opus文件");
        return NULL;
    }
    
    // 读取文件头
    opus_header_t header;
    if (fread(&header, sizeof(header), 1, file) != 1) {
        fprintf(stderr, "读取opus文件头失败\n");
        fclose(file);
        return NULL;
    }
    
    printf("Opus文件信息: 采样率=%uHz, 通道数=%u, 帧长=%ums, 数据大小=%u字节\n",
           header.sample_rate, header.channels, header.frame_duration_ms, header.data_size);
    
    // 先发送start命令
    if (g_connected && g_shaked && g_ws_client && g_session_id[0]) {
        unsigned char start_buf[LWS_PRE + 256];
        int n = snprintf((char*)start_buf + LWS_PRE, 256,
            "{\"session_id\":\"%s\",\"type\":\"listen\",\"state\":\"start\",\"mode\":\"manual\"}", g_session_id);
        lws_write(g_ws_client, start_buf + LWS_PRE, (size_t)n, LWS_WRITE_TEXT);
        g_listen_active = 1;
        printf("已发送start命令，开始发送opus文件数据\n");
        
        // 等待一段时间确保start命令生效
        sleep(1);
    }
    
    g_audio_thread_ready = 1;
    
    printf("开始读取opus文件数据...\n");
    
    uint32_t total_read = 0;
    int frame_count = 0;
    
    while (total_read < header.data_size && g_listen_active) {
        // 读取帧长度
        int32_t opus_len;
        if (fread(&opus_len, sizeof(opus_len), 1, file) != 1) {
            fprintf(stderr, "读取帧长度失败\n");
            break;
        }
        total_read += sizeof(opus_len);
        
        // 读取Opus数据
        unsigned char *opus_data = malloc(opus_len);
        if (!opus_data) {
            fprintf(stderr, "内存分配失败\n");
            break;
        }
        
        if (fread(opus_data, 1, opus_len, file) != opus_len) {
            fprintf(stderr, "读取Opus数据失败\n");
            free(opus_data);
            break;
        }
        total_read += opus_len;
        
        // 将数据加入队列
        audio_enqueue(opus_data, opus_len);
        free(opus_data);
        
        frame_count++;
        printf("已读取第%d帧, 长度: %d字节, 总进度: %u/%u字节\n", 
               frame_count, opus_len, total_read, header.data_size);
        
        // 通知WebSocket线程有数据可写
        if (g_ws_client) {
            lws_callback_on_writable(g_ws_client);
        }
        
        // 模拟实时发送的间隔（60ms）
        usleep(60000);
    }
    
    fclose(file);
    printf("Opus文件读取完成，共%d帧\n", frame_count);
    
    // 等待一段时间让服务器处理完所有数据
    sleep(2);
    
    // 文件读取完成后发送stop命令
    if (g_connected && g_shaked && g_ws_client && g_session_id[0]) {
        unsigned char stop_buf[LWS_PRE + 256];
        int n = snprintf((char*)stop_buf + LWS_PRE, 256,
            "{\"session_id\":\"%s\",\"type\":\"listen\",\"state\":\"stop\"}", g_session_id);
        lws_write(g_ws_client, stop_buf + LWS_PRE, (size_t)n, LWS_WRITE_TEXT);
        g_listen_active = 0;
        printf("已发送stop命令（文件播放完成）\n");
    }
    
    return NULL;
}

/* 下行音频（从 WS 收到的二进制）-> 通过 UDP 发给 sound_app 的 AUDIO_PORT_DOWN */
static int g_udp_send_fd = -1;
static struct sockaddr_in g_udp_send_addr;

static int init_udp_sender(void) {
    g_udp_send_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (g_udp_send_fd < 0) {
        perror("udp sender socket");
        return -1;
    }
    memset(&g_udp_send_addr, 0, sizeof(g_udp_send_addr));
    g_udp_send_addr.sin_family = AF_INET;
    g_udp_send_addr.sin_port = htons(AUDIO_PORT_DOWN);
    inet_pton(AF_INET, "127.0.0.1", &g_udp_send_addr.sin_addr);
    return 0;
}

static void audio_udp_senddownlink(const void *data, size_t len) {
    if (g_udp_send_fd < 0 || !data || len == 0) return;
    sendto(g_udp_send_fd, (const char*)data, (int)len, 0,
           (struct sockaddr*)&g_udp_send_addr, sizeof(g_udp_send_addr));
}

struct memory_struct {
    char *memory;
    size_t size;
};

struct headers_data {
    char auth_header[512];
    char device_header[128];
    char client_header[128];
    char version_header[8];
};

/* ---------- utils ---------- */

static size_t write_memory_callback(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t realsize = size * nmemb;
    struct memory_struct *mem = (struct memory_struct *)userp;
    char *ptr = realloc(mem->memory, mem->size + realsize + 1);
    if (!ptr) {
        fprintf(stderr, "Not enough memory\n");
        return 0;
    }
    mem->memory = ptr;
    memcpy(mem->memory + mem->size, contents, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;
    return realsize;
}

/* 解析形如： ws://host[:port]/path   或  wss://host[:port]/path */
static int parse_ws_url(const char *url,
                        int *is_secure, char *host, size_t host_sz,
                        int *port, char *path, size_t path_sz) {
  if (!url || !is_secure || !host || !port || !path) return -1;

  const char *p = NULL;
  if (!strncmp(url, "wss://", 6)) {
    *is_secure = 1;
    p = url + 6;
  } else if (!strncmp(url, "ws://", 5)) {
    *is_secure = 0;
    p = url + 5;
  } else {
    fprintf(stderr, "Unsupported scheme in url: %s\n", url);
    return -1;
  }

  const char *slash = strchr(p, '/');
  if (!slash) {
    // 无 path 时，给默认 "/"
    slash = p + strlen(p);
    strncpy(path, "/", path_sz);
  } else {
    snprintf(path, path_sz, "%s", slash);
  }

  // host[:port]
  char hostport[256] = {0};
  size_t hp_len = (size_t)(slash - p);
  if (hp_len >= sizeof(hostport)) hp_len = sizeof(hostport) - 1;
  memcpy(hostport, p, hp_len);
  hostport[hp_len] = '\0';

  char *colon = strchr(hostport, ':');
  if (colon) {
    *colon = '\0';
    snprintf(host, host_sz, "%s", hostport);
    *port = atoi(colon + 1);
    if (*port <= 0) *port = *is_secure ? 443 : 80;
  } else {
    snprintf(host, host_sz, "%s", hostport);
    *port = *is_secure ? 443 : 80;
  }

  return 0;
}

/* ---------- OTA 激活 / 获取 ws_url + token ---------- */

static int activate_and_fetch_ws() {
  CURL *curl;
  CURLcode res;
  struct memory_struct chunk = {.memory = malloc(1), .size = 0};

  if (!chunk.memory) return -1;

  char post_data[2048];
  snprintf(post_data, sizeof(post_data),
           "{\"application\":{\"name\":\"xiaoniu-web-test\",\"version\":\"1.0.0\","
           "\"compile_time\":\"2025-04-16 10:00:00\",\"idf_version\":\"4.4.3\","
           "\"elf_sha256\":\"1234567890abcdef1234567890abcdef1234567890abcdef\"},"
           "\"ota\":{\"label\":\"xiaoling-web-test\"},"
           "\"board\":{\"type\":\"xiaoling-web-test\",\"name\":\"xiaoniu-web-test\","
           "\"ssid\":\"xiaoling-web-test\",\"rssi\":-50,\"channel\":1,"
           "\"ip\":\"192.168.1.1\",\"mac\":\"%s\"},"
           "\"mac_address\":\"%s\",\"uuid\":\"%s\",\"version\":0,"
           "\"chip_model_name\":\"esp32s3\",\"flash_size\":8388608,"
           "\"minimum_free_heap_size\":32768,"
           "\"chip_info\":{\"model\":0,\"cores\":2,\"revision\":0,\"features\":0},"
           "\"partition_table\":[{\"label\":\"ota\",\"type\":0,\"subtype\":16,"
           "\"address\":65536,\"size\":4194304}]}",
           MAC, MAC, UUID);

  curl = curl_easy_init();
  if (!curl) {
    free(chunk.memory);
    return -1;
  }

  struct curl_slist *headers = NULL;
  headers = curl_slist_append(headers, "Content-Type: application/json");
  headers = curl_slist_append(headers, "Device-Id: " MAC);
  headers = curl_slist_append(headers, "Client-Id: " UUID);
  headers = curl_slist_append(headers, "User-Agent: esp-box-3/1.5.6");
  headers = curl_slist_append(headers, "Accept-Language: zh-CN");
  headers = curl_slist_append(headers, "Activation-Version: 2");

  curl_easy_setopt(curl, CURLOPT_URL, OTA_URL);
  curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post_data);
  curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_memory_callback);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);
  // NOTE: 若生产环境请开启证书验证
  curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
  curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
  curl_easy_setopt(curl, CURLOPT_TIMEOUT, 30L);

  res = curl_easy_perform(curl);
  if (res != CURLE_OK) {
    fprintf(stderr, "CURL error: %s\n", curl_easy_strerror(res));
    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);
    free(chunk.memory);
    return -1;
  }

  int ret = -1;
  json_error_t jerr;
  json_t *root = json_loads(chunk.memory, 0, &jerr);
  if (!root) {
    fprintf(stderr, "JSON parse error at line %d: %s\n", jerr.line, jerr.text);
    goto out;
  }

  // 1) 若有 activation.code 说明未绑定
  json_t *activation = json_object_get(root, "activation");
  if (activation) {
    const char *code = json_string_value(json_object_get(activation, "code"));
    if (code && *code) {
      printf("激活码: %s, 请到 https://xrobo.qiniu.com 绑定设备后再试。\n", code);
      // 仍然可能返回 websocket 信息，但一般需要绑定后才可用；
      // 这里按“未激活”处理，返回 -1 以便上层循环重试。
      ret = -1;
      goto parse_ws; // 继续解析，打印一下有无 ws_url，便于排查
    }
  }

parse_ws:
  // 2) 解析 websocket.url 和 token
  {
    json_t *ws = json_object_get(root, "websocket");
    if (ws) {
      const char *url = json_string_value(json_object_get(ws, "url"));
      const char *token = json_string_value(json_object_get(ws, "token"));
      if (url && *url) {
        snprintf(g_ws_url, sizeof(g_ws_url), "%s", url);
      }
      if (token && *token) {
        snprintf(g_ws_token, sizeof(g_ws_token), "%s", token);
      }
      if (g_ws_url[0]) {
        printf("获取到 WebSocket URL: %s\n", g_ws_url);
      }
      if (g_ws_token[0]) {
        printf("获取到 Token: %s\n", g_ws_token);
      }
    }
  }

  // 若没有激活码且拿到了 token，视为可连
  if ((!activation || !json_object_get(activation, "code")) && g_ws_token[0] && g_ws_url[0]) {
    printf("激活成功，准备建立 WebSocket。\n");
    ret = 0;
  }

  json_decref(root);

out:
  curl_slist_free_all(headers);
  curl_easy_cleanup(curl);
  free(chunk.memory);
  return ret;
}

/* ---------- libwebsockets 回调 ---------- */

static int ws_callback(struct lws *wsi, enum lws_callback_reasons reason, void *user, void *in, size_t len) {
  switch (reason) {
    case LWS_CALLBACK_CLIENT_ESTABLISHED: {
      g_connected = 1;
      printf("WebSocket连接已建立\n");
      unsigned char buf[LWS_PRE + 512];
      int n = snprintf((char*)buf + LWS_PRE, 512,
        "{\"type\":\"hello\",\"version\":1,\"transport\":\"websocket\","
        "\"audio_params\":{\"format\":\"opus\",\"sample_rate\":16000,"
        "\"channels\":1,\"frame_duration\":60}}");
      lws_write(wsi, buf + LWS_PRE, (size_t)n, LWS_WRITE_TEXT);
      printf("已发送 hello\n");
      lws_callback_on_writable(wsi);
      break;
    }

     case LWS_CALLBACK_CLIENT_RECEIVE: {
            if (lws_frame_is_binary(wsi)) {
                printf("收到二进制音频数据，长度: %zu字节\n", len);
                if (len > 0) audio_udp_senddownlink(in, len);
                break;
            }
            
            printf("收到文本消息: %.*s\n", (int)len, (const char*)in);
            json_error_t jerr;
            json_t *root = json_loadb((const char*)in, len, 0, &jerr);
            if (root) {
                const char *type = json_string_value(json_object_get(root, "type"));
                if (type) {
                    printf("消息类型: %s\n", type);
                    
                    if (!strcmp(type, "hello")) {
                        const char *sid = json_string_value(json_object_get(root, "session_id"));
                        if (sid) {
                            snprintf(g_session_id, sizeof(g_session_id), "%s", sid);
                            g_shaked = 1;
                            printf("会话建立，session_id = %s\n", g_session_id);
                        }
                    } 
                    else if (!strcmp(type, "asr")) {
                        const char *text = json_string_value(json_object_get(root, "text"));
                        if (text) printf("语音识别结果: %s\n", text);
                    } 
                    else if (!strcmp(type, "stt")) {
                        const char *text = json_string_value(json_object_get(root, "text"));
                        if (text) printf("语音识别: %s\n", text);
                    } 
                    else if (!strcmp(type, "llm")) {
                        const char *text = json_string_value(json_object_get(root, "text"));
                        const char *emotion = json_string_value(json_object_get(root, "emotion"));
                        printf("大模型回复: %s (情绪: %s)\n", text ? text : "", emotion ? emotion : "");
                    } 
                    else if (!strcmp(type, "tts")) {
                        const char *state = json_string_value(json_object_get(root, "state"));
                        const char *text = json_string_value(json_object_get(root, "text"));
                        printf("语音合成 %s: %s\n", state ? state : "", text ? text : "");
                    }
                    else {
                        printf("未知消息类型，完整消息: %.*s\n", (int)len, (const char*)in);
                    }
                }
                json_decref(root);
            } else {
                printf("JSON解析失败: %s\n", jerr.text);
            }
            break;
        }

    case LWS_CALLBACK_CLIENT_WRITEABLE: {
      unsigned char wbuf[LWS_PRE + 4096];
      size_t nbin = audio_dequeue(wbuf + LWS_PRE, sizeof(wbuf) - LWS_PRE);
      if (nbin > 0) {
        lws_write(wsi, wbuf + LWS_PRE, nbin, LWS_WRITE_BINARY);
        pthread_mutex_lock(&g_audio_mtx);
        int more = (g_audio_head != NULL);
        pthread_mutex_unlock(&g_audio_mtx);
        if (more) lws_callback_on_writable(wsi);
      }
      break;
    }

    case LWS_CALLBACK_CLIENT_APPEND_HANDSHAKE_HEADER: {
      struct headers_data *hd = (struct headers_data *)user;
      unsigned char **p = (unsigned char **)in;
      unsigned char *end = (*p) + len;

      if (lws_add_http_header_by_name(wsi, (unsigned char *)"Authorization: ",
            (unsigned char *)hd->auth_header, strlen(hd->auth_header), p, end))
        return -1;

      if (lws_add_http_header_by_name(wsi, (unsigned char *)"Protocol-Version: ",
            (unsigned char *)hd->version_header, strlen(hd->version_header), p, end))
        return -1;

      if (lws_add_http_header_by_name(wsi, (unsigned char *)"Device-Id: ",
            (unsigned char *)hd->device_header, strlen(hd->device_header), p, end))
        return -1;

      if (lws_add_http_header_by_name(wsi, (unsigned char *)"Client-Id: ",
            (unsigned char *)hd->client_header, strlen(hd->client_header), p, end))
        return -1;
      break;
    }

    case LWS_CALLBACK_CLOSED:
    case LWS_CALLBACK_CLIENT_CONNECTION_ERROR:
      if (user) free(user);
      g_connected = 0;
      g_shaked = 0;
      fprintf(stderr, "WebSocket连接关闭/错误\n");
      break;

    default:
      break;
  }
  return 0;
}

static struct lws_protocols protocols[] = {
    { "http", lws_callback_http_dummy, 0, 0 },
    { "voice-client", ws_callback, sizeof(struct headers_data), 0 },
    { NULL, NULL, 0, 0 }
};

struct ws_thread_args {
    int is_secure;
    char host[256];
    int port;
    char path[256];
};

static void *websocket_thread(void *arg) {
  struct ws_thread_args *cfg = (struct ws_thread_args *)arg;

  struct lws_context_creation_info info;
  memset(&info, 0, sizeof(info));
  info.port = CONTEXT_PORT_NO_LISTEN;
  info.protocols = protocols;
  info.options = LWS_SERVER_OPTION_DO_SSL_GLOBAL_INIT;

  struct lws_context *context = lws_create_context(&info);
  if (!context) {
    fprintf(stderr, "创建lws上下文失败\n");
    free(cfg);
    return NULL;
  }

  struct lws_client_connect_info i;
  memset(&i, 0, sizeof(i));
  i.context = context;
  i.address = cfg->host;
  i.port = cfg->port;
  i.path = cfg->path;
  i.host = cfg->host;
  i.origin = cfg->is_secure ? "https://" : "http://";
  i.ssl_connection = cfg->is_secure ? LCCSCF_USE_SSL : 0;
  i.protocol = "voice-client";
  i.pwsi = &g_ws_client;

  // 准备握手时要带的头部数据
  struct headers_data *hd = (struct headers_data *)malloc(sizeof(struct headers_data));
  if (!hd) {
    fprintf(stderr, "内存分配失败\n");
    lws_context_destroy(context);
    free(cfg);
    return NULL;
  }
  snprintf(hd->auth_header, sizeof(hd->auth_header), "Bearer %s", g_ws_token);
  snprintf(hd->device_header, sizeof(hd->device_header), "%s", MAC);
  snprintf(hd->client_header, sizeof(hd->client_header), "%s", UUID);
  snprintf(hd->version_header, sizeof(hd->version_header), "%s", "1");
  i.userdata = hd;

  g_ws_client = lws_client_connect_via_info(&i);
  if (!g_ws_client) {
    fprintf(stderr, "lws客户端连接失败（host=%s port=%d path=%s secure=%d）\n",
            cfg->host, cfg->port, cfg->path, cfg->is_secure);
    free(hd);
    lws_context_destroy(context);
    free(cfg);
    return NULL;
  }

  // 事件循环
  while (1) {
    lws_service(context, 0);
    if (g_listen_active && g_ws_client) {
      pthread_mutex_lock(&g_audio_mtx);
      int has_data = (g_audio_head != NULL);
      pthread_mutex_unlock(&g_audio_mtx);
      if (has_data) lws_callback_on_writable(g_ws_client);
    }
    usleep(10000);
  }

  // not reached
  // lws_context_destroy(context);
  // free(cfg);
  // return NULL;
}

/* ---------- main ---------- */
/* ---------- main ---------- */
int main(void) {
    // 检查opus文件是否存在
    if (access(OPUS_FILE, R_OK) != 0) {
        fprintf(stderr, "错误: Opus文件 %s 不存在或无法读取\n", OPUS_FILE);
        fprintf(stderr, "请先运行录音程序生成音频文件\n");
        return 1;
    }

    lws_set_log_level(LLL_ERR | LLL_WARN | LLL_NOTICE | LLL_CLIENT | LLL_HEADER, NULL);

    printf("开始设备激活流程...\n");
    while (activate_and_fetch_ws() != 0) {
        printf("激活失败或未绑定，5秒后重试...\n");
        sleep(5);
    }

    int is_secure = 0, port = 0;
    char host[256] = {0}, path[256] = {0};
    if (parse_ws_url(g_ws_url, &is_secure, host, sizeof(host), &port, path, sizeof(path)) != 0) {
        fprintf(stderr, "解析 WebSocket URL 失败: %s\n", g_ws_url);
        return 1;
    }
    printf("即将连接：%s://%s:%d%s\n", is_secure ? "wss" : "ws", host, port, path);

    struct ws_thread_args *cfg = (struct ws_thread_args *)malloc(sizeof(struct ws_thread_args));
    if (!cfg) {
        fprintf(stderr, "内存分配失败\n");
        return 1;
    }
    cfg->is_secure = is_secure;
    snprintf(cfg->host, sizeof(cfg->host), "%s", host);
    cfg->port = port;
    snprintf(cfg->path, sizeof(cfg->path), "%s", path);

    pthread_t tid;
    if (pthread_create(&tid, NULL, websocket_thread, cfg) != 0) {
        perror("创建WebSocket线程失败");
        free(cfg);
        return 1;
    }

    // 启动opus文件读取线程（自动开始）
    pthread_t opus_tid;
    if (pthread_create(&opus_tid, NULL, opus_file_reader_thread, NULL) != 0) {
        perror("创建opus文件读取线程失败");
        return 1;
    }
    
    if (init_udp_sender() != 0) {
        fprintf(stderr, "UDP下行发送通道初始化失败\n");
    }
    
    // 主循环只用于等待和显示消息
    printf("等待处理完成...\n");
    while (1) {
        // 处理WebSocket消息
        if (g_ws_client) {
            lws_callback_on_writable(g_ws_client);
        }
        sleep(1);
        
        // 如果文件发送完成且一段时间没有活动，退出
        static int idle_count = 0;
        if (!g_listen_active) {
            idle_count++;
            if (idle_count > 50) { // 10秒无活动
                printf("长时间无活动，退出程序\n");
                break;
            }
        }
    }
    
    return 0;
}