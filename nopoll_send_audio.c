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
#include "opus_data.h"

//#define OTA_URL "http://114.66.50.145:8003/xiaozhi/ota/"
#define OTA_URL "https://xrobo.qiniuapi.com/v1/ota/"
#define MAC "d4:06:06:b6:a9:fb"
#define UUID "webai_test"

static char g_session_id[128] = {0};
static char g_ws_token[512] = {0};
static char g_ws_url[512]   = {0};

static noPollConn *g_nopoll_conn = NULL;
static noPollCtx *g_nopoll_ctx = NULL;
static int g_connected = 0;
static int g_shaked = 0;

static volatile int g_listen_active = 0;
static volatile int g_audio_thread_ready = 0;


// 修改 opus_memory_reader_thread 函数
static void *opus_memory_reader_thread(void *arg) {

    sleep(2);
    while(1){
        nopoll_loop_wait(g_nopoll_ctx, 1000);
    }

}

// 修改 opus_memory_reader_thread 函数
static void opus_memory_reader_send_test(void)
 {
    int frame_count = 0;
    int result = 0;
    uint8_t len_bytes[4];
    g_audio_thread_ready = 1;
    int tries = 0;

    printf("开始解析并发送opus音频数据...\n");
     printf("listen\n");
     char start_buf[256];
    int n = snprintf(start_buf, sizeof(start_buf),
        "{\"session_id\":\"%s\",\"type\":\"listen\",\"state\":\"start\",\"mode\":\"manual\"}", g_session_id);
    int ret = nopoll_conn_send_text(g_nopoll_conn, start_buf, n);
    if (ret > 0) {
        printf("listen, 发送成功 %d\n", ret);
    } else if (ret == -2) {
        printf("listen, 需要重试发送\n");
    } else {
        printf("listen, 发送失败，错误码: %d\n", ret);
    }
    
    frame_count = 0;
    size_t offset = 0;
    while (offset < opus_audio_data_size) {
        // 读取帧长度（4字节，小端格式）
        if (offset + sizeof(int32_t) > opus_audio_data_size) {
            fprintf(stderr, "数据不完整，无法读取帧长度\n");
            break;
        }

        // 正确处理小端格式的帧长度解析
        memcpy(len_bytes, &opus_audio_data[offset], 4);
        int32_t opus_len = (len_bytes[0] << 0) | 
                        (len_bytes[1] << 8) | 
                        (len_bytes[2] << 16) | 
                        (len_bytes[3] << 24);
        offset += sizeof(int32_t);

        // 增加帧长度合理性检查
        if (opus_len <= 0 || opus_len > 1024 * 1024) {  // 限制最大帧长为1MB
            fprintf(stderr, "无效的帧长度: %d字节，跳过该帧\n", opus_len);
            continue;
        }

        printf("开始发送第%d帧, 长度: %d字节,\n", frame_count + 1, opus_len);

        tries = 0;
        // 发送当前帧的音频数据
        int result = nopoll_conn_send_binary(g_nopoll_conn, 
                                            (const char *)&opus_audio_data[offset], 
                                            opus_len);
        if (result > 0) {
            printf("成功发送第%d帧, 长度: %d字节, 总进度: %zu/%u字节\n", 
                frame_count + 1, result, offset + opus_len, opus_audio_data_size);
                
        } else{

            printf("第%d帧发送阻塞，需要重试\n", frame_count + 1);
            offset -= sizeof(int32_t);
            usleep(1000);
            continue; 
        }
        
        offset += opus_len;
        frame_count++;
        // 模拟实时发送的间隔（60ms，符合Opus帧时长）
        usleep(60000);
    }

    printf("音频数据发送完成，共发送%d帧，总大小: %zu/%u字节\n", frame_count, offset, opus_audio_data_size);
    
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
    //  noPollMsg *msg = NULL;
    // if (msg != NULL) {
    //     const char *content = (const char *)nopoll_msg_get_payload(msg);
    //     size_t msg_size = nopoll_msg_get_payload_size(msg);
    //     printf("< %s\n", content);  
    //     nopoll_msg_unref(msg);
    //     return 0;
    // } else {
    //     printf("No message received after ms\n");
    //     return -1;
    // }

}


static int wait_and_process_websocket_message(noPollConn *conn, int timeout_ms) {
    if (conn == NULL) {
        printf("Connection is NULL\n");
        return -1;
    }
    
    noPollMsg *msg = NULL;
    int attempts = 0;
    const int max_attempts = timeout_ms / 10; // 每10ms尝试一次
    
    while (msg == NULL && attempts < max_attempts) {
        msg = nopoll_conn_get_msg(conn);
        if (msg == NULL) {
            usleep(10000); // 等待10ms
            attempts++;
        }
    }
    
    if (msg != NULL) {
        const char *content = (const char *)nopoll_msg_get_payload(msg);
        size_t msg_size = nopoll_msg_get_payload_size(msg);
        printf("< %s\n", content);  
        nopoll_msg_unref(msg);
        return 0;
    } else {
        printf("No message received after %dms\n", timeout_ms);
        return -1;
    }
}

// static void *opus_memory_reader_thread(void *arg) {
//     while(1){
//         wait_and_process_websocket_message(g_nopoll_conn,5000);
//     }
// }

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
    curl_easy_setopt(curl, CURLOPT_POST, 1);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post_data);
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_memory_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);
    //curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)post_data);
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

    printf("请求回复内容如下：\r\n%s\r\n",chunk.memory);

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
        //printf("收到文本消息: %.*s\n", (int)len, content);
        
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
                else if (!strcmp(type, "tts") ) {
                    const char *state = json_string_value(json_object_get(root, "state"));
                    const char *text = json_string_value(json_object_get(root, "text"));

                    printf("语音合成 %s: %s\n", state ? state : "", text ? text : "");
                }else if (!strcmp(type, "llm")) {
                    const char *text = json_string_value(json_object_get(root, "text"));
                    const char *emotion = json_string_value(json_object_get(root, "emotion"));
                    printf("大模型回复: %s (情绪: %s)\n", text ? text : "", emotion ? emotion : "");
                } 
                else {
                    printf("未知消息类型: %s\n", type);
                }
            }
            json_decref(root);
        } else {
            printf("收到二进制音频数据，长度: %zu字节\n", len);
            //printf("JSON解析失败: %s\n", jerr.text);
        }
    } else {
        printf("nopoll_msg_is_final faild!\n");
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

    int ret=0;
    struct ws_thread_args *cfg = (struct ws_thread_args *)arg;
    g_nopoll_ctx = nopoll_ctx_new();
    if (!g_nopoll_ctx) {
        printf("无法创建 nopoll 上下文\n");
        free(cfg);
        return NULL;
    }

    noPollConnOpts  *opts = nopoll_conn_opts_new();
    if (!opts) {
        printf("无法创建连接选项\n");
        nopoll_ctx_unref(g_nopoll_ctx);
        free(cfg);
        return NULL;
    }

     printf("g_ws_token:%s \n", g_ws_token);

    // 设置额外请求头
    char extra_headers[1024];
    snprintf(extra_headers, sizeof(extra_headers),
             "\r\nPragma: no-cache\r\n"
             "Cache-Control: no-cache\r\n"
             "Authorization: Bearer %s\r\n"
             "Device-Id: %s\r\n"
             "Client-Id: %s\r\n"
             "Protocol-Version: 1",
             g_ws_token, MAC, UUID);
    nopoll_conn_opts_set_extra_headers(opts, extra_headers);
    
    //for(int i=0;i<)

    // 连接到 WebSocket
    // const char *host = "xrobo-io.qiniuapi.com";
    // const char *port = "80";
    // const char *path = "/v1/ws/";
    // const char *host = "192.168.20.90";
    // const char *port = "80";

    const char *host = cfg->host;
    const char *port = cfg->port;
    const char *path = cfg->path;

    printf("正在连接：host=%s,port=%s,path=%s\r\n",host,port,path);

    g_nopoll_conn = nopoll_conn_new_opts (g_nopoll_ctx,
				   opts,
				   host, 
				   port, 
				   "xrobo-io.qiniuapi.com",
				   path, 
				   "voice-client",
				   "http://");


    //g_nopoll_conn = nopoll_conn_new(g_nopoll_ctx, host, port, host, NULL, NULL,NULL);
    if (g_nopoll_conn == NULL) {
        printf("WebSocket connection created unsuccessfully\n");
        nopoll_conn_opts_free(opts);
        nopoll_ctx_unref(g_nopoll_ctx);
        free(cfg);
        return NULL;
    }
    printf("发送websocket连接完成\n");
    
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


    printf("发送hello消息\n");
    // 发送hello消息
    const char *hello_msg = "{\"type\":\"hello\",\"version\":1,\"transport\":\"websocket\",\"audio_params\":{\"format\":\"opus\",\"sample_rate\":16000,\"channels\":1,\"frame_duration\":60}}";
    ret = nopoll_conn_send_text(g_nopoll_conn, hello_msg, strlen(hello_msg));
    if(ret>0){
        printf("hello, 已发送 hello: %s\n", hello_msg);
    }else {
        printf("hello, 发送失败\n");
    }

    opus_memory_reader_send_test();

    while (1) {
       sleep(3); 
    }
    return NULL;
}

// static void *webmsg_loop_thread(void *arg) {
//     while (1) {
//        nopoll_loop_wait(g_nopoll_ctx,1000);
//     }
// }

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

    // // 启动opus内存数据读取线程
    pthread_t opus_tid;
    if (pthread_create(&opus_tid, NULL, opus_memory_reader_thread, NULL) != 0) {
        perror("创建opus数据读取线程失败");
        return 1;
    }
    // 主循环等待处理完成
    printf("等待处理完成...\n");
    while (1) {
        sleep(1);
    }
    
    return 0;
}