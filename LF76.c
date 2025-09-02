#include <nopoll/nopoll.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <jansson.h>
#include <signal.h>

// 全局变量
char session_id[64] = "";
int is_manual_mode = 0; // 0表示自动模式，1表示手动模式

// 消息回调函数
void websocket_on_message(noPollCtx *ctx, noPollConn *conn, noPollMsg *msg, noPollPtr user_data) {
    if (nopoll_msg_is_final(msg)) {
        const char *content = (const char *)nopoll_msg_get_payload(msg);
        printf("< %s\n", content);
        
        // 解析JSON消息
        json_error_t error;
        json_t *root = json_loads(content, 0, &error);
        
        if (!root) {
            fprintf(stderr, "JSON解析错误: %s\n", error.text);
            return;
        }
        
        // 检查消息类型
        json_t *type_obj = json_object_get(root, "type");
        if (type_obj && json_is_string(type_obj)) {
            const char *type = json_string_value(type_obj);
            
            if (strcmp(type, "asr") == 0) {
                // 语音识别结果
                json_t *text_obj = json_object_get(root, "text");
                if (text_obj && json_is_string(text_obj)) {
                    printf("识别结果: %s\n", json_string_value(text_obj));
                }
            }
        }
        
        json_decref(root);
    } else {
        printf("收到二进制消息，长度: %d\n", nopoll_msg_get_payload_size(msg));
    }
}

// 从消息中提取session_id
static int extract_session_id(const char *message) {
    if (message == NULL) {
        printf("Message is NULL\n");
        return -1;
    }
    
    // 解析JSON消息
    json_error_t error;
    json_t *root = json_loads(message, 0, &error);
    
    if (!root) {
        fprintf(stderr, "JSON解析错误: %s\n", error.text);
        return -1;
    }
    
    json_t *session_obj = json_object_get(root, "session_id");
    if (session_obj && json_is_string(session_obj)) {
        strncpy(session_id, json_string_value(session_obj), sizeof(session_id) - 1);
        printf("获取到session_id: %s\n", session_id);
        json_decref(root);
        return 0;
    }
    
    json_decref(root);
    return -1;
}

// 创建模拟的OPUS音频数据
// 这是一个简化的示例，实际应用中应该使用真正的OPUS编码器
unsigned char* create_mock_opus_data(size_t *length) {
    // 创建一个简单的模拟OPUS帧
    // 注意：这不是真正的OPUS编码数据，只是一个模拟
    static unsigned char mock_opus_frame[] = {
        0x4F, 0x70, 0x75, 0x73, 0x48, 0x65, 0x61, 0x64,  // "OpusHead" magic signature
        0x01, 0x00, 0x00, 0x00,  // Version
        0x01, 0x00, 0x00, 0x00,  // Channels
        0x80, 0x3E, 0x00, 0x00,  // Sample rate (16000)
        0x00, 0x00, 0x00, 0x00,  // Output gain
        0x00, 0x00, 0x00, 0x00   // Channel mapping
    };
    
    *length = sizeof(mock_opus_frame);
    unsigned char *data = malloc(*length);
    if (data) {
        memcpy(data, mock_opus_frame, *length);
    }
    return data;
}

int main(int argc, char *argv[]) {
    noPollCtx *ctx = nopoll_ctx_new();
    if (!ctx) {
        printf("无法创建 nopoll 上下文\n");
        return -1;
    }

    noPollConnOpts *opts = nopoll_conn_opts_new();
    if (!opts) {
        printf("无法创建连接选项\n");
        nopoll_ctx_unref(ctx);
        return -1;
    }

    // 设置额外请求头
    char extra_headers[1024];
    snprintf(extra_headers, sizeof(extra_headers),
             "Authorization: Bearer %s\r\n"
             "Device-Id: 00:0c:29:34:45:0f\r\n"
             "Client-Id: webai_test\r\n"
             "Protocol-Version: 1\r\n",
             "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJkYXRhIjoicEhQLUpvMG9Ja2ZTSldzcHFMeTBKWTJROUktNG5CeUVNei11QzdvcVlLa25oMVgxNGl3MEhoRmY2Wml5NXdveXVGVi1QcmxhbWVOcldYN0VFYVp0UVlKMF9SUkJkTkJlb0xYUjVqYUsifQ.sEw_86B8sprZPJdg-70oG9Des7cT1U8M4x08FCcHfxM");

    nopoll_conn_opts_set_extra_headers(opts, extra_headers);

    // 连接到 WebSocket
    const char *host = "xrobo-io.qiniuapi.com";
    const char *port = "443";
    const char *path = "/v1/ws/";
    noPollConn *ws_conn = nopoll_conn_tls_new(ctx, opts, host, port, NULL, path, NULL, NULL);
    if (!ws_conn) {
        printf("WebSocket connection created unsuccessfully\n");
        nopoll_conn_opts_free(opts);
        nopoll_ctx_unref(ctx);
        return -1;
    }

    // 等待连接就绪
    if (!nopoll_conn_wait_until_connection_ready(ws_conn, 10)) {
        printf("WebSocket connection not ready within timeout\n");
        nopoll_conn_close(ws_conn);
        nopoll_conn_opts_free(opts);
        nopoll_ctx_unref(ctx);
        return -1;
    }

    printf("WebSocket connection created successfully\n");

    // 检查连接状态
    if (!nopoll_conn_is_ready(ws_conn)) {
        printf("WebSocket connection is not ready\n");
        nopoll_conn_close(ws_conn);
        nopoll_conn_opts_free(opts);
        nopoll_ctx_unref(ctx);
        return -1;
    }

    // 设置消息回调
    nopoll_conn_set_on_msg(ws_conn, websocket_on_message, NULL);

    // 发送 hello 消息
    const char *hello_msg = "{\"type\":\"hello\",\"version\":1,\"transport\":\"websocket\",\"audio_params\":{\"format\":\"opus\",\"sample_rate\":16000,\"channels\":1,\"frame_duration\":60}}";
    nopoll_conn_send_text(ws_conn, hello_msg, strlen(hello_msg));
    printf("> %s\n", hello_msg);

    // 等待并获取hello响应
    int tries = 0;
    const char *content = NULL;
    noPollMsg *msg = NULL;
    
    while (tries < 50 && session_id[0] == '\0') {
        msg = nopoll_conn_get_msg(ws_conn);
        if (msg != NULL) {
            content = (const char *)nopoll_msg_get_payload(msg);
            printf("< %s\n", content);
            extract_session_id(content);
            nopoll_msg_unref(msg);
        }
        usleep(100000); // 100ms
        tries++;
    }

    if (session_id[0] == '\0') {
        printf("未能获取到session_id，退出\n");
        nopoll_conn_close(ws_conn);
        nopoll_conn_opts_free(opts);
        nopoll_ctx_unref(ctx);
        return -1;
    }

    // 发送开始监听消息
    char listen_msg[256];
    snprintf(listen_msg, sizeof(listen_msg),
             "{\"session_id\":\"%s\",\"type\":\"listen\",\"state\":\"start\",\"mode\":\"auto\"}",
             session_id);
    nopoll_conn_send_text(ws_conn, listen_msg, strlen(listen_msg));
    printf("> %s\n", listen_msg);

    // 发送模拟的OPUS音频数据
    printf("发送模拟OPUS音频数据...\n");
    
    size_t opus_length;
    unsigned char *opus_data = create_mock_opus_data(&opus_length);
    
    if (opus_data) {
        // 发送多帧音频数据
        for (int i = 0; i < 10; i++) {
            nopoll_conn_send_binary(ws_conn, opus_data, opus_length);
            printf("发送音频帧 %d, 长度: %zu\n", i + 1, opus_length);
            
            // 处理接收到的消息
            msg = nopoll_conn_get_msg(ws_conn);
            if (msg != NULL) {
                if (nopoll_msg_is_final(msg)) {
                    content = (const char *)nopoll_msg_get_payload(msg);
                    printf("< %s\n", content);
                } else {
                    printf("收到二进制消息，长度: %d\n", nopoll_msg_get_payload_size(msg));
                }
                nopoll_msg_unref(msg);
            }
            
            usleep(100000); // 100ms 间隔
        }
        
        free(opus_data);
    }

    // 发送停止监听消息
    // char stop_listen_msg[256];
    // snprintf(stop_listen_msg, sizeof(stop_listen_msg),
    //          "{\"session_id\":\"%s\",\"type\":\"listen\",\"state\":\"auto\"}",
    //          session_id);
    // nopoll_conn_send_text(ws_conn, stop_listen_msg, strlen(stop_listen_msg));
    // printf("> %s\n", stop_listen_msg);

    // 保持连接，处理事件
    printf("等待服务器响应...\n");
    for (int i = 0; i < 200; i++) {
        msg = nopoll_conn_get_msg(ws_conn);
        if (msg != NULL) {
            if (nopoll_msg_is_final(msg)) {
                content = (const char *)nopoll_msg_get_payload(msg);
                printf("< %s\n", content);
            } else {
                printf("收到二进制消息，长度: %d\n", nopoll_msg_get_payload_size(msg));
            }
            nopoll_msg_unref(msg);
        }
        usleep(100000); // 100ms
    }

    // 清理资源
    nopoll_conn_close(ws_conn);
    nopoll_conn_opts_free(opts);
    nopoll_ctx_unref(ctx);
    
    printf("程序结束\n");
    return 0;
}