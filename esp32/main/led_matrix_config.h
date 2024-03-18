#ifndef LED_MATRIX_CONFIG_H
#define LED_MATRIX_CONFIG_H

#define LED_STRIP_USE_SPI 1
#define LED_STRIP_USE_RMT 0 
#define LED_STRIP_GPIO 13 
#define LED_STRIP_LED_NUMBER 40
#define LED_STRIP_RMT_RES_HZ 10000000 // 10MHz resolution, 1 tick = 0.1us (led strip needs a high resolution)
#define WIFI_SSID "myssid"
#define WIFI_PASSWORD "mypassword"
#define WIFI_MAXIMUM_RETRY 5 // avoid station reconnecting to the AP unlimited when the AP is really inexistent.
#define WIFI_IPV4_ADDR "000.000.0.000" // esp_client will connect to this address.
#define WIFI_IPV4_PORT 3333
#define TCP_KEEPALIVE_IDLE 5 // Keep-alive idle time. In idle time without receiving any data from peer, will send keep-alive probe packet
#define TCP_KEEPALIVE_INTERVAL 5 // Keep-alive probe packet interval time.
#define TCP_KEEPALIVE_COUNT 3 // Keep-alive probe packet retry count.
#define WIFI_CONNECTED_BIT          0x00000001
#define WIFI_FAIL_BIT               0x00000002

#endif