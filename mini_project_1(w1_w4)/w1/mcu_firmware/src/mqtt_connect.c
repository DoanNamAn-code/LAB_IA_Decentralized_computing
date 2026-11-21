#include "connection.h"
static const char *TAG = "MQTT_CLIENT";

static esp_mqtt_client_handle_t mqtt_client = NULL;
static EventGroupHandle_t mqtt_event_group = NULL;
#define MQTT_CONNECTED_BIT BIT0

static void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data) {
    esp_mqtt_event_handle_t event = (esp_mqtt_event_handle_t)event_data;

    switch ((esp_mqtt_event_id_t)event_id) {
        case MQTT_EVENT_CONNECTED:
            ESP_LOGI(TAG, "Da ket noi toi MQTT Broker!");
            xEventGroupSetBits(mqtt_event_group, MQTT_CONNECTED_BIT);

            // Báo trạng thái online (Retain = 1) và đăng ký kênh nhận lệnh từ Server
            esp_mqtt_client_publish(mqtt_client, TOPIC_CAM_STATUS, "online", 0, 1, 1);
            esp_mqtt_client_subscribe(mqtt_client, TOPIC_CAM_CMD, 1);
            break;

        case MQTT_EVENT_DISCONNECTED:
            ESP_LOGW(TAG, "Mat ket noi MQTT Broker!");
            xEventGroupClearBits(mqtt_event_group, MQTT_CONNECTED_BIT);
            break;

        case MQTT_EVENT_DATA:
            // Nhận lệnh điều khiển từ Server đẩy xuống
            ESP_LOGI(TAG, "Nhan lenh [%.*s]: %.*s",
                     event->topic_len, event->topic,
                     event->data_len, event->data);
            break;

        case MQTT_EVENT_ERROR:
            ESP_LOGE(TAG, "Loi giao thuc/ket noi MQTT!");
            break;

        default:
            break;
    }
}

void mqtt_app_start(void) {
    mqtt_event_group = xEventGroupCreate();

    // Chờ có Wi-Fi và nhận IP xong mới khởi động MQTT Client
    xEventGroupWaitBits(wifi_event_group, WIFI_CONNECTED_BIT, pdFALSE, pdTRUE, portMAX_DELAY);

    esp_mqtt_client_config_t mqtt_cfg = {
        .broker.address.uri = MQTT_BROKER_URI,
        .credentials.client_id = "esp32_cam_01",
        .session.keepalive = 15,
        .session.last_will = {
            .topic = TOPIC_CAM_STATUS,
            .msg = "offline",
            .msg_len = 7,
            .qos = 1,
            .retain = 1,
        },
        .buffer.size = 1024,
        .buffer.out_size = 4096, // Tăng bộ đệm xuất Socket để đẩy ảnh lớn nhanh hơn
    };

    mqtt_client = esp_mqtt_client_init(&mqtt_cfg);
    esp_mqtt_client_register_event(mqtt_client, ESP_EVENT_ANY_ID, mqtt_event_handler, NULL);
    esp_mqtt_client_start(mqtt_client);
}

void mqtt_publish_task(void *pvParameters) {
    ring_buffer *cam_buffer = (ring_buffer *)pvParameters;
    camera_fb_t *fb = NULL;

    while (1) {
        // 1. Chỉ chạy khi đồng thời có Wi-Fi và đã bắt tay xong với Broker
        xEventGroupWaitBits(wifi_event_group, WIFI_CONNECTED_BIT, pdFALSE, pdTRUE, portMAX_DELAY);
        xEventGroupWaitBits(mqtt_event_group, MQTT_CONNECTED_BIT, pdFALSE, pdTRUE, portMAX_DELAY);

        // 2. Rút con trỏ khung hình từ ring_buffer
        if (rb_pop(cam_buffer, &fb)) {
            // Dùng QoS = 0 để bắn trực tiếp từ fb->buf mà không nhân bản 150 KB vào RAM outbox
            int msg_id = esp_mqtt_client_publish(
                mqtt_client,
                TOPIC_CAM_IMAGE,
                (const char *)fb->buf,
                fb->len,
                0, // QoS 0
                0  // Retain = false
            );

            if (msg_id >= 0) {
                ESP_LOGI(TAG, "Publish anh thanh cong (%zu bytes)", fb->len);
            } else {
                ESP_LOGE(TAG, "Publish anh that bai (ma loi: %d)", msg_id);
            }

            // 3. Bắt buộc trả buffer về cho driver camera tái sử dụng
            esp_camera_fb_return(fb);
            fb = NULL;
        } else {
            vTaskDelay(pdMS_TO_TICKS(50));
        }
    }
}