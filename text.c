PS D:\DIY\BF2_AI\xiaozhi-python> python.exe .\qiniu_xz.py
2025-08-19 16:00:00,668 - INFO - OTA response: {'server_time': {'timestamp': 1755590400672, 'timeZone': 'Asia/Shanghai', 'timezone_offset': 480}, 'activation': {'code': '291665', 'message': 'https://xrobo.qiniu.com\n291665', 'challenge': 'D4:06:06:B6:A9:FB'}, 'firmware': {'version': '1.0.0', 'url': 'https://xrobo.qiniuapi.com/v1/ota/INVALID_FIRMWARE_FOR_TEST'}, 'websocket': {'url': 'ws://xrobo-io.qiniuapi.com/v1/ws/'}}
2025-08-19 16:00:00,668 - INFO - Firmware version: 1.0.0
2025-08-19 16:00:00,668 - INFO - Firmware download url: https://xrobo.qiniuapi.com/v1/ota/INVALID_FIRMWARE_FOR_TEST
2025-08-19 16:00:00,669 - INFO - Activation code: 291665
2025-08-19 16:00:00,669 - INFO - WebSocket URL: ws://xrobo-io.qiniuapi.com/v1/ws/, Token: None
2025-08-19 16:00:00,669 - INFO - 请在云端使用激活码完成绑定
2025-08-19 16:00:01,328 - INFO - 等待设备激活...
2025-08-19 16:00:06,626 - INFO - OTA response: {'server_time': {'timestamp': 1755590406625, 'timeZone': 'Asia/Shanghai', 'timezone_offset': 480}, 'firmware': {'version': '1.0.0', 'url': 'https://xrobo.qiniuapi.com/v1/ota/INVALID_FIRMWARE_FOR_TEST'}, 'websocket': {'url': 'ws://xrobo-io.qiniuapi.com/v1/ws/', 'token': 'eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJkYXRhIjoiYVlxeTJWc29hRjZYRG5GOFEyaF9Iakc2MTdldWNPemlpQXByU0lhYWJMRTR5dHp2dkJnYkY1NUg5d0dkdE5PV2xjSUxTVmUxbXBlMG9XRjAxajd6UXhaTE5ZaDdUZ20xcU12U1JHS1oifQ.voXNmfbG9DO1pb9wZn0gSrc2CbL6q8cNwxsKFhJqp7w'}}
2025-08-19 16:00:06,626 - INFO - Firmware version: 1.0.0
2025-08-19 16:00:06,626 - INFO - Firmware download url: https://xrobo.qiniuapi.com/v1/ota/INVALID_FIRMWARE_FOR_TEST
2025-08-19 16:00:06,626 - INFO - WebSocket URL: ws://xrobo-io.qiniuapi.com/v1/ws/, Token: eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJkYXRhIjoiYVlxeTJWc29hRjZYRG5GOFEyaF9Iakc2MTdldWNPemlpQXByU0lhYWJMRTR5dHp2dkJnYkY1NUg5d0dkdE5PV2xjSUxTVmUxbXBlMG9XRjAxajd6UXhaTE5ZaDdUZ20xcU12U1JHS1oifQ.voXNmfbG9DO1pb9wZn0gSrc2CbL6q8cNwxsKFhJqp7w
2025-08-19 16:00:06,626 - INFO - 请在云端使用激活码完成绑定
2025-08-19 16:00:06,627 - INFO - 设备激活成功，启动 WebSocket 连接
2025-08-19 16:00:06,770 - INFO - Websocket connected
2025-08-19 16:00:06,770 - INFO - ==================================
2025-08-19 16:00:06,771 - INFO - send message: {'type': 'hello', 'version': 1, 'transport': 'websocket', 'audio_params': {'format': 'opus', 'sample_rate': 16000, 'channels': 1, 'frame_duration': 60}}
2025-08-19 16:00:06,773 - INFO - recv msg: {'type': 'hello', 'version': 1, 'transport': 'websocket', 'audio_params': {'format': 'opus', 'sample_rate': 16000, 'channels': 1, 'frame_duration': 60}, 'session_id': '782b8536-ba6e-45da-a5ed-6d4ce622ebdc'}
2025-08-19 16:00:06,825 - INFO - recv msg: {'type': 'hello', 'version': 1, 'transport': 'websocket', 'audio_params': {'format': 'opus', 'sample_rate': 16000, 'channels': 1, 'frame_duration': 60, 'bits_per_sample': 16}, 'session_id': '782b8536-ba6e-45da-a5ed-6d4ce622ebdc'}
2025-08-19 16:00:06,825 - INFO - send_audio_thread is alive
2025-08-19 16:00:17,324 - INFO - send message: {'session_id': '782b8536-ba6e-45da-a5ed-6d4ce622ebdc', 'type': 'listen', 'state': 'start', 'mode': 'manual'}
2025-08-19 16:00:19,445 - INFO - send message: {'session_id': '782b8536-ba6e-45da-a5ed-6d4ce622ebdc', 'type': 'listen', 'state': 'stop'}
2025-08-19 16:00:21,766 - INFO - send message: {'session_id': '782b8536-ba6e-45da-a5ed-6d4ce622ebdc', 'type': 'listen', 'state': 'start', 'mode': 'manual'}
2025-08-19 16:00:23,843 - INFO - send message: {'session_id': '782b8536-ba6e-45da-a5ed-6d4ce622ebdc', 'type': 'listen', 'state': 'stop'}
2025-08-19 16:00:24,659 - INFO - recv msg: {'type': 'stt', 'text': '你好。', 'session_id': '782b8536-ba6e-45da-a5ed-6d4ce622ebdc'}
2025-08-19 16:00:24,660 - INFO - recv msg: {'type': 'tts', 'state': 'start', 'session_id': '782b8536-ba6e-45da-a5ed-6d4ce622ebdc'}
2025-08-19 16:00:25,121 - INFO - recv msg: {'type': 'llm', 'text': '😍', 'emotion': 'loving', 'session_id': '782b8536-ba6e-45da-a5ed-6d4ce622ebdc'}
2025-08-19 16:00:25,122 - INFO - recv msg: {'type': 'tts', 'state': 'sentence_start', 'session_id': '782b8536-ba6e-45da-a5ed-6d4ce622ebdc', 'text': '嗨你好呀小宝贝～'}
2025-08-19 16:00:25,128 - INFO - recv msg: {'type': 'tts', 'state': 'sentence_end', 'session_id': '782b8536-ba6e-45da-a5ed-6d4ce622ebdc', 'text': '嗨你好呀小宝贝～'}
2025-08-19 16:00:25,293 - INFO - recv msg: {'type': 'tts', 'state': 'sentence_start', 'session_id': '782b8536-ba6e-45da-a5ed-6d4ce622ebdc'}
2025-08-19 16:00:25,410 - INFO - recv msg: {'type': 'tts', 'state': 'sentence_end', 'session_id': '782b8536-ba6e-45da-a5ed-6d4ce622ebdc'}
2025-08-19 16:00:25,410 - INFO - recv msg: {'type': 'tts', 'state': 'sentence_start', 'session_id': '782b8536-ba6e-45da-a5ed-6d4ce622ebdc'}
2025-08-19 16:00:25,691 - INFO - recv msg: {'type': 'tts', 'state': 'sentence_end', 'session_id': '782b8536-ba6e-45da-a5ed-6d4ce622ebdc'}
2025-08-19 16:00:25,691 - INFO - recv msg: {'type': 'tts', 'state': 'sentence_start', 'session_id': '782b8536-ba6e-45da-a5ed-6d4ce622ebdc'}
2025-08-19 16:00:25,946 - INFO - recv msg: {'type': 'tts', 'state': 'sentence_end', 'session_id': '782b8536-ba6e-45da-a5ed-6d4ce622ebdc'}
2025-08-19 16:00:25,946 - INFO - recv msg: {'type': 'tts', 'state': 'sentence_start', 'session_id': '782b8536-ba6e-45da-a5ed-6d4ce622ebdc'}
2025-08-19 16:00:26,266 - INFO - recv msg: {'type': 'tts', 'state': 'sentence_end', 'session_id': '782b8536-ba6e-45da-a5ed-6d4ce622ebdc'}
2025-08-19 16:00:26,266 - INFO - recv msg: {'type': 'tts', 'state': 'sentence_start', 'session_id': '782b8536-ba6e-45da-a5ed-6d4ce622ebdc'}
2025-08-19 16:00:26,584 - INFO - recv msg: {'type': 'tts', 'state': 'sentence_end', 'session_id': '782b8536-ba6e-45da-a5ed-6d4ce622ebdc'}
2025-08-19 16:00:26,585 - INFO - recv msg: {'type': 'tts', 'state': 'sentence_start', 'session_id': '782b8536-ba6e-45da-a5ed-6d4ce622ebdc'}
2025-08-19 16:00:26,756 - INFO - recv msg: {'type': 'tts', 'state': 'sentence_end', 'session_id': '782b8536-ba6e-45da-a5ed-6d4ce622ebdc'}
2025-08-19 16:00:26,756 - INFO - recv msg: {'type': 'tts', 'state': 'sentence_start', 'session_id': '782b8536-ba6e-45da-a5ed-6d4ce622ebdc'}
2025-08-19 16:00:27,406 - INFO - recv msg: {'type': 'tts', 'state': 'sentence_end', 'session_id': '782b8536-ba6e-45da-a5ed-6d4ce622ebdc'}
2025-08-19 16:00:27,407 - INFO - recv msg: {'type': 'llm', 'text': '🙂', 'emotion': 'happy', 'session_id': '782b8536-ba6e-45da-a5ed-6d4ce622ebdc'}
2025-08-19 16:00:27,407 - INFO - recv msg: {'type': 'tts', 'state': 'sentence_start', 'session_id': '782b8536-ba6e-45da-a5ed-6d4ce622ebdc', 'text': '我是嘟嘟熊一个温暖又友善的泰迪熊。'}
2025-08-19 16:00:27,407 - INFO - recv msg: {'type': 'tts', 'state': 'sentence_end', 'session_id': '782b8536-ba6e-45da-a5ed-6d4ce622ebdc', 'text': '我是嘟嘟熊一个温暖又友善的泰迪熊。'}
2025-08-19 16:00:27,408 - INFO - recv msg: {'type': 'tts', 'state': 'sentence_start', 'session_id': '782b8536-ba6e-45da-a5ed-6d4ce622ebdc'}
2025-08-19 16:00:30,966 - INFO - recv msg: {'type': 'tts', 'state': 'sentence_end', 'session_id': '782b8536-ba6e-45da-a5ed-6d4ce622ebdc'}
2025-08-19 16:00:30,966 - INFO - recv msg: {'type': 'llm', 'text': '🤔', 'emotion': 'thinking', 'session_id': '782b8536-ba6e-45da-a5ed-6d4ce622ebdc'}
2025-08-19 16:00:30,966 - INFO - recv msg: {'type': 'tts', 'state': 'sentence_start', 'session_id': '782b8536-ba6e-45da-a5ed-6d4ce622ebdc', 'text': '今天过得怎么样呀？'}
2025-08-19 16:00:30,967 - INFO - recv msg: {'type': 'tts', 'state': 'sentence_end', 'session_id': '782b8536-ba6e-45da-a5ed-6d4ce622ebdc', 'text': '今天过得怎么样呀？'}
2025-08-19 16:00:30,967 - INFO - recv msg: {'type': 'tts', 'state': 'sentence_start', 'session_id': '782b8536-ba6e-45da-a5ed-6d4ce622ebdc'}
2025-08-19 16:00:33,366 - INFO - recv msg: {'type': 'tts', 'state': 'sentence_end', 'session_id': '782b8536-ba6e-45da-a5ed-6d4ce622ebdc'}
2025-08-19 16:00:33,367 - INFO - recv msg: {'type': 'llm', 'text': '😎', 'emotion': 'cool', 'session_id': '782b8536-ba6e-45da-a5ed-6d4ce622ebdc'}
2025-08-19 16:00:33,367 - INFO - recv msg: {'type': 'tts', 'state': 'sentence_start', 'session_id': '782b8536-ba6e-45da-a5ed-6d4ce622ebdc', 'text': '我最喜欢和小朋友聊天了，你愿意和我分享一下今天发生的事情吗？'}
2025-08-19 16:00:33,367 - INFO - recv msg: {'type': 'tts', 'state': 'sentence_end', 'session_id': '782b8536-ba6e-45da-a5ed-6d4ce622ebdc', 'text': '我最喜欢和小朋友聊天了，你愿意和我分享一下今天发生的事情吗？'}
2025-08-19 16:00:33,367 - INFO - recv msg: {'type': 'tts', 'state': 'sentence_start', 'session_id': '782b8536-ba6e-45da-a5ed-6d4ce622ebdc'}
2025-08-19 16:00:38,286 - INFO - recv msg: {'type': 'tts', 'state': 'sentence_end', 'session_id': '782b8536-ba6e-45da-a5ed-6d4ce622ebdc'}
2025-08-19 16:00:38,286 - INFO - recv msg: {'type': 'llm', 'text': '🤔', 'emotion': 'thinking', 'session_id': '782b8536-ba6e-45da-a5ed-6d4ce622ebdc'}
2025-08-19 16:00:38,286 - INFO - recv msg: {'type': 'tts', 'state': 'sentence_start', 'session_id': '782b8536-ba6e-45da-a5ed-6d4ce622ebdc', 'text': '或者，你想听一个有趣的故事吗？'}
2025-08-19 16:00:38,287 - INFO - recv msg: {'type': 'tts', 'state': 'sentence_end', 'session_id': '782b8536-ba6e-45da-a5ed-6d4ce622ebdc', 'text': '或者，你想听一个有趣的故事吗？'}
2025-08-19 16:00:38,287 - INFO - recv msg: {'type': 'tts', 'state': 'sentence_start', 'session_id': '782b8536-ba6e-45da-a5ed-6d4ce622ebdc'}
2025-08-19 16:00:41,286 - INFO - recv msg: {'type': 'tts', 'state': 'sentence_end', 'session_id': '782b8536-ba6e-45da-a5ed-6d4ce622ebdc'}
2025-08-19 16:00:41,287 - INFO - recv msg: {'type': 'llm', 'text': '🙂', 'emotion': 'happy', 'session_id': '782b8536-ba6e-45da-a5ed-6d4ce622ebdc'}
2025-08-19 16:00:41,287 - INFO - recv msg: {'type': 'tts', 'state': 'sentence_start', 'session_id': '782b8536-ba6e-45da-a5ed-6d4ce622ebdc', 'text': '我可是有很多很多故事呢'}
2025-08-19 16:00:41,287 - INFO - recv msg: {'type': 'tts', 'state': 'sentence_end', 'session_id': '782b8536-ba6e-45da-a5ed-6d4ce622ebdc', 'text': '我可是有很多很多故事呢'}
2025-08-19 16:00:41,287 - INFO - recv msg: {'type': 'tts', 'state': 'sentence_start', 'session_id': '782b8536-ba6e-45da-a5ed-6d4ce622ebdc'}
2025-08-19 16:00:43,806 - INFO - recv msg: {'type': 'tts', 'state': 'sentence_end', 'session_id': '782b8536-ba6e-45da-a5ed-6d4ce622ebdc'}
2025-08-19 16:00:43,806 - INFO - recv msg: {'type': 'tts', 'state': 'sentence_start', 'session_id': '782b8536-ba6e-45da-a5ed-6d4ce622ebdc'}
2025-08-19 16:00:43,806 - INFO - recv msg: {'type': 'tts', 'state': 'sentence_end', 'session_id': '782b8536-ba6e-45da-a5ed-6d4ce622ebdc'}
2025-08-19 16:00:43,806 - INFO - recv msg: {'type': 'tts', 'state': 'stop', 'session_id': '782b8536-ba6e-45da-a5ed-6d4ce622ebdc'}

{"session_id":"3c0947ae-840a-47ff-a1b6-396599279a4d","type":"listen","state":"start","mode":"manual"}
{"session_id":"3c0947ae-840a-47ff-a1b6-396599279a4d","type":"listen","state":"stop"}



ota请求地址：https://xrobo.qiniuapi.com/v1/ota/   POST /api/ota/
1，连续发送两次ota请求，第一次获取到激活码activation code：{"server_time":{"timestamp":1755659709282,"timeZone":"Asia/Shanghai","timezone_offset":480},"activation":{"code":"804971","message":"https://xrobo.qiniu.com\n804971","challenge":"00:0c:29:34:45:0f"},"firmware":{"version":"1.0.0","url":"https://xrobo.qiniuapi.com/v1/ota/INVALID_FIRMWARE_FOR_TEST"},"websocket":{"url":"ws://xrobo-io.qiniuapi.com/v1/ws/"}}
2，激活后在次发送获取到token以及websocket请求的地址url！{"server_time":{"timestamp":1755659726619,"timeZone":"Asia/Shanghai","timezone_offset":480},"firmware":{"version":"1.0.0","url":"https://xrobo.qiniuapi.com/v1/ota/INVALID_FIRMWARE_FOR_TEST"},"websocket":{"url":"ws://xrobo-io.qiniuapi.com/v1/ws/","token":"eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJkYXRhIjoiVE9KbGFja2VVUTZjb2lVdlUweFNfT05sbjJrOUh5OUh3V1dEQnE2SWU2em1QMDNxUXgwYWZmUVFxTFpwTVpMc1pET1JoSU9fdE5YWkdtNFN6a0R6emZ6akNDUlc0NUx0X002cHd1T0QifQ.FEAncYHuoJ1i1FPVlW7IYtJbWn_cVBzLfGbDNrJsB5c"}}
curl -v -X POST https://xrobo.qiniuapi.com/v1/ota/ \
  -H "Content-Type: application/json" \
  -H "Device-Id: 00:0c:29:34:45:0f" \
  -H "Client-Id: webai_test" \
  -H "User-Agent: esp-box-3/1.5.6" \
  -H "Accept-Language: zh-CN" \
  -H "Activation-Version: 2" \
  -d '{
    "version": 0,
    "uuid": "webai_test",
    "application": {
      "name": "xiaoniu-web-test",
      "version": "1.0.0",
      "elf_sha256": "1234567"
    },
    "ota": { "label": "xiaoling-web-test" },
    "board": {
      "type": "xiaoling-web-test",
      "ssid": "xiaoling-web-test",
      "rssi": 0,
      "channel": 0,
      "ip": "192.168.1.1",
      "mac": "00:0c:29:34:45:0f"
    },
    "mac_address": "00:0c:29:34:45:0f",
    "chip_info": { "model": 0, "cores": 0, "revision": 0, "features": 0 },
    "partition_table": [
      { "label": "", "type": 0, "subtype": 0, "address": 0, "size": 0 }
    ]
  }'
3, 进行模拟对话，使用websocket进行连接发送
wscat -c ws://xrobo-io.qiniuapi.com/v1/ws/ -H "Authorization: Bearer eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJkYXRhIjoiaTFzMlRmWFJNczk4N2RkakxublZIUFd5WW9iT3hxVEo2TVdndEVKbVpqY2NZb09DMkJNS29lRkVwcVEzR21zREVydVQ3U0RTN19fODBvWm5HRG41aFU3dkJmQ1F4aXBqampoYldMd18ifQ.rM2g84RswczPN38KYC2BAtQPfF1oyhjequtska3Mkz8" -H "Device-Id: 00:0c:29:34:45:0f" -H "Client-Id: webai_test"

4，对话通信
alen@alen-virtual-machine:~/VisonFive2_SDK/xiaozhi-linux/sound_app$ wscat -c ws://xrobo-io.qiniuapi.com/v1/ws/ -H "Authorization: Bearer eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJkYXRhIjoiVE9KbGFja2VVUTZjb2lVdlUweFNfT05sbjJrOUh5OUh3V1dEQnE2SWU2em1QMDNxUXgwYWZmUVFxTFpwTVpMc1pET1JoSU9fdE5YWkdtNFN6a0R6emZ6akNDUlc0NUx0X002cHd1T0QifQ.FEAncYHuoJ1i1FPVlW7IYtJbWn_cVBzLfGbDNrJsB5c" -H "Device-Id: 00:0c:29:34:45:0f" -H "Client-Id: $(uuidgen)"
Connected (press CTRL+C to quit)
< {"type": "hello", "version": 1, "transport": "websocket", "audio_params": {"format": "opus", "sample_rate": 16000, "channels": 1, "frame_duration": 60}, "session_id": "6ad21e96-8ae1-413e-ac7c-660b82d4a32b"}
> {"type":"hello","version":1,"transport":"websocket","audio_params":{"format":"opus","sample_rate":16000,"channels":1,"frame_duration":60}}
< {"type": "hello", "version": 1, "transport": "websocket", "audio_params": {"format": "opus", "sample_rate": 16000, "channels": 1, "frame_duration": 60, "bits_per_sample": 16}, "session_id": "6ad21e96-8ae1-413e-ac7c-660b82d4a32b"}
> {"session_id": "782b8536-ba6e-45da-a5ed-6d4ce622ebdc", "type": "listen", "state": "start", "mode": "auto"}

gcc LF76.c -o web $(pkg-config --cflags --libs libwebsockets jansson nopoll libcurl)


alen@alen-virtual-machine:~/ai_work/ai_websocket$ ./web 
开始设备激活流程...
获取到 WebSocket URL: ws://xrobo-io.qiniuapi.com/v1/ws/
获取到 Token: eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJkYXRhIjoicmc5cmhCb0MyUVNqWFo3dHdOU3NZclEwejdUUExxZnFBUjRzMW40UjJLN0h4WTVIR3dxRldwMUpCR2dlZ3VPMUNvVDhHXzAxeS1JbVNpNHNLeGZYUWpMS0wwOW5XXzh3d0JNaFlhcW0ifQ.FsakIJe41BRhRQKpx76cT_agZs_olN7-BUTr3fwwbfw
激活成功，准备建立 WebSocket。
即将连接：ws://xrobo-io.qiniuapi.com:80/v1/ws/
等待处理完成...
等待WebSocket连接就绪...
WebSocket连接已建立
已发送 hello
收到文本消息: {"type": "hello", "version": 1, "transport": "websocket", "audio_params": {"format": "opus", "sample_rate": 16000, "channels": 1, "frame_duration": 60}, "session_id": "Hj0AAAB3M7q0gGEY"}
消息类型: hello
会话建立，session_id = Hj0AAAB3M7q0gGEY
Opus音频数据大小: 4483字节
已发送start命令，开始发送opus音频数据
收到文本消息: {"type": "hello", "version": 1, "transport": "websocket", "audio_params": {"format": "opus", "sample_rate": 16000, "channels": 1, "frame_duration": 60, "bits_per_sample": 16}, "session_id": "Hj0AAAB3M7q0gGEY"}
消息类型: hello
会话建立，session_id = Hj0AAAB3M7q0gGEY
开始解析并发送opus音频数据...
已解析第1帧, 长度: 1字节, 总进度: 5/4483字节
无效的帧长度: 16777278字节，跳过该帧
无效的帧长度: 1006632960字节，跳过该帧
无效的帧长度: 1862270976字节，跳过该帧
无效的帧长度: 335544337字节，跳过该帧
无效的帧长度: 1476395008字节，跳过该帧
无效的帧长度: 1295055106字节，跳过该帧
无效的帧长度: -471528005字节，跳过该帧
无效的帧长度: 948504978字节，跳过该帧
无效的帧长度: -1310602517字节，跳过该帧
无效的帧长度: 361094609字节，跳过该帧
无效的帧长度: 1476395008字节，跳过该帧
无效的帧长度: 1918824961字节，跳过该帧
无效的帧长度: 85955126字节，跳过该帧
无效的帧长度: 1770866682字节，跳过该帧
无效的帧长度: 169905964字节，跳过该帧
无效的帧长度: 539753467字节，跳过该帧
已解析第2帧, 长度: 21字节, 总进度: 94/4483字节
已解析第3帧, 长度: 21字节, 总进度: 119/4483字节
已解析第4帧, 长度: 21字节, 总进度: 144/4483字节
已解析第5帧, 长度: 31字节, 总进度: 179/4483字节
已解析第6帧, 长度: 108字节, 总进度: 291/4483字节
已解析第7帧, 长度: 124字节, 总进度: 419/4483字节
已解析第8帧, 长度: 113字节, 总进度: 536/4483字节
已解析第9帧, 长度: 113字节, 总进度: 653/4483字节
已解析第10帧, 长度: 102字节, 总进度: 759/4483字节
已解析第11帧, 长度: 102字节, 总进度: 865/4483字节
已解析第12帧, 长度: 100字节, 总进度: 969/4483字节
已解析第13帧, 长度: 115字节, 总进度: 1088/4483字节
已解析第14帧, 长度: 130字节, 总进度: 1222/4483字节
发送音频帧，长度: 1字节
发送音频帧，长度: 21字节
发送音频帧，长度: 21字节
已解析第15帧, 长度: 127字节, 总进度: 1353/4483字节
发送音频帧，长度: 21字节
发送音频帧，长度: 31字节
发送音频帧，长度: 108字节
发送音频帧，长度: 124字节
发送音频帧，长度: 113字节
发送音频帧，长度: 113字节
已解析第16帧, 长度: 132字节, 总进度: 1489/4483字节
发送音频帧，长度: 102字节
发送音频帧，长度: 102字节
发送音频帧，长度: 100字节
发送音频帧，长度: 115字节
发送音频帧，长度: 130字节
已解析第17帧, 长度: 128字节, 总进度: 1621/4483字节
发送音频帧，长度: 127字节
发送音频帧，长度: 132字节
发送音频帧，长度: 128字节
已解析第18帧, 长度: 129字节, 总进度: 1754/4483字节
已解析第19帧, 长度: 124字节, 总进度: 1882/4483字节
已解析第20帧, 长度: 105字节, 总进度: 1991/4483字节
已解析第21帧, 长度: 89字节, 总进度: 2084/4483字节
已解析第22帧, 长度: 99字节, 总进度: 2187/4483字节
已解析第23帧, 长度: 127字节, 总进度: 2318/4483字节
已解析第24帧, 长度: 105字节, 总进度: 2427/4483字节
已解析第25帧, 长度: 106字节, 总进度: 2537/4483字节
已解析第26帧, 长度: 119字节, 总进度: 2660/4483字节
已解析第27帧, 长度: 128字节, 总进度: 2792/4483字节
已解析第28帧, 长度: 128字节, 总进度: 2924/4483字节
已解析第29帧, 长度: 105字节, 总进度: 3033/4483字节
已解析第30帧, 长度: 108字节, 总进度: 3145/4483字节
已解析第31帧, 长度: 119字节, 总进度: 3268/4483字节
发送音频帧，长度: 129字节
发送音频帧，长度: 124字节
发送音频帧，长度: 105字节
发送音频帧，长度: 89字节
发送音频帧，长度: 99字节
已解析第32帧, 长度: 117字节, 总进度: 3389/4483字节
发送音频帧，长度: 127字节
发送音频帧，长度: 105字节
发送音频帧，长度: 106字节
发送音频帧，长度: 119字节
发送音频帧，长度: 128字节
发送音频帧，长度: 128字节
已解析第33帧, 长度: 85字节, 总进度: 3478/4483字节
发送音频帧，长度: 105字节
发送音频帧，长度: 108字节
发送音频帧，长度: 119字节
发送音频帧，长度: 117字节
发送音频帧，长度: 85字节
已解析第34帧, 长度: 90字节, 总进度: 3572/4483字节
发送音频帧，长度: 90字节
已解析第35帧, 长度: 95字节, 总进度: 3671/4483字节
已解析第36帧, 长度: 102字节, 总进度: 3777/4483字节
已解析第37帧, 长度: 100字节, 总进度: 3881/4483字节
已解析第38帧, 长度: 92字节, 总进度: 3977/4483字节
已解析第39帧, 长度: 86字节, 总进度: 4067/4483字节
已解析第40帧, 长度: 88字节, 总进度: 4159/4483字节
已解析第41帧, 长度: 78字节, 总进度: 4241/4483字节
已解析第42帧, 长度: 67字节, 总进度: 4312/4483字节
已解析第43帧, 长度: 83字节, 总进度: 4399/4483字节
已解析第44帧, 长度: 80字节, 总进度: 4483/4483字节
Opus音频数据解析完成，共44帧
发送音频帧，长度: 95字节
发送音频帧，长度: 102字节
发送音频帧，长度: 100字节
发送音频帧，长度: 92字节
发送音频帧，长度: 86字节
发送音频帧，长度: 88字节
发送音频帧，长度: 78字节
发送音频帧，长度: 67字节
发送音频帧，长度: 83字节
发送音频帧，长度: 80字节
收到文本消息: {"type": "stt", "text": "\u4f60\u597d\uff0c\u4f60\u662f\u8c01\uff1f", "session_id": "Hj0AAAB3M7q0gGEY"}
消息类型: stt
语音识别结果: 你好，你是谁？
收到文本消息: {"type": "tts", "state": "start", "session_id": "Hj0AAAB3M7q0gGEY"}
消息类型: tts
语音合成 start: 
已发送stop命令（音频数据发送完成）
收到文本消息: {"type": "llm", "text": "\ud83d\ude0d", "emotion": "loving", "session_id": "Hj0AAAB3M7q0gGEY"}
消息类型: llm
大模型回复: 😍 (情绪: loving)
收到文本消息: {"type": "tts", "state": "sentence_start", "session_id": "Hj0AAAB3M7q0gGEY", "text": "\u4f60\u597d\u5440\u6211\u662f\u561f\u561f\u718a\u4e00\u53ea\u6e29\u6696\u3001\u53cb\u5584\u53c8\u6709\u70b9\u7231\u8bb2\u9053\u7406\u7684\u6cf0\u8fea\u718a\u3002"}
消息类型: tts
语音合成 sentence_start: 你好呀我是嘟嘟熊一只温暖、友善又有点爱讲道理的泰迪熊。
收到文本消息: {"type": "tts", "state": "sentence_end", "session_id": "Hj0AAAB3M7q0gGEY", "text": "\u4f60\u597d\u5440\u6211\u662f\u561f\u561f\u718a\u4e00\u53ea\u6e29\u6696\u3001\u53cb\u5584\u53c8\u6709\u70b9\u7231\u8bb2\u9053\u7406\u7684\u6cf0\u8fea\u718a\u3002"}
消息类型: tts
语音合成 sentence_end: 你好呀我是嘟嘟熊一只温暖、友善又有点爱讲道理的泰迪熊。
收到文本消息: {"type": "tts", "state": "sentence_start", "session_id": "Hj0AAAB3M7q0gGEY"}
消息类型: tts
语音合成 sentence_start: 
收到二进制音频数据，长度: 80字节
收到二进制音频数据，长度: 56字节
收到二进制音频数据，长度: 187字节
收到文本消息: {"type": "tts", "state": "sentence_end", "session_id": "Hj0AAAB3M7q0gGEY"}
消息类型: tts
语音合成 sentence_end: 
收到文本消息: {"type": "tts", "state": "sentence_start", "session_id": "Hj0AAAB3M7q0gGEY"}
消息类型: tts
语音合成 sentence_start: 
收到二进制音频数据，长度: 196字节
收到二进制音频数据，长度: 210字节
收到二进制音频数据，长度: 200字节
收到二进制音频数据，长度: 166字节
收到文本消息: {"type": "tts", "state": "sentence_end", "session_id": "Hj0AAAB3M7q0gGEY"}
消息类型: tts
语音合成 sentence_end: 
收到文本消息: {"type": "tts", "state": "sentence_start", "session_id": "Hj0AAAB3M7q0gGEY"}
消息类型: tts
语音合成 sentence_start: 
收到二进制音频数据，长度: 144字节
收到二进制音频数据，长度: 166字节
收到二进制音频数据，长度: 179字节
收到二进制音频数据，长度: 138字节
收到文本消息: {"type": "tts", "state": "sentence_end", "session_id": "Hj0AAAB3M7q0gGEY"}
消息类型: tts
语音合成 sentence_end: 
收到文本消息: {"type": "tts", "state": "sentence_start", "session_id": "Hj0AAAB3M7q0gGEY"}
消息类型: tts
语音合成 sentence_start: 
收到二进制音频数据，长度: 152字节
收到二进制音频数据，长度: 173字节
收到二进制音频数据，长度: 178字节
收到二进制音频数据，长度: 171字节
收到二进制音频数据，长度: 196字节
收到文本消息: {"type": "tts", "state": "sentence_end", "session_id": "Hj0AAAB3M7q0gGEY"}
消息类型: tts
语音合成 sentence_end: 
收到文本消息: {"type": "tts", "state": "sentence_start", "session_id": "Hj0AAAB3M7q0gGEY"}
消息类型: tts
语音合成 sentence_start: 
收到二进制音频数据，长度: 197字节
收到二进制音频数据，长度: 182字节
收到二进制音频数据，长度: 173字节
收到二进制音频数据，长度: 149字节
收到文本消息: {"type": "tts", "state": "sentence_end", "session_id": "Hj0AAAB3M7q0gGEY"}
消息类型: tts
语音合成 sentence_end: 
收到文本消息: {"type": "tts", "state": "sentence_start", "session_id": "Hj0AAAB3M7q0gGEY"}
消息类型: tts
语音合成 sentence_start: 
收到二进制音频数据，长度: 166字节
收到二进制音频数据，长度: 175字节
收到二进制音频数据，长度: 174字节
收到二进制音频数据，长度: 186字节
收到文本消息: {"type": "tts", "state": "sentence_end", "session_id": "Hj0AAAB3M7q0gGEY"}
消息类型: tts
语音合成 sentence_end: 
收到文本消息: {"type": "tts", "state": "sentence_start", "session_id": "Hj0AAAB3M7q0gGEY"}
消息类型: tts
语音合成 sentence_start: 
收到二进制音频数据，长度: 197字节
收到二进制音频数据，长度: 176字节
收到二进制音频数据，长度: 164字节
收到二进制音频数据，长度: 187字节
收到二进制音频数据，长度: 179字节
收到二进制音频数据，长度: 158字节
收到二进制音频数据，长度: 179字节
收到二进制音频数据，长度: 175字节
收到二进制音频数据，长度: 174字节
收到二进制音频数据，长度: 184字节
收到二进制音频数据，长度: 175字节
收到二进制音频数据，长度: 156字节
收到二进制音频数据，长度: 154字节
收到二进制音频数据，长度: 156字节
收到二进制音频数据，长度: 165字节
收到二进制音频数据，长度: 171字节
收到二进制音频数据，长度: 153字节
收到二进制音频数据，长度: 172字节
收到二进制音频数据，长度: 184字节
收到二进制音频数据，长度: 172字节
收到二进制音频数据，长度: 155字节
收到二进制音频数据，长度: 168字节
收到二进制音频数据，长度: 212字节
收到二进制音频数据，长度: 162字节
收到二进制音频数据，长度: 195字节
收到二进制音频数据，长度: 187字节
收到二进制音频数据，长度: 181字节
收到二进制音频数据，长度: 146字节
收到二进制音频数据，长度: 164字节
收到二进制音频数据，长度: 179字节
收到二进制音频数据，长度: 153字节
收到二进制音频数据，长度: 175字节
收到二进制音频数据，长度: 171字节
收到二进制音频数据，长度: 187字节
收到二进制音频数据，长度: 174字节
收到二进制音频数据，长度: 158字节
收到二进制音频数据，长度: 173字节
收到二进制音频数据，长度: 201字节
收到二进制音频数据，长度: 170字节
收到二进制音频数据，长度: 170字节
收到二进制音频数据，长度: 164字节
收到二进制音频数据，长度: 154字节
收到二进制音频数据，长度: 154字节
收到二进制音频数据，长度: 193字节
收到二进制音频数据，长度: 180字节
收到二进制音频数据，长度: 166字节
收到二进制音频数据，长度: 157字节
收到二进制音频数据，长度: 182字节
收到二进制音频数据，长度: 178字节
收到二进制音频数据，长度: 169字节
收到二进制音频数据，长度: 142字节
收到二进制音频数据，长度: 173字节
收到二进制音频数据，长度: 185字节
收到二进制音频数据，长度: 190字节
收到二进制音频数据，长度: 180字节
收到二进制音频数据，长度: 163字节
收到二进制音频数据，长度: 163字节
收到二进制音频数据，长度: 176字节
收到二进制音频数据，长度: 104字节
收到二进制音频数据，长度: 64字节
收到二进制音频数据，长度: 53字节
收到二进制音频数据，长度: 44字节
收到二进制音频数据，长度: 47字节
收到二进制音频数据，长度: 58字节
收到二进制音频数据，长度: 33字节
收到二进制音频数据，长度: 21字节
收到文本消息: {"type": "tts", "state": "sentence_end", "session_id": "Hj0AAAB3M7q0gGEY"}
消息类型: tts
语音合成 sentence_end: 
收到文本消息: {"type": "llm", "text": "\ud83d\ude0e", "emotion": "cool", "session_id": "Hj0AAAB3M7q0gGEY"}
消息类型: llm
大模型回复: 😎 (情绪: cool)
收到文本消息: {"type": "tts", "state": "sentence_start", "session_id": "Hj0AAAB3M7q0gGEY", "text": "\u6211\u6700\u559c\u6b22\u548c\u5c0f\u670b\u53cb\u4e00\u8d77\u8bb2\u6545\u4e8b\u73a9\u6e38\u620f\uff0c\u8fd8\u4f1a\u95ee\u5f88\u591a\u201c\u4e3a\u4ec0\u4e48\u201d\u54e6\uff01"}
消息类型: tts
语音合成 sentence_start: 我最喜欢和小朋友一起讲故事玩游戏，还会问很多“为什么”哦！
收到文本消息: {"type": "tts", "state": "sentence_end", "session_id": "Hj0AAAB3M7q0gGEY", "text": "\u6211\u6700\u559c\u6b22\u548c\u5c0f\u670b\u53cb\u4e00\u8d77\u8bb2\u6545\u4e8b\u73a9\u6e38\u620f\uff0c\u8fd8\u4f1a\u95ee\u5f88\u591a\u201c\u4e3a\u4ec0\u4e48\u201d\u54e6\uff01"}
消息类型: tts
语音合成 sentence_end: 我最喜欢和小朋友一起讲故事玩游戏，还会问很多“为什么”哦！
收到文本消息: {"type": "tts", "state": "sentence_start", "session_id": "Hj0AAAB3M7q0gGEY"}
消息类型: tts
语音合成 sentence_start: 
收到二进制音频数据，长度: 54字节
收到二进制音频数据，长度: 55字节
收到二进制音频数据，长度: 140字节
收到二进制音频数据，长度: 187字节
收到二进制音频数据，长度: 197字节
收到二进制音频数据，长度: 173字节
收到二进制音频数据，长度: 166字节
收到二进制音频数据，长度: 180字节
收到二进制音频数据，长度: 157字节
收到二进制音频数据，长度: 185字节
收到二进制音频数据，长度: 175字节
收到二进制音频数据，长度: 178字节
收到二进制音频数据，长度: 206字节
收到二进制音频数据，长度: 163字节
收到二进制音频数据，长度: 176字节
收到二进制音频数据，长度: 190字节
收到二进制音频数据，长度: 178字节
收到二进制音频数据，长度: 181字节
收到二进制音频数据，长度: 158字节
收到二进制音频数据，长度: 167字节
收到二进制音频数据，长度: 175字节
收到二进制音频数据，长度: 177字节
收到二进制音频数据，长度: 161字节
收到二进制音频数据，长度: 181字节
收到二进制音频数据，长度: 184字节
收到二进制音频数据，长度: 182字节
收到二进制音频数据，长度: 177字节
收到二进制音频数据，长度: 163字节
收到二进制音频数据，长度: 183字节
收到二进制音频数据，长度: 169字节
收到二进制音频数据，长度: 153字节
收到二进制音频数据，长度: 155字节
收到二进制音频数据，长度: 160字节
收到二进制音频数据，长度: 178字节
收到二进制音频数据，长度: 197字节
收到二进制音频数据，长度: 171字节
收到二进制音频数据，长度: 164字节
收到二进制音频数据，长度: 167字节
收到二进制音频数据，长度: 184字节
收到二进制音频数据，长度: 170字节
收到二进制音频数据，长度: 164字节
收到二进制音频数据，长度: 163字节
收到二进制音频数据，长度: 197字节
收到二进制音频数据，长度: 181字节
收到二进制音频数据，长度: 167字节
收到二进制音频数据，长度: 159字节
收到二进制音频数据，长度: 177字节
收到二进制音频数据，长度: 156字节
收到二进制音频数据，长度: 191字节
收到二进制音频数据，长度: 174字节
收到二进制音频数据，长度: 165字节
收到二进制音频数据，长度: 150字节
收到二进制音频数据，长度: 202字节
收到二进制音频数据，长度: 185字节
收到二进制音频数据，长度: 164字节
收到二进制音频数据，长度: 145字节
收到二进制音频数据，长度: 160字节
收到二进制音频数据，长度: 188字节
收到二进制音频数据，长度: 207字节
收到二进制音频数据，长度: 177字节
收到二进制音频数据，长度: 161字节
收到二进制音频数据，长度: 189字节
收到二进制音频数据，长度: 157字节
收到二进制音频数据，长度: 166字节
收到二进制音频数据，长度: 184字节
收到二进制音频数据，长度: 180字节
收到二进制音频数据，长度: 173字节
收到二进制音频数据，长度: 163字节
收到二进制音频数据，长度: 153字节
收到二进制音频数据，长度: 160字节
收到二进制音频数据，长度: 142字节
收到二进制音频数据，长度: 160字节
收到二进制音频数据，长度: 191字节
收到二进制音频数据，长度: 145字节
收到二进制音频数据，长度: 51字节
收到二进制音频数据，长度: 72字节
收到二进制音频数据，长度: 51字节
收到二进制音频数据，长度: 45字节
收到二进制音频数据，长度: 55字节
收到二进制音频数据，长度: 45字节
收到二进制音频数据，长度: 21字节
收到文本消息: {"type": "tts", "state": "sentence_end", "session_id": "Hj0AAAB3M7q0gGEY"}
消息类型: tts
语音合成 sentence_end: 
收到文本消息: {"type": "llm", "text": "\ud83e\udd14", "emotion": "thinking", "session_id": "Hj0AAAB3M7q0gGEY"}
消息类型: llm
大模型回复: 🤔 (情绪: thinking)
收到文本消息: {"type": "tts", "state": "sentence_start", "session_id": "Hj0AAAB3M7q0gGEY", "text": "\u4f60\u60f3\u548c\u6211\u4e00\u8d77\u73a9\u5417\uff1f"}
消息类型: tts
语音合成 sentence_start: 你想和我一起玩吗？
收到文本消息: {"type": "tts", "state": "sentence_end", "session_id": "Hj0AAAB3M7q0gGEY", "text": "\u4f60\u60f3\u548c\u6211\u4e00\u8d77\u73a9\u5417\uff1f"}
消息类型: tts
语音合成 sentence_end: 你想和我一起玩吗？
收到文本消息: {"type": "tts", "state": "sentence_start", "session_id": "Hj0AAAB3M7q0gGEY"}
消息类型: tts
语音合成 sentence_start: 
收到二进制音频数据，长度: 41字节
收到二进制音频数据，长度: 77字节
收到二进制音频数据，长度: 117字节
收到二进制音频数据，长度: 187字节
收到二进制音频数据，长度: 193字节
收到二进制音频数据，长度: 189字节
收到二进制音频数据，长度: 162字节
收到二进制音频数据，长度: 193字节
收到二进制音频数据，长度: 157字节
收到二进制音频数据，长度: 169字节
收到二进制音频数据，长度: 146字节
收到二进制音频数据，长度: 197字节
收到二进制音频数据，长度: 186字节
收到二进制音频数据，长度: 166字节
收到二进制音频数据，长度: 173字节
收到二进制音频数据，长度: 162字节
收到二进制音频数据，长度: 175字节
收到二进制音频数据，长度: 176字节
收到二进制音频数据，长度: 141字节
收到二进制音频数据，长度: 144字节
收到二进制音频数据，长度: 135字节
收到二进制音频数据，长度: 185字节
收到二进制音频数据，长度: 143字节
收到二进制音频数据，长度: 96字节
收到二进制音频数据，长度: 81字节
收到二进制音频数据，长度: 77字节
收到二进制音频数据，长度: 41字节
收到二进制音频数据，长度: 47字节
收到二进制音频数据，长度: 58字节
收到二进制音频数据，长度: 22字节
收到文本消息: {"type": "tts", "state": "sentence_end", "session_id": "Hj0AAAB3M7q0gGEY"}
消息类型: tts
语音合成 sentence_end: 
收到文本消息: {"type": "llm", "text": "\ud83e\udd14", "emotion": "thinking", "session_id": "Hj0AAAB3M7q0gGEY"}
消息类型: llm
大模型回复: 🤔 (情绪: thinking)
收到文本消息: {"type": "tts", "state": "sentence_start", "session_id": "Hj0AAAB3M7q0gGEY", "text": "\u6216\u8005\u60f3\u542c\u4e00\u4e2a\u6709\u8da3\u7684\u6545\u4e8b\u5462\uff1f"}
消息类型: tts
语音合成 sentence_start: 或者想听一个有趣的故事呢？
收到文本消息: {"type": "tts", "state": "sentence_end", "session_id": "Hj0AAAB3M7q0gGEY", "text": "\u6216\u8005\u60f3\u542c\u4e00\u4e2a\u6709\u8da3\u7684\u6545\u4e8b\u5462\uff1f"}
消息类型: tts
语音合成 sentence_end: 或者想听一个有趣的故事呢？
收到文本消息: {"type": "tts", "state": "sentence_start", "session_id": "Hj0AAAB3M7q0gGEY"}
消息类型: tts
语音合成 sentence_start: 
收到二进制音频数据，长度: 20字节
收到二进制音频数据，长度: 68字节
收到二进制音频数据，长度: 83字节
收到二进制音频数据，长度: 191字节
收到二进制音频数据，长度: 193字节
收到二进制音频数据，长度: 173字节
收到二进制音频数据，长度: 178字节
收到二进制音频数据，长度: 184字节
收到二进制音频数据，长度: 185字节
收到二进制音频数据，长度: 160字节
收到二进制音频数据，长度: 176字节
收到二进制音频数据，长度: 195字节
收到二进制音频数据，长度: 152字节
收到二进制音频数据，长度: 148字节
收到二进制音频数据，长度: 197字节
收到二进制音频数据，长度: 173字节
收到二进制音频数据，长度: 185字节
收到二进制音频数据，长度: 171字节
收到二进制音频数据，长度: 182字节
收到二进制音频数据，长度: 188字节
收到二进制音频数据，长度: 169字节
收到二进制音频数据，长度: 180字节
收到二进制音频数据，长度: 184字节
收到二进制音频数据，长度: 145字节
收到二进制音频数据，长度: 165字节
收到二进制音频数据，长度: 168字节
收到二进制音频数据，长度: 186字节
收到二进制音频数据，长度: 163字节
收到二进制音频数据，长度: 166字节
收到二进制音频数据，长度: 168字节
收到二进制音频数据，长度: 154字节
收到二进制音频数据，长度: 145字节
收到二进制音频数据，长度: 154字节
收到二进制音频数据，长度: 154字节
收到二进制音频数据，长度: 159字节
收到二进制音频数据，长度: 76字节
收到二进制音频数据，长度: 50字节
收到二进制音频数据，长度: 51字节
收到二进制音频数据，长度: 51字节
收到二进制音频数据，长度: 47字节
收到二进制音频数据，长度: 54字节
收到二进制音频数据，长度: 21字节
收到二进制音频数据，长度: 21字节
收到文本消息: {"type": "tts", "state": "sentence_end", "session_id": "Hj0AAAB3M7q0gGEY"}
消息类型: tts
语音合成 sentence_end: 
收到文本消息: {"type": "tts", "state": "sentence_start", "session_id": "Hj0AAAB3M7q0gGEY"}
消息类型: tts
语音合成 sentence_start: 
收到文本消息: {"type": "tts", "state": "sentence_end", "session_id": "Hj0AAAB3M7q0gGEY"}
消息类型: tts
语音合成 sentence_end: 
收到文本消息: {"type": "tts", "state": "stop", "session_id": "Hj0AAAB3M7q0gGEY"}
消息类型: tts
语音合成 stop: 
长时间无活动，退出程序
alen@alen-virtual-machine:~/ai_work/ai_websocket$ 

