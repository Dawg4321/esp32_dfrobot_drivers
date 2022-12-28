#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_log.h"

#include "rotation_sensor_v2.h"

// macros deining the rotation sensor pin
#define ROT_SENS_ADC ADC_UNIT_1
#define ROT_SENS_CHANNEL ADC_CHANNEL_0

void app_main(void)
{
    static const char* TAG = "Rotation Sensor";

    rotation_sensor_handle_t rot_sens_handle;

    rotation_sensor_config(ROT_SENS_ADC, ROT_SENS_CHANNEL, &rot_sens_handle);
    
    int rotation_val;
    while(1){
        rotation_val = rotation_sensor_read(rot_sens_handle);
        ESP_LOGI(TAG, "%d degrees\n", rotation_val);
        vTaskDelay(100);

    }
}
    