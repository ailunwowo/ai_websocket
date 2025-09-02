# ai_websocket

#### 介绍
用于测试七牛云的云平台语音对话链接，使用Linux C的方式

#### 文件说明
.
├── audio.opus            //原始opus编码数据
├── cfg.h                 //用于对接sonud_app 的配置文件
├── LF76.c                //主要程序，实现将opus数据发生到云端进行处理
├── opus_data.h           //audio.opus解析出来的数组格式数据
├── opus_recorder        //录音并将pcm转为opus编码的数据
├── opus_recorder.c     //录音并将pcm转为opus编码的数据
├── opus_to_array       //转换数据为opus的数组数据
├── opus_to_array.c    //转换数据为opus的数组数据
├── sound_app         //用于测试的udp传输音频数据程序
├── text.c            //测试记录
└── web               //主程序的可执行程序

编译方式

1.  gcc -o opus_recorder opus_recorder.c -lasound -lopus
2.  gcc opus_to_array.c -o opus_to_array
3.  gcc LF76.c -o web $(pkg-config --cflags --libs libwebsockets jansson nopoll libcurl)

#### 开发环境搭建
本人使用的系统为Ubuntu22.04

#### 使用说明

1.  xxxx
2.  xxxx
3.  xxxx

#### 参与贡献

1.  Fork 本仓库
2.  新建 Feat_xxx 分支
3.  提交代码
4.  新建 Pull Request