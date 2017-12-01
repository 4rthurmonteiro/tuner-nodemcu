#include <stdlib.h>
#include "espressif/esp_common.h"
#include "esp/uart.h"
#include "FreeRTOS.h"
#include "task.h"
#include "esp8266.h"

const int gpio = 2;

long map(long x, long in_min, long in_max, long out_min, long out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
/* This task uses the high level GPIO API (esp_gpio.h) to blink an LED.
 *
 */
void blinkenTask(void *pvParameters)
{

    gpio_enable(gpio, GPIO_OUTPUT);

    long time = map(40, 0, 1023, 40, 210 ); // map in range of 40 to 210 bpm

    long interval = (1000/ time)*60; // convert time to bpm

    while(1) {
        gpio_write(gpio, 1);

        vTaskDelay(100 / portTICK_PERIOD_MS);

        gpio_write(gpio, 0);

        vTaskDelay(interval / portTICK_PERIOD_MS);
    }
}

void user_init(void)
{
    uart_set_baud(0, 115200);
    xTaskCreate(blinkenTask, "blinkenTask", 256, NULL, 2, NULL);
}

