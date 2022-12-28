#include "rotation_sensor_v2.h"

void rotation_sensor_config(adc_unit_t adc_unit, adc_channel_t adc_channel_num, rotation_sensor_handle_t* rotation_sensor_handle){
    
    // TODO: move adc instance to another function as multiple instances will be generated if multiple rotation sensors are used
    
    adc_oneshot_unit_init_cfg_t adc_init_config = { // setting configuration for adc instance 
        .unit_id = adc_unit,
        .ulp_mode = ROTATION_ULP_MODE  // disabling low power mode
    };

    adc_oneshot_new_unit(&adc_init_config, &rotation_sensor_handle->rot_adc_handle); // initiating adc instance
    
    adc_oneshot_chan_cfg_t adc_chan_config = { // config for adc channel of rotation sensor
        .bitwidth = ROTATION_BW,
        .atten = ROTATION_ATTENUATION 
    };

    adc_oneshot_config_channel(rotation_sensor_handle->rot_adc_handle, adc_channel_num, &adc_chan_config); // applying config to given adc channel

    return;
}

int rotation_sensor_read(rotation_sensor_handle_t rotation_sensor_handle){
    int result = 0; // int to store result of adc read

    adc_oneshot_read(rotation_sensor_handle.rot_adc_handle, rotation_sensor_handle.rot_adc_channel_num, &result);

    // TODO: change conversion to fix linearity issues associated with ESP32's ADC
    result = MAX_ROTATION*result/MAX_ADC_OUTPUT; // converting adc value to degrees of rotation sensor
                                                 // conversion = max rotation * adc read / max adc value

    return result;
}

