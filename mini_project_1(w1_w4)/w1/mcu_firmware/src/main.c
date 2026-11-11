#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_timer.h"
#include "ring_buffer.h"


#include "esp_wifi.h"
#include "nvs_flash.h"
#include "esp_event.h"
#include "esp_log.h"
#include "lwip/sockets.h"
#include "lwip/netdb.h"



static const char *TAG = "ESP32_TCP";
static EventGroupHandle_t wifi_event_group;
const int WIFI_CONNECTED_BIT = BIT0;

#define WIFI_SSID "DANG KHOA"
#define WIFI_PASS "1234"
#define SERVER_IP "12213"
#define SERVERPORT 5000

//xử lý sự kiện wifi
static void wifi_event_handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data){
    if(event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START){
        esp_wifi_connect();
    } else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED){
        ESP_LOGW(TAG, "Mất kết nối Wi-fi, đan thử kết nối lại...");
        esp_wifi_connect();
        xEventGroupClearBits(wifi_event_group, WIFI_CONNECTED_BIT);
    } else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP){
        ip_event_got_ip_t* event  = (ip_event_got_ip_t*) event_data;
        ESP_LOGI(TAG, "Đã nhận IP:" IPSTR, IP2STR(&event->ip_info.ip));
        xEventGroupSetBits(wifi_event_group, WIFI_CONNECTED_BIT);
    } 
}
//Khởi tạo wifi station 
static void wifi_init_sta(void){
    wifi_event_group = xEventGroupCreate();
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    esp_netif_create_default_wifi_sta();

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
    esp_event_handler_instance_t instance_any_id;
    esp_event_handler_instance_t instance_got_ip;
    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT, 
                                                        ESP_EVENT_ANY_ID, 
                                                        &wifi_event_handler, 
                                                        NULL, 
                                                        &instance_any_id));
    ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT, 
                                                        IP_EVENT_STA_GOT_IP, 
                                                        &wifi_event_handler, 
                                                        NULL, 
                                                        &instance_got_ip));
    wifi_config_t wifi_config = {
        .sta = {
            .ssid = WIFI_SSID,
            .password = WIFI_PASS,
        },
    };
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());
}


ring_buffer sensor_buffer;
static void get_sample_data(void *pvParameters){
    int id = 0;
    while(1){
    SensorData sample;
    sample.sensor_id = id;
    float temp = 28.0f + ((float)(rand() % 40) / 10.0f);
    if (rand() % 10 == 0){
        temp+= 15.0f;
    }
    sample.temperature = temp;
    sample.timestamp = esp_timer_get_time() / 1000;
    if (rb_push(&sensor_buffer, &sample)){
        ESP_LOGD(TAG, "push buffer: ID: %d | Temp: %.2f", sample.sensor_id, sample.temperature);
    } else {
        ESP_LOGW(TAG, "Ring buffer đã đầy");
    }
    vTaskDelay(pdMS_TO_TICKS(500));
    }
}

//Kết nối TCP
static void tcp_client_task(void *pvParameters){
    xEventGroupWaitBits(wifi_event_group, WIFI_CONNECTED_BIT, pdFALSE, pdTRUE, portMAX_DELAY);

    struct sockaddr_in dest_addr;
    dest_addr.sin_addr.s_addr = inet_addr(SERVER_IP);
    dest_addr.sin_family = AF_INET;
    dest_addr.sin_port = htons(SERVERPORT);

    while(1){
        int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
        if (sock < 0){
            ESP_LOGE(TAG, "Lỗi tạo socket: %d", errno);
            vTaskDelay(pdMS_TO_TICKS(500));
            continue;
        }
        ESP_LOGI(TAG, "ĐANG KẾT NỐI TỚI SBC: %s:%d...", SERVER_IP, SERVERPORT);
        if(connect(sock, (struct sockaddr *) &dest_addr, sizeof(dest_addr)) != 0 ){
            ESP_LOGW(TAG, "Kết nối thất bại");
            close(sock);
            vTaskDelay(pdMS_TO_TICKS(2000));
            continue;
        }
        ESP_LOGI(TAG, "Đã kết nối thành công");
        while(1){
            SensorData item;
            if (rb_peek(&sensor_buffer, &item)){
                int err = send(sock, (const char*) &item, sizeof(SensorData), 0);
                if (err < 0){
                    ESP_LOGE(TAG, "Lỗi gửi");
                    break;
                }
                rb_pop(&sensor_buffer, &item);
                ESP_LOGI(TAG, "Đã gửi thành công");
            } else {
            vTaskDelay(pdMS_TO_TICKS(100));
        }
        }
        close(sock);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
void app_main(void){

    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND){
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);
    init(&sensor_buffer);

    wifi_init_sta();
    xTaskCreate(get_sample_data, "get_sample_data", 3072, NULL, 5, NULL);
    xTaskCreate(tcp_client_task, "tcp_client_task", 8192, NULL, 5, NULL);
    
}