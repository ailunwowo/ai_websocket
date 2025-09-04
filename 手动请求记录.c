http post请求发送

[2025-09-04 09:44:25.176]# RECV ASCII/908 from 192.168.20.93 :38062 <<<
POST / HTTP/1.1
Host: 192.168.20.90
Accept: */*
Content-Type: application/json
Device-Id: D4:06:06:B6:A9:FB
Client-Id: webai_test
User-Agent: esp-box-3/1.5.6
Accept-Language: zh-CN
Activation-Version: 2
Content-Length: 673

{"application":{"name":"xiaoniu-web-test","version":"1.0.0","compile_time":"2025-04-16 10:00:00","idf_version":"4.4.3","elf_sha256":"1234567890abcdef1234567890abcdef1234567890abcdef"},"ota":{"label":"xiaoling-web-test"},"board":{"type":"xiaoling-web-test","name":"xiaoniu-web-test","ssid":"xiaoling-web-test","rssi":-50,"channel":1,"ip":"192.168.1.1","mac":"D4:06:06:B6:A9:FB"},"mac_address":"D4:06:06:B6:A9:FB","uuid":"webai_test","version":0,"chip_model_name":"esp32s3","flash_size":8388608,"minimum_free_heap_size":32768,"chip_info":{"model":0,"cores":2,"revision":0,"features":0},"partition_table":[{"label":"ota","type":0,"subtype":16,"address":65536,"size":4194304}]}

http get请求建立websocket连接

[2025-09-04 09:48:50.638]# RECV ASCII/537 from 192.168.20.93 :44202 <<<
GET / HTTP/1.1
Sec-WebSocket-Version: 13
Sec-WebSocket-Key: sXPUeP7S+l+KHO8g4/BBWw==
Connection: Upgrade
Upgrade: websocket
Authorization:  Bearer eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJkYXRhIjoiaTFzMlRmWFJNczk4N2RkakxublZIUFd5WW9iT3hxVEo2TVdndEVKbVpqY2NZb09DMkJNS29lRkVwcVEzR21zREVydVQ3U0RTN19fODBvWm5HRG41aFU3dkJmQ1F4aXBqampoYldMd18ifQ.rM2g84RswczPN38KYC2BAtQPfF1oyhjequtska3Mkz8
Device-Id:  00:0c:29:34:45:0f
Client-Id:  webai_test
Sec-WebSocket-Extensions: permessage-deflate; client_max_window_bits
Host: 192.168.20.90



websocket的交互是以http请求为通信方式的。所有的请求都是由客户端主动发起