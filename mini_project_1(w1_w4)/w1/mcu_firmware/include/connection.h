#ifndef CONNECTION_H
#define CONNECTION_H

#include "freertos/FreeRTOS.h"
#include "esp_wifi.h"
#define SERVER_IP "192.168.2.11"
#define SERVERPORT 5000
#define SERVER_URL "ancsad"
#include "ring_buffer.h"
#include "lwip/sockets.h"
#include "esp_log.h"
#include "esp_timer.h"
#include "nvs_flash.h"

extern EventGroupHandle_t wifi_event_group;
#define WIFI_CONNECTED_BIT BIT0


#define WIFI_SSID "DANG KHOA"
#define WIFI_PASS "123quenroi"

void tcp_client_task(void *pvParameters);
void wifi_init_sta(void);


#endif