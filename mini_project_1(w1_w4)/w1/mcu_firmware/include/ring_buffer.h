#ifndef RING_BUFFER_H
#define RING_BUFFER_H

#include "stdint.h"
#include "stdbool.h"
#include "freertos/FreeRTOS.h"
#include "data.h"



typedef struct 
{
    camera_fb_t *data[BUFFER_CAPACITY];
    int tail;
    int head;
    int count; 
    SemaphoreHandle_t mutex;
} ring_buffer;

void init(ring_buffer *rb);
bool rb_push(ring_buffer *rb, camera_fb_t *item);
bool rb_push_over(ring_buffer *rb, camera_fb_t *item);
bool rb_pop(ring_buffer *rb, camera_fb_t **item);

#endif