#include "data.h"
static const char *TAG = "RING_PUSH";
// void get_sample_data(void *pvParameters){
//     ring_buffer *sensor_buffer = (ring_buffer *)pvParameters;
//     while (1) {
//         SensorData sample;
//         strcpy(sample.device_id, "ESP32_01");
//         sample.temperature = 27.0f + (esp_random() % 50) / 10.0f;
//         sample.humidity    = 60.0f + (esp_random() % 100) / 10.0f;
//         sample.timestamp_ms = (uint32_t)(esp_timer_get_time() / 1000);
//         rb_push_over(sensor_buffer, &sample);
//         ESP_LOGI(TAG, "Đã push dữ liệu thành công");
//         vTaskDelay(pdMS_TO_TICKS(2000)); 
//     }
//}
void get_camera_data(void *pvParameters){
    ring_buffer *sensor_buffer = (ring_buffer*) pvParameters;
    camera_fb_t *fb = esp_camera_fb_get();
    while(1){
        if(rb_push(sensor_buffer, fb)){
            ESP_LOGI(TAG, "Push dữ liệu vào ring thành công");
        } else {
            vTaskDelay(pdMS_TO_TICKS(200));
        }
    }
}
