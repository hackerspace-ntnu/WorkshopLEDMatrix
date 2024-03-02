#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "lwip/err.h"
#include "lwip/sys.h"
#include "lwip/sockets.h"

#define WIFI_SSID "SSID"
#define WIFI_PASS "PASSWORD"

static const char *TAG = "esp32_client";

static void wifi_init_sta() {
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    esp_wifi_init(&cfg);

    wifi_config_t sta_config = {
        .sta = {
            .ssid = WIFI_SSID,
            .password = WIFI_PASS,
        },
    };
    esp_wifi_set_config(WIFI_IF_STA, &sta_config);

    esp_wifi_start();
}

void send_data_to_server(const char *data) {
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("IP_ADDRESS");  // Replace with the actual IP address of your server
    server_addr.sin_port = htons(12345);

    int client_sock = socket(AF_INET, SOCK_STREAM, 0);
    connect(client_sock, (struct sockaddr*)&server_addr, sizeof(server_addr));

    send(client_sock, data, strlen(data), 0);
    printf("Sent data to server: %s\n", data);

    close(client_sock);
}

void app_main() {
    ESP_ERROR_CHECK(nvs_flash_init());
    esp_netif_init();
    esp_event_loop_create_default();

    wifi_init_sta();

    const char *data_to_send = "Hello, server!";
    send_data_to_server(data_to_send);
}
