#include "connection.h"

static const char *NEW_TAG = "HTTP_CLIENT";
//Đóng gói struct thành json
bool send_json_post(const camera_fb_t *item){
    esp_http_client_config_t config = {
        .url = SERVER_URL,
        .method = HTTP_METHOD_POST,
        .timeout_ms = 3000,
    };
    esp_http_client_handle_t client = esp_http_client_init(&config);
    if (client == NULL){
        ESP_LOGE(NEW_TAG, "Không thể khởi tạo http client");
        return false;
    }
    esp_http_client_set_header(client, "Content-Type", "application/octet-stream");
    esp_http_client_set_post_field(client, (const char*) item->buf, item->len);
    esp_err_t err = esp_http_client_perform(client);
    bool is_sent = false;
    if (err == ESP_OK){
        int status_code = esp_http_client_get_status_code(client);
        if (status_code == 200 || status_code == 201){
            ESP_LOGI(NEW_TAG, "POST thành công [%d]", status_code);
            is_sent = true;
    } else {
        ESP_LOGW(NEW_TAG, "Server từ chối dữ liệu, mã HTTP: %d", status_code);
    }
    } else {
        ESP_LOGE(NEW_TAG, "Lỗi kết nối HTTP: %s", esp_err_to_name(err));
    }
    esp_http_client_cleanup(client);
    return is_sent;
}
void http_client_task(void* pvParameters){
    ring_buffer* sensor_buffer = (ring_buffer*) pvParameters;
    bool pending_item = false;
    camera_fb_t *item;
    xEventGroupWaitBits(wifi_event_group, WIFI_CONNECTED_BIT, pdFALSE, pdTRUE, portMAX_DELAY);
    while(1){
        if(!pending_item){
            if (rb_pop(sensor_buffer, &item)){
                pending_item = true;
            } else {
                vTaskDelay(pdMS_TO_TICKS(500));
            }
        } else {
            if (send_json_post(item)){
                esp_camera_fb_return(item);
                item = NULL;
                pending_item = false;
            } else {
                vTaskDelay(pdMS_TO_TICKS(2000));
            }
        }
}
}