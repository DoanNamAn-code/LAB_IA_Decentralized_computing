#include "data.h"
static const char *TAG = "ESP32_TCP";
void get_sample_data(void *pvParameters){
    ring_buffer *sensor_buffer = (ring_buffer *)pvParameters;
    while (1) {
        SensorData sample;
        strcpy(sample.device_id, "ESP32_01");
        sample.temperature = 27.0f + (esp_random() % 50) / 10.0f;
        sample.humidity    = 60.0f + (esp_random() % 100) / 10.0f;
        sample.timestamp_ms = (uint32_t)(esp_timer_get_time() / 1000);
        rb_push_over(sensor_buffer, &sample);
        ESP_LOGI(TAG, "Đã push dữ liệu thành công");
        vTaskDelay(pdMS_TO_TICKS(2000)); 
    }
}
