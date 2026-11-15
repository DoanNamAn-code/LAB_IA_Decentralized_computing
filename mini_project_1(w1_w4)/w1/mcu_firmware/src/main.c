
#include "ring_buffer.h"
#include "connection.h"
#include "nvs_flash.h"
static const char *TAG = "ESP32_TCP";
static void get_sample_data(void *pvParameters){
    data* params = (data*) pvParameters;
    ring_buffer sensor_buffer = params->sensor_buffer;
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


void app_main(void){
    static data my_data;
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND){
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    init(&my_data.sensor_buffer);
    ESP_ERROR_CHECK(ret);
    wifi_init_sta();
    xTaskCreate(get_sample_data, "get_sample_data", 3072, NULL, 5, NULL);
    xTaskCreate(tcp_client_task, "tcp_client_task", 8192, NULL, 5, NULL);
    
}