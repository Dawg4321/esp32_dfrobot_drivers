#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_log.h"
#include "led_strip.h"

#include "rotation_sensor_v2.h"

// rotation sensor ADC macros
#define ROT_SENS_ADC ADC_UNIT_1
#define ROT_SENS_CHANNEL ADC_CHANNEL_0

#define LED_GPIO 48 // GPIO of onboard addressable LED
#define MAX_RGB_VAL 8 // maximum value of any RGB colour at once

void convert_rotation_to_rgb(int rotation_val, int* red, int* green, int* blue);

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
    }
}

void convert_rotation_to_rgb(int rotation_val, int* red, int* green, int* blue){ // function to convert linear rotation value into non-linear RGB value

    int rgb_val = (6*MAX_RGB_VAL)*rotation_val/MAX_ROTATION; // converting rotation degrees into RGB sequence range (0 - 255*6)
    int rgb_division = rgb_val/MAX_RGB_VAL; // int division to determine which step of non-linear aspect in RGB sequence 
    int rgb_modulus = rgb_val%MAX_RGB_VAL; // getting remainder to modify an rgb colour

    switch(rgb_division){
        case 0:
        case 6:
            *red = MAX_RGB_VAL;
            *green = rgb_modulus;
            *blue = 0;
            break;
        case 1:
            *red = MAX_RGB_VAL - rgb_modulus;
            *green = MAX_RGB_VAL;
            *blue = 0;
            break;
        case 2:
            *red = 0;
            *green = MAX_RGB_VAL;
            *blue = rgb_modulus;
            break;
        case 3:
            *red = 0;
            *green = MAX_RGB_VAL - rgb_modulus;
            *blue = MAX_RGB_VAL;
            break;
        case 4:
            *red = rgb_modulus;
            *green = 0;
            *blue = MAX_RGB_VAL;
            break;
        case 5:
            *red = MAX_RGB_VAL;
            *green = 0;
            *blue = MAX_RGB_VAL - rgb_modulus;
            break;
    }

    return;
}