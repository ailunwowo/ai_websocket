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


