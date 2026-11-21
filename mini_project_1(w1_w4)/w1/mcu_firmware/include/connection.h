#ifndef CONNECTION_H
#define CONNECTION_H

#include "ring_buffer.h"
#include "base_lib.h"

//TCP-only
#define SERVER_IP "192.168.2.11"
#define SERVERPORT 5000
//HTTP
#define SERVER_URL "ancsad"
//MQTT
#define MQTT_BROKER_URI   "mqtt://192.168.1.10:1883"  
#define TOPIC_CAM_IMAGE   "lab/cam/image"
#define TOPIC_CAM_STATUS  "lab/cam/status"
#define TOPIC_CAM_CMD     "lab/cam/cmd"

//Wifi
extern EventGroupHandle_t wifi_event_group;
#define WIFI_CONNECTED_BIT BIT0
#define WIFI_SSID "DANG KHOA"
#define WIFI_PASS "123quenroi"

void tcp_client_task(void *pvParameters);
void wifi_init_sta(void);
void http_client_task(void* pvParameters);


#endif