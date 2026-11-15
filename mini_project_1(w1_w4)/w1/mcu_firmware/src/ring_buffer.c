#include "ring_buffer.h"

void init(ring_buffer *rb){
    rb->head = 0;
    rb->count = 0;
    rb->tail = 0;
    rb->mutex = xSemaphoreCreateMutex();
}
bool rb_push(ring_buffer *rb, SensorData *item){
    if (xSemaphoreTake(rb->mutex, pdMS_TO_TICKS(10)) != pdTRUE){
        return false;
    }
    if (rb->count >= BUFFER_CAPACITY) {
        xSemaphoreGive(rb->mutex);
        return false;
    }
    rb->data[rb->head] = *item;
    rb->head = (rb->head+1) % BUFFER_CAPACITY;
    rb->count++;
    return true;
}
bool rb_push_over(ring_buffer *rb, SensorData *item){
    if (xSemaphoreTake(rb->mutex, pdMS_TO_TICKS(10)) != pdTRUE){
        return false;
    }
    rb->data[rb->head] = *item;
    rb->head = (rb->head+1) % BUFFER_CAPACITY;
    if (rb->count < BUFFER_CAPACITY){
        rb->count++;
    } else {
        rb->tail = (rb->tail+1) % BUFFER_CAPACITY;
    }
    return true;

}
bool rb_pop(ring_buffer *rb, SensorData *item){
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
    return true;
}
bool rb_peek(ring_buffer *rb, SensorData *item) {
    if (xSemaphoreTake(rb->mutex, pdMS_TO_TICKS(10)) != pdTRUE){
        return false;
    }
    if (rb->count == 0) {
        xSemaphoreGive(rb->mutex);
        return false; 
    }
    *item = rb->data[rb->tail]; 
    return true;
}