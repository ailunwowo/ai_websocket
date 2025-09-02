#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <alsa/asoundlib.h>
#include <opus/opus.h>

// 音频参数定义
#define PROTOCOL_VERSION 1
#define SAMPLE_RATE 16000
#define CHANNELS 1
#define FRAME_DURATION_MS 60
#define FRAME_SIZE (SAMPLE_RATE * FRAME_DURATION_MS / 1000)  // 960 samples
#define RECORD_SECONDS 3
#define MAX_PACKET_SIZE (3*1276)

// 音频文件头结构
typedef struct {
    uint32_t protocol_version;
    uint32_t sample_rate;
    uint32_t channels;
    uint32_t frame_duration_ms;
    uint32_t data_size;  // Opus数据总大小
} audio_header_t;

volatile sig_atomic_t stop_recording = 0;

void handle_signal(int sig) {
    stop_recording = 1;
}

int main() {
    signal(SIGINT, handle_signal);
    
    int err;
    snd_pcm_t *capture_handle;
    snd_pcm_hw_params_t *hw_params;
    OpusEncoder *encoder;
    unsigned char opus_data[MAX_PACKET_SIZE];
    opus_int32 opus_len;
    
    // 创建输出文件
    FILE *output_file = fopen("audio.opus", "wb");
    if (!output_file) {
        perror("无法创建输出文件");
        return 1;
    }

    // 初始化ALSA
    if ((err = snd_pcm_open(&capture_handle, "default", SND_PCM_STREAM_CAPTURE, 0)) < 0) {
        fprintf(stderr, "无法打开音频设备: %s\n", snd_strerror(err));
        fclose(output_file);
        return 1;
    }

    // 设置ALSA参数
    snd_pcm_hw_params_alloca(&hw_params);
    snd_pcm_hw_params_any(capture_handle, hw_params);
    snd_pcm_hw_params_set_access(capture_handle, hw_params, SND_PCM_ACCESS_RW_INTERLEAVED);
    snd_pcm_hw_params_set_format(capture_handle, hw_params, SND_PCM_FORMAT_S16_LE);
    
    unsigned int sample_rate = SAMPLE_RATE;
    snd_pcm_hw_params_set_rate_near(capture_handle, hw_params, &sample_rate, 0);
    snd_pcm_hw_params_set_channels(capture_handle, hw_params, CHANNELS);
    snd_pcm_hw_params(capture_handle, hw_params);
    snd_pcm_prepare(capture_handle);

    // 初始化Opus编码器
    int opus_err;
    encoder = opus_encoder_create(SAMPLE_RATE, CHANNELS, OPUS_APPLICATION_VOIP, &opus_err);
    if (opus_err != OPUS_OK) {
        fprintf(stderr, "无法创建Opus编码器: %s\n", opus_strerror(opus_err));
        snd_pcm_close(capture_handle);
        fclose(output_file);
        return 1;
    }

    // 设置Opus编码器参数
    opus_encoder_ctl(encoder, OPUS_SET_BITRATE(16000));
    opus_encoder_ctl(encoder, OPUS_SET_COMPLEXITY(8));

    printf("开始录制3秒音频...\n");
    printf("音频参数:\n");
    printf("  协议版本: %d\n", PROTOCOL_VERSION);
    printf("  采样率: %dHz\n", SAMPLE_RATE);
    printf("  通道数: %d\n", CHANNELS);
    printf("  帧长: %dms\n", FRAME_DURATION_MS);
    printf("按Ctrl+C可提前停止录制\n");

    short *pcm_buffer = (short *)malloc(FRAME_SIZE * CHANNELS * sizeof(short));
    if (!pcm_buffer) {
        fprintf(stderr, "内存分配失败\n");
        opus_encoder_destroy(encoder);
        snd_pcm_close(capture_handle);
        fclose(output_file);
        return 1;
    }

    // 先预留文件头位置，最后再写入
    audio_header_t header = {
        .protocol_version = PROTOCOL_VERSION,
        .sample_rate = SAMPLE_RATE,
        .channels = CHANNELS,
        .frame_duration_ms = FRAME_DURATION_MS,
        .data_size = 0
    };
    
    // 跳过文件头，先写数据
    fseek(output_file, sizeof(audio_header_t), SEEK_SET);

    time_t start_time = time(NULL);
    int frames_encoded = 0;
    uint32_t total_data_size = 0;

    while (!stop_recording && (time(NULL) - start_time) < RECORD_SECONDS) {
        // 读取PCM数据
        err = snd_pcm_readi(capture_handle, pcm_buffer, FRAME_SIZE);
        if (err != FRAME_SIZE) {
            if (err == -EPIPE) {
                snd_pcm_prepare(capture_handle);
                continue;
            }
            fprintf(stderr, "读取音频数据错误: %s\n", snd_strerror(err));
            break;
        }

        // 编码为Opus
        opus_len = opus_encode(encoder, pcm_buffer, FRAME_SIZE, opus_data, MAX_PACKET_SIZE);
        if (opus_len < 0) {
            fprintf(stderr, "编码错误: %s\n", opus_strerror(opus_len));
            break;
        }

        // 写入数据：先写帧长度，再写Opus数据
        fwrite(&opus_len, sizeof(opus_len), 1, output_file);
        fwrite(opus_data, 1, opus_len, output_file);
        
        total_data_size += sizeof(opus_len) + opus_len;
        frames_encoded++;
        
        printf("已编码 %d 帧音频，总数据量: %u 字节\r", 
               frames_encoded, total_data_size);
        fflush(stdout);
    }

    // 更新文件头并写入文件开头
    header.data_size = total_data_size;
    fseek(output_file, 0, SEEK_SET);
    fwrite(&header, sizeof(header), 1, output_file);

    printf("\n录制完成！\n");
    printf("共编码 %d 帧音频，总数据量: %u 字节\n", frames_encoded, total_data_size);
    printf("输出文件: audio.opus\n");
    printf("文件格式:\n");
    printf("  文件头: %zu 字节\n", sizeof(audio_header_t));
    printf("  音频数据: %u 字节\n", total_data_size);

    // 清理资源
    free(pcm_buffer);
    opus_encoder_destroy(encoder);
    snd_pcm_close(capture_handle);
    fclose(output_file);

    return 0;
}