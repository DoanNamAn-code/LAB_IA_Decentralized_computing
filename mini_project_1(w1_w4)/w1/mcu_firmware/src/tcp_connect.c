//Kết nối TCP
#include "connection.h"
static const char *TAG = "ESP32_TCP";


//Chuyển tin bằng tcp thuần
void tcp_client_task(void *pvParameters){
    ring_buffer *sensor_buffer = (ring_buffer *)pvParameters;
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
        SensorData item;
        bool pending_item =  false;
        int count =  0;
        char tx_buffer[160];
        while(1){
            if (!pending_item){
                if(rb_pop(sensor_buffer, &item)){
                    pending_item = true;
                } else {
                    vTaskDelay(pdMS_TO_TICKS(1000));
                    continue;
                }
            } else {
            int len = snprintf(tx_buffer, sizeof(tx_buffer),
                               "{\"device_id\":\"%s\",\"temperature\":%.2f,\"humidity\":%.2f,\"timestamp_ms\":%lu}\n",
                               item.device_id,
                               item.temperature,
                               item.humidity,
                               (unsigned long)item.timestamp_ms);
            if (send(sock, tx_buffer, len, 0)){
                pending_item = false;
            } else {
                vTaskDelay(pdMS_TO_TICKS(1000));
                count+=1;
            }
            if (count == 3) break;
        }
    }
    close(sock);
    vTaskDelay(500);
}
}