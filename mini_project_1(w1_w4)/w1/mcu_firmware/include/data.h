#ifndef DATA_H
#define DATA_H

#include "base_lib.h"
#include "ring_buffer.h"

//Sample data 
// #define BUFFER_CAPACITY 32
// typedef struct 
// {
//     char device_id[16];
//     float temperature;
//     float humidity;
//     uint32_t timestamp_ms;
// } SensorData;
//camera data



// void get_sample_data(void *pvParameters);
void get_camera_data(void* pvParameters);
#endif