
#include "ring_buffer.h"
#include "connection.h"
#include "data.h"
static const char *TAG = "ESP32_TCP";
void app_main(void){
    static ring_buffer shared_buffer;
    init(&shared_buffer);
    wifi_event_group = xEventGroupCreate();
    wifi_init_sta();
    xTaskCreate(get_sample_data, "get_sample_data", 4096, &shared_buffer, 5, NULL);
    xTaskCreate(tcp_client_task, "tcp_client_task", 4096, &shared_buffer, 5, NULL);
    
}