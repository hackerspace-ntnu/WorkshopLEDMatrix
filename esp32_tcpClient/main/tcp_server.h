#ifndef TCP_SERVER_H
#define TCP_SERVER_H


#define WIFI_SSID "SSID"
#define WIFI_PASS "PASSWORD"
#define EXAMPLE_ESP_MAXIMUM_RETRY  10

/* The event group allows multiple bits for each event, but we only care about two events:
 * - we are connected to the AP with an IP
 * - we failed to connect after the maximum amount of retries */
#define WIFI_CONNECTED_BIT BIT0
#define WIFI_FAIL_BIT      BIT1

void wifi_init_sta();
void start_server();

#endif


