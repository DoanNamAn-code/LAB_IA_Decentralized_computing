#ifndef RING_BUFFER_H
#define RING_BUFFER_H

#include "stdint.h"
#include "stdbool.h"
#define BUFFER_CAPACITY 16

typedef struct 
{
    float timestamp;
    float temperature;
    uint8_t sensor_id;
} SensorData;

typedef struct 
{
    SensorData data[BUFFER_CAPACITY];
    int tail;
    int head;
    int count;
} ring_buffer;

void init(ring_buffer *rb);
bool rb_push(ring_buffer *rb, SensorData *item);
void rb_push_over(ring_buffer *rb, SensorData *item);
bool rb_pop(ring_buffer *rb, SensorData *item);
bool rb_peek(ring_buffer *rb, SensorData *item)

#endif