#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_log.h"
#include "led_strip.h"

#define LED_GPIO 48 // GPIO of the devkit's addressable led

#define MAX_RGB_VAL 16 // Maximum value of any RGB colour at once

#define APPROX_LEN_OF_COLOUR_CYCLE 10000 // length between each RGB value step change in ms

void app_main(void)
{
    static const char* TAG = "LED Colour";

    led_strip_handle_t led_strip;
    led_strip_config_t led_config = {
        .strip_gpio_num = LED_GPIO,
        .max_leds = 1,
    };
    led_strip_rmt_config_t rmt_config = {
        .resolution_hz = 10 * 1000 * 1000,
    };

    led_strip_new_rmt_device(&led_config, &rmt_config, &led_strip);

    int red = MAX_RGB_VAL; // starting RGB values for algorithm 
    int blue = 0;
    int green = 0;

    float col_cycle_delay = (APPROX_LEN_OF_COLOUR_CYCLE/MAX_RGB_VAL); // calculating length of steps for specified delay length of the colour ` 

    while(1){
        for(int i = 0; i < MAX_RGB_VAL*6; i++){
            
            if(i < MAX_RGB_VAL){
                green++;
            }
            else if(i < 2*MAX_RGB_VAL){
                red--;
            }
            else if (i < 3*MAX_RGB_VAL){
                blue++;
            }
            else if( i < 4*MAX_RGB_VAL){
                green--;
            }
            else if ( i < 5*MAX_RGB_VAL){
                red++;
            }
            else{
                blue--;
            }
            
            led_strip_set_pixel(led_strip, 0, red, green, blue);
            led_strip_refresh(led_strip);
            
            vTaskDelay(col_cycle_delay/portTICK_PERIOD_MS);

            ESP_LOGI(TAG, "R: %d G: %d B: %d\n", red, green, blue);
        }
    }
}