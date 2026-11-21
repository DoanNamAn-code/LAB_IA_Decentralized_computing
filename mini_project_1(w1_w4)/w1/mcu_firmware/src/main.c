
#include "ring_buffer.h"
#include "connection.h"
#include "data.h"
#include "camera_config.h"
static const char *TAG = "ESP32_TCP";
void app_main(void){
    static ring_buffer shared_buffer;
    init(&shared_buffer);
    wifi_event_group = xEventGroupCreate();
    wifi_init_sta();
    if (camera_init() != ESP_OK){
        return;
    }
    ESP_LOGI("RAM", "Tong Heap con trong: %lu bytes", esp_get_free_heap_size());
    ESP_LOGI("RAM", "Heap ho tro DMA con trong: %lu bytes", heap_caps_get_free_size(MALLOC_CAP_DMA));
    xTaskCreate(get_camera_data, "get_camera_data", 8192, &shared_buffer, 5, NULL);
    xTaskCreate(http_client_task, "tcp_client_task", 8192, &shared_buffer, 5, NULL);
    
}