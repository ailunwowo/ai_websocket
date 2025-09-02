// simple_voice_dialog.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <curl/curl.h>
#include <jansson.h>
#include <nopoll/nopoll.h>
#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif
#include "cfg.h"
#include "opus_data.h"

#define OTA_URL "https://xrobo.qiniuapi.com/v1/ota/"
#define MAC "00:0c:29:34:45:0f"
#define UUID "webai_test"
#define AUDIO_PORT_DOWN 12345  // 假设的下行音频端口

char session_id[64] = "";

static char g_session_id[128] = {0};
static char g_ws_token[512] = {0};
static char g_ws_url[512]   = {0};

static noPollConn *g_nopoll_conn = NULL;
static noPollCtx *g_nopoll_ctx = NULL;
static int g_connected = 0;
static int g_shaked = 0;

/* 音频发送队列：从内存数组读取数据，排队后由WS二进制发送 */
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

// 修改 opus_memory_reader_thread 函数
static void *opus_memory_reader_thread(void *arg) {
    // 等待WebSocket连接就绪
    printf("等待WebSocket连接就绪...\n");
    while (!g_connected || !g_shaked) {
        usleep(100000);
    }
    
    printf("Opus音频数据大小: %u字节\n", opus_audio_data_size);
    
    // 跳过Opus文件头（通常是前19个字节）
    size_t offset = 19;
    int frame_count = 0;
    
    g_audio_thread_ready = 1;
    
    // 先发送start命令
    if (g_connected && g_shaked && g_nopoll_conn && g_session_id[0]) {
        char start_buf[256];
        int n = snprintf(start_buf, sizeof(start_buf),
            "{\"session_id\":\"%s\",\"type\":\"listen\",\"state\":\"start\",\"mode\":\"manual\"}", g_session_id);
        nopoll_conn_send_text(g_nopoll_conn, start_buf, n);
        g_listen_active = 1;
        printf("已发送start命令，开始发送opus音频数据\n");
        
        // 等待一段时间确保start命令生效
        sleep(1);
    }
    
    printf("开始解析并发送opus音频数据...\n");
    
    // 直接发送整个音频数据（跳过文件头）
    if (offset < opus_audio_data_size) {
        size_t data_size = opus_audio_data_size - offset;
        audio_enqueue(&opus_audio_data[offset], data_size);
        printf("已跳过文件头，发送音频数据: %zu字节\n", data_size);
        
        // 通知主线程有数据可发送
        if (g_nopoll_conn) {
            // 这里需要一种方式通知主线程有数据可写
        }
    }
    
    // 等待一段时间让服务器处理数据
    sleep(2);
    
    // 数据发送完成后发送stop命令
    if (g_connected && g_shaked && g_nopoll_conn && g_session_id[0]) {
        char stop_buf[256];
        int n = snprintf(stop_buf, sizeof(stop_buf),
            "{\"session_id\":\"%s\",\"type\":\"listen\",\"state\":\"stop\"}", g_session_id);
        nopoll_conn_send_text(g_nopoll_conn, stop_buf, n);
        g_listen_active = 0;
        printf("已发送stop命令（音频数据发送完成）\n");
    }
    
    return NULL;
}

/* 下行音频处理 */
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

static int parse_ws_url(const char *url, int *is_secure, char *host, size_t host_sz,
                        char *port, size_t port_sz, char *path, size_t path_sz) {
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
        strncpy(path, "/", path_sz);
    } else {
        snprintf(path, path_sz, "%s", slash);
    }

    char hostport[256] = {0};
    size_t hp_len = (size_t)(slash - p);
    if (hp_len >= sizeof(hostport)) hp_len = sizeof(hostport) - 1;
    memcpy(hostport, p, hp_len);
    hostport[hp_len] = '\0';

    char *colon = strchr(hostport, ':');
    if (colon) {
        *colon = '\0';
        snprintf(host, host_sz, "%s", hostport);
        snprintf(port, port_sz, "%s", colon + 1);
        if (atoi(port) <= 0) {
            snprintf(port, port_sz, "%d", *is_secure ? 443 : 80);
        }
    } else {
        snprintf(host, host_sz, "%s", hostport);
        snprintf(port, port_sz, "%d", *is_secure ? 443 : 80);
    }

    return 0;
}

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

    json_t *activation = json_object_get(root, "activation");
    if (activation) {
        const char *code = json_string_value(json_object_get(activation, "code"));
        if (code && *code) {
            printf("激活码: %s, 请到 https://xrobo.qiniu.com 绑定设备后再试。\n", code);
            ret = -1;
            goto parse_ws;
        }
    }

parse_ws:
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
 
/// 消息回调函数
static void ws_on_message(noPollCtx *ctx, noPollConn *conn, noPollMsg *msg, void *user_data) {
    if (nopoll_msg_is_final(msg)) {
        const char *content = (const char *)nopoll_msg_get_payload(msg);
        size_t len = nopoll_msg_get_payload_size(msg);
        printf("收到文本消息: %.*s\n", (int)len, content);
        
        json_error_t jerr;
        json_t *root = json_loadb(content, len, 0, &jerr);
        if (root) {
            const char *type = json_string_value(json_object_get(root, "type"));
            if (type) {
                printf("消息类型: %s\n", type);
                
                if (!strcmp(type, "hello")) {
                    const char *sid = json_string_value(json_object_get(root, "session_id"));
                    if (sid && strlen(sid) > 0 && strlen(g_session_id) == 0) {
                        snprintf(g_session_id, sizeof(g_session_id), "%s", sid);
                        g_shaked = 1;
                        printf("会话建立，session_id = %s\n", g_session_id);
                    }
                } 
                else if (!strcmp(type, "asr") || !strcmp(type, "stt")) {
                    const char *text = json_string_value(json_object_get(root, "text"));
                    if (text) printf("语音识别结果: %s\n", text);
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
                    printf("未知消息类型: %s\n", type);
                }
            }
            json_decref(root);
        } else {
            printf("JSON解析失败: %s\n", jerr.text);
        }
    } else {
        // 处理二进制消息（下行音频）
        const void *data = nopoll_msg_get_payload(msg);
        size_t len = nopoll_msg_get_payload_size(msg);
        printf("收到二进制音频数据，长度: %zu字节\n", len);
        if (len > 0) audio_udp_senddownlink(data, len);
    }
}

struct ws_thread_args {
    int is_secure;
    char host[256];
    char port[16];
    char path[256];
};

// 修改 websocket_thread 函数，移除主动轮询部分
static void *websocket_thread(void *arg) {
    struct ws_thread_args *cfg = (struct ws_thread_args *)arg;
    g_nopoll_ctx = nopoll_ctx_new();
    if (!g_nopoll_ctx) {
        printf("无法创建 nopoll 上下文\n");
        free(cfg);
        return NULL;
    }

    noPollConnOpts *opts = nopoll_conn_opts_new();
    if (!opts) {
        printf("无法创建连接选项\n");
        nopoll_ctx_unref(g_nopoll_ctx);
        free(cfg);
        return NULL;
    }

    // 设置额外请求头
    char extra_headers[1024];
    snprintf(extra_headers, sizeof(extra_headers),
             "Authorization: Bearer %s\r\n"
             "Device-Id: %s\r\n"
             "Client-Id: %s\r\n"
             "Protocol-Version: 1\r\n",
             g_ws_token, MAC, UUID);
    nopoll_conn_opts_set_extra_headers(opts, extra_headers);

    // 连接到 WebSocket
    const char *host = "xrobo-io.qiniuapi.com";
    const char *port = "443";
    const char *path = "/v1/ws/";
    g_nopoll_conn = nopoll_conn_tls_new(g_nopoll_ctx, opts, host, port, NULL, path, NULL, NULL);
    if (!g_nopoll_conn) {
        printf("WebSocket connection created unsuccessfully\n");
        nopoll_conn_opts_free(opts);
        nopoll_ctx_unref(g_nopoll_ctx);
        free(cfg);
        return NULL;
    }

    // 等待连接就绪
    if (!nopoll_conn_wait_until_connection_ready(g_nopoll_conn, 10)) {
        printf("WebSocket connection not ready within timeout\n");
        nopoll_conn_close(g_nopoll_conn);
        nopoll_conn_opts_free(opts);
        nopoll_ctx_unref(g_nopoll_ctx);
        free(cfg);
        return NULL;
    }

    printf("WebSocket connection created successfully\n");

    // 检查连接状态
    if (!nopoll_conn_is_ready(g_nopoll_conn)) {
        printf("WebSocket connection is not ready\n");
        nopoll_conn_close(g_nopoll_conn);
        nopoll_conn_opts_free(opts);
        nopoll_ctx_unref(g_nopoll_ctx);
        free(cfg);
        return NULL;
    }
    
    printf("WebSocket连接已建立\n");
    g_connected = 1;

    // 设置消息回调 - 这是关键，所有消息将通过回调处理
    nopoll_conn_set_on_msg(g_nopoll_conn, ws_on_message, NULL);

    // 发送hello消息
    const char *hello_msg = "{\"type\":\"hello\",\"version\":1,\"transport\":\"websocket\",\"audio_params\":{\"format\":\"opus\",\"sample_rate\":16000,\"channels\":1,\"frame_duration\":60}}";
    nopoll_conn_send_text(g_nopoll_conn, hello_msg, strlen(hello_msg));
    printf("已发送 hello: %s\n", hello_msg);

    // 主循环处理消息和发送音频数据
    while (1) {
       nopoll_loop_wait(g_nopoll_ctx,1000);

        // 发送队列中的音频数据
        if (g_listen_active && g_nopoll_conn) {
            pthread_mutex_lock(&g_audio_mtx);
            int has_data = (g_audio_head != NULL);
            pthread_mutex_unlock(&g_audio_mtx);
            
            if (has_data) {
                unsigned char wbuf[4096];
                size_t nbin = audio_dequeue(wbuf, sizeof(wbuf));
                if (nbin > 0) {
                    nopoll_conn_send_binary(g_nopoll_conn, wbuf, nbin);
                    printf("发送音频帧，长度: %zu字节\n", nbin);
                }
            }
        }

        usleep(10000); // 10ms延迟，减少CPU占用
    }

    // 清理资源（实际不会执行到这里）
    nopoll_conn_close(g_nopoll_conn);
    nopoll_conn_opts_free(opts);
    nopoll_ctx_unref(g_nopoll_ctx);
    free(cfg);
    return NULL;
}

int main(void) {
    printf("开始设备激活流程...\n");
    while (activate_and_fetch_ws() != 0) {
        printf("激活失败或未绑定，5秒后重试...\n");
        sleep(5);
    }

    int is_secure = 0;
    char host[256] = {0}, port[16] = {0}, path[256] = {0};
    if (parse_ws_url(g_ws_url, &is_secure, host, sizeof(host), port, sizeof(port), path, sizeof(path)) != 0) {
        fprintf(stderr, "解析 WebSocket URL 失败: %s\n", g_ws_url);
        return 1;
    }
    printf("即将连接：%s://%s:%s%s\n", is_secure ? "wss" : "ws", host, port, path);

    struct ws_thread_args *cfg = (struct ws_thread_args *)malloc(sizeof(struct ws_thread_args));
    if (!cfg) {
        fprintf(stderr, "内存分配失败\n");
        return 1;
    }
    cfg->is_secure = is_secure;
    snprintf(cfg->host, sizeof(cfg->host), "%s", host);
    snprintf(cfg->port, sizeof(cfg->port), "%s", port);
    snprintf(cfg->path, sizeof(cfg->path), "%s", path);

    pthread_t tid;
    if (pthread_create(&tid, NULL, websocket_thread, cfg) != 0) {
        perror("创建WebSocket线程失败");
        free(cfg);
        return 1;
    }

    // 启动opus内存数据读取线程
    pthread_t opus_tid;
    if (pthread_create(&opus_tid, NULL, opus_memory_reader_thread, NULL) != 0) {
        perror("创建opus数据读取线程失败");
        return 1;
    }

    if (init_udp_sender() != 0) {
        fprintf(stderr, "UDP下行发送通道初始化失败\n");
    }

    // 主循环等待处理完成
    printf("等待处理完成...\n");
    while (1) {
        sleep(1);
        
        // 如果数据发送完成且一段时间没有活动，退出
        static int idle_count = 0;
        if (!g_listen_active) {
            idle_count++;
            if (idle_count > 40) {
                printf("长时间无活动，退出程序\n");
                break;
            }
        } else {
            idle_count = 0; // 重置计数器
        }
    }
    
    // 清理工作
    audio_clear_queue();
    if (g_nopoll_conn) {
        nopoll_conn_close(g_nopoll_conn);
    }
    if (g_nopoll_ctx) {
        nopoll_ctx_unref(g_nopoll_ctx);
    }
    if (g_udp_send_fd != -1) {
        close(g_udp_send_fd);
    }
    
    return 0;
}