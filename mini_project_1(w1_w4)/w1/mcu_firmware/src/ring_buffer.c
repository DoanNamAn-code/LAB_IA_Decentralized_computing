#include "ring_buffer.h"

void init(ring_buffer *rb){
    rb->head = 0;
    rb->count = 0;
    rb->tail = 0;
    rb->mutex = xSemaphoreCreateMutex();
}
bool rb_push(ring_buffer *rb, camera_fb_t *item){
    if(item == NULL) return false;
    if (rb->mutex == NULL || rb == NULL){
        esp_camera_fb_return(item);
        return false;
    }
    if (xSemaphoreTake(rb->mutex, pdMS_TO_TICKS(10)) != pdTRUE){
        esp_camera_fb_return(item);
        return false;
    }
    if (rb->count >= BUFFER_CAPACITY) {
        xSemaphoreGive(rb->mutex);
        esp_camera_fb_return(item);
        return false;
    }
    rb->data[rb->head] = item;
    rb->head = (rb->head+1) % BUFFER_CAPACITY;
    rb->count++;
    xSemaphoreGive(rb->mutex);
    return true;
}
bool rb_push_over(ring_buffer *rb, camera_fb_t *item){
    if(item == NULL) return false;
    if (rb->mutex == NULL || rb == NULL){
        esp_camera_fb_return(item);
        return false;
    }
    if (xSemaphoreTake(rb->mutex, pdMS_TO_TICKS(10)) != pdTRUE){
        esp_camera_fb_return(item);
        return false;
    }
    if (rb->count == BUFFER_CAPACITY){
        esp_camera_fb_return(rb->data[rb->head]);
        rb->data[rb->head] = item;
        rb->head = (rb->head+1) % BUFFER_CAPACITY;
        rb->tail = (rb->tail+1) % BUFFER_CAPACITY;
    } else {
        rb->data[rb->head] = item;
        rb->head = (rb->head+1) % BUFFER_CAPACITY;
        rb->count++;
    }
    xSemaphoreGive(rb->mutex);
    return true;

}
bool rb_pop(ring_buffer *rb, camera_fb_t **item){
    if (xSemaphoreTake(rb->mutex, pdMS_TO_TICKS(10)) != pdTRUE){
        return false;
    }
    if (rb->count == 0) {
        xSemaphoreGive(rb->mutex);
        return false;
    }
    *item = rb->data[rb->tail];
    rb->tail = (rb->tail+1) % BUFFER_CAPACITY;
    rb->count--;
    xSemaphoreGive(rb->mutex);
    return true;
}
