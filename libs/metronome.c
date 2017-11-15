#include "inc/metronome.h"

const int gpio = 2;

void metronomeTask(void *pvParameters)
{
    gpio_enable(gpio, GPIO_OUTPUT);
    while(1) {
      short int dutycicle = sdk_system_adc_read();
      if(dutycicle > 1023) dutycicle = 1023;
        gpio_write(gpio, 1);
        vTaskDelay(dutycycle / portTICK_PERIOD_MS);
        gpio_write(gpio, 0);
        vTaskDelay(dutycycle / portTICK_PERIOD_MS);
    }
}
