#include "ring_buffer.h"

void init(ring_buffer *rb){
    rb->head = 0;
    rb->count = 0;
    rb->tail = 0;
}
bool rb_push(ring_buffer *rb, SensorData *item){
    if (rb->count >= BUFFER_CAPACITY) return false;
    rb->data[rb->head] = *item;
    rb->head = (rb->head+1) % BUFFER_CAPACITY;
    rb->count++;
    return true;
}
void rb_push_over(ring_buffer *rb, SensorData *item){
    rb->data[rb->head] = *item;
    rb->head = (rb->head+1) % BUFFER_CAPACITY;
    if (rb->count < BUFFER_CAPACITY){
        rb->count++;
    } else {
        rb->tail = (rb->tail+1) % BUFFER_CAPACITY;
    }

}
bool rb_pop(ring_buffer *rb, SensorData *item){
    if (rb->count == 0) return false;
    *item = rb->data[rb->tail];
    rb->tail = (rb->tail+1) % BUFFER_CAPACITY;
    rb->count--;
    return true;
}
bool rb_peek(ring_buffer *rb, SensorData *item) {
    if (rb->count == 0) {
        return false; 
    }
    *item = rb->data[rb->tail]; 
    return true;
}