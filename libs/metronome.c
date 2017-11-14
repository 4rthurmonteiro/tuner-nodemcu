#include "inc/metronome.h"

const int gpio = 2;
const int gpio_analog = 0;

void metronomeTask(void *pvParameters)
{
    gpio_enable(gpio, GPIO_OUTPUT);
    gpio_enable(gpio_analog, GPIO_INPUT);
    while(1) {
      short int dutycicle = gpio_analog;
      if(dutycicle > 1023) dutycicle = 1023;
        gpio_write(gpio, 1);
        vTaskDelay(1000 / dutycicle);
        gpio_write(gpio, 0);
        vTaskDelay(1000 / dutycicle);
    }
}
