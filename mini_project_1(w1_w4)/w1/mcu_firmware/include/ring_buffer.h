#ifndef RING_BUFFER_H
#define RING_BUFFER_H

#include "stdint.h"
#include "stdbool.h"
#include "base_lib.h"



#define BUFFER_CAPACITY 1

typedef struct 
{
    camera_fb_t *data[BUFFER_CAPACITY];
    // SensorData *data[BUFFER_CAPACITY];
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