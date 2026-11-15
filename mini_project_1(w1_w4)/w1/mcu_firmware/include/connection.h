#ifndef CONNECTION
#define CONNECTION

#include "freertos/FreeRTOS.h"
#include "esp_wifi.h"
#define SERVER_IP "12213"
#define SERVERPORT 5000
#define SERVER_URL "ancsad"
#include "ring_buffer.h"
#include "lwip/sockets.h"
#include "esp_log.h"
#include "esp_timer.h"

extern EventGroupHandle_t wifi_event_group;
#define WIFI_CONNECTED_BIT BIT0


#define WIFI_SSID "DANG KHOA"
#define WIFI_PASS "1234"
typedef struct {
    ring_buffer sensor_buffer;
} data;

void tcp_client_task(void *pvParameters);
void wifi_init_sta(void);


#endif