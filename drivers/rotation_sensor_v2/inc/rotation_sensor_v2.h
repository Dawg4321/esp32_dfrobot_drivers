#ifndef ROTATION_SENSOR_V2_H
#define ROTATION_SENSOR_V2_H

#include "esp_adc/adc_oneshot.h"

// Rotation Sensor Docs: https://wiki.dfrobot.com/Analog_Rotation_Sensor_V2__SKU__DFR0058_

// adc settings
#define ROTATION_BW ADC_BITWIDTH_12 // 12 bit resolution -> 4k (2^12) possible adc values 
#define ROTATION_ATTENUATION ADC_ATTEN_DB_11 // 11 dB attenuation -> 0 - 3.3 V range 

// disabling Ultra Low Power mode on ADC
#define ROTATION_ULP_MODE ADC_ULP_MODE_DISABLE

#define MAX_ROTATION 3600 // maximum rotation of sensor in degrees
#define MAX_ADC_OUTPUT 4095 // maximum value for adc as determined by resolution (2^12 - 1)

typedef struct { // struct to store required data for rotation sensor reads
    adc_oneshot_unit_handle_t rot_adc_handle;
    adc_channel_t rot_adc_channel_num;
}rotation_sensor_handle_t;

void rotation_sensor_config(adc_unit_t adc_unit, adc_channel_t adc_channel_num, rotation_sensor_handle_t* rotation_sensor_handle); // configures rotation sensor adc

int rotation_sensor_read(rotation_sensor_handle_t rotation_sensor_handle); // reads rotation sensor output using adc

#endif