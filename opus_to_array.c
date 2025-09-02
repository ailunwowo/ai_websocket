#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 生成C数组文件
void generate_c_array(const char *input_filename, const char *output_filename, const char *array_name) {
    FILE *in_file, *out_file;
    unsigned char *buffer;
    long file_size;
    size_t bytes_read;
    int i;

    // 打开输入文件
    in_file = fopen(input_filename, "rb");
    if (!in_file) {
        perror("无法打开输入文件");
        exit(EXIT_FAILURE);
    }

    // 获取文件大小
    fseek(in_file, 0, SEEK_END);
    file_size = ftell(in_file);
    rewind(in_file);

    // 分配内存缓冲区
    buffer = (unsigned char *)malloc(file_size);
    if (!buffer) {
        perror("内存分配失败");
        fclose(in_file);
        exit(EXIT_FAILURE);
    }

    // 读取文件内容
    bytes_read = fread(buffer, 1, file_size, in_file);
    if (bytes_read != file_size) {
        perror("读取文件失败");
        free(buffer);
        fclose(in_file);
        exit(EXIT_FAILURE);
    }
    fclose(in_file);

    // 打开输出文件
    out_file = fopen(output_filename, "w");
    if (!out_file) {
        perror("无法打开输出文件");
        free(buffer);
        exit(EXIT_FAILURE);
    }

    // 写入C数组定义
    fprintf(out_file, "// 自动生成的音频数据数组\n");
    fprintf(out_file, "// 源文件: %s\n", input_filename);
    fprintf(out_file, "// 数据大小: %ld 字节\n\n", file_size);
    fprintf(out_file, "const unsigned int %s_size = %ld;\n", array_name, file_size);
    fprintf(out_file, "const unsigned char %s[] = {\n", array_name);

    // 写入数组元素
    for (i = 0; i < file_size; i++) {
        fprintf(out_file, "0x%02X", buffer[i]);
        if (i < file_size - 1) {
            fprintf(out_file, ", ");
        }
        
        // 每16个元素换行，提高可读性
        if ((i + 1) % 16 == 0) {
            fprintf(out_file, "\n");
        }
    }

    fprintf(out_file, "\n};\n");
    fclose(out_file);
    free(buffer);

    printf("成功生成C数组文件: %s\n", output_filename);
    printf("数组名称: %s\n", array_name);
    printf("数据大小: %ld 字节\n", file_size);
}

int main(int argc, char *argv[]) {
    const char *input_filename = "audio.opus";
    const char *output_filename = "opus_data.h";
    const char *array_name = "opus_audio_data";

    // 解析命令行参数
    if (argc > 1) input_filename = argv[1];
    if (argc > 2) output_filename = argv[2];
    if (argc > 3) array_name = argv[3];

    printf("将 %s 转换为C数组...\n", input_filename);
    generate_c_array(input_filename, output_filename, array_name);

    return 0;
}
