#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_log.h"
#include "led_strip.h"

#include "rotation_sensor_v2.h"

// rotation sensor ADC macros
#define ROT_SENS_ADC ADC_UNIT_1
#define ROT_SENS_CHANNEL ADC_CHANNEL_0

#define LED_GPIO 48 // GPIO of onboard addressable LED
#define MAX_RGB_VAL 16 // maximum value of any RGB colour at once

void convert_rotation_to_rgb(int rotation_val, int* red, int* blue, int* green);

void app_main(void)
{
    static const char* TAG = "Rotation Sensor";

    // configuring addressable LED strip
    led_strip_handle_t led_strip;
    led_strip_config_t led_config = {
        .strip_gpio_num = LED_GPIO,
        .max_leds = 1,
    };
    led_strip_rmt_config_t rmt_config = {
        .resolution_hz = 10 * 1000 * 1000,
    };

    led_strip_new_rmt_device(&led_config, &rmt_config, &led_strip);
    led_strip_clear(led_strip);

    // configuring rotation sensor's ADC
    static rotation_sensor_handle_t rot_sens_handle;
    rotation_sensor_config(ROT_SENS_ADC, ROT_SENS_CHANNEL, &rot_sens_handle);

    int rotation_val, red, green, blue;

    while(1){
        
        rotation_val = rotation_sensor_read(rot_sens_handle);

        // have a rotation value which is linear, need to convert to non-linear RGB value to adjust the LED
        convert_rotation_to_rgb(rotation_val, &red, &green, &blue); // adjusting RGB value

        led_strip_set_pixel(led_strip, 0, red, green, blue);
        led_strip_refresh(led_strip);

        ESP_LOGD(TAG, "%d degrees\n", rotation_val);
    }
}

void convert_rotation_to_rgb(int rotation_val, int* red, int* blue, int* green){ // function to convert linear rotation value into non-linear RGB value
    
    *red = MAX_RGB_VAL; // resetting RGB values 
    *blue = 0;
    *green = 0;

    for(int i = 0; i < (6*MAX_RGB_VAL)*rotation_val/MAX_ROTATION; i++){
        
        if(i < MAX_RGB_VAL){
            (*green)++;
        }
        else if(i < 2*MAX_RGB_VAL){
            (*red)--;
        }
        else if (i < 3*MAX_RGB_VAL){
            (*blue)++;
        }
        else if( i < 4*MAX_RGB_VAL){
            (*green)--;
        }
        else if ( i < 5*MAX_RGB_VAL){
            (*red)++;
        }
        else{
            (*blue)--;
        }
    }
    
    return;
}