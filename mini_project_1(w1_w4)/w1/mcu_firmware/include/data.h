#ifndef DATA_H
#define DATA_H
#include<stdio.h>
#include <esp_camera.h>
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
#define BUFFER_CAPACITY 2


void get_sample_data(void *pvParameters);

#endif