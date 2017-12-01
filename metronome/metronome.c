#include <stdlib.h>
#include "espressif/esp_common.h"
#include "esp/uart.h"
#include "FreeRTOS.h"
#include "task.h"
#include "esp8266.h"
#include "queue.h"

const int gpio = 2; // the buzzer port
const int gpio_button_plus = 0;   /* gpio 0 usually has "PROGRAM" button attached */
const int gpio_button_low = 14;
const int active = 0; /* active == 0 for active low */

long map(long x, long in_min, long in_max, long out_min, long out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
/* This task uses the high level GPIO API (esp_gpio.h) to blink an LED.
 *
 */
void metronomeTask(void *pvParameters)
{
    long button = 40;

    gpio_enable(gpio, GPIO_OUTPUT);
    gpio_enable(gpio_button_low, GPIO_INPUT);

    long time = map(button, 0, 1023, 40, 210 ); // map in range of 40 to 210 bpm

    long interval = (1000/ time)*60; // convert time to bpm
    printf("intervalo inicial: %ld\n", interval);

    while(1) {
      while((gpio_read(gpio_button_plus) != active) || (gpio_read(gpio_button_low) != active)) // botão foi pressionado
        {
            if((gpio_read(gpio_button_plus) != active)) button++;

            if((gpio_read(gpio_button_low) != active)) button--;

            vTaskDelay(200 / portTICK_PERIOD_MS);
            printf("button atualizado: %ld\n", button);
            time = map(button, 0, 1023, 40, 218 ); // map in range of 40 to 218 bpm

            interval = (1000/ time)*60; // convert time to bpm
        }


        gpio_write(gpio, 1);

        vTaskDelay(100 / portTICK_PERIOD_MS);

        gpio_write(gpio, 0);

        printf("intervalo atualizado: %ld\n", interval);
        vTaskDelay(interval / portTICK_PERIOD_MS);


    }
}

void user_init(void)
{
    uart_set_baud(0, 115200);
    xTaskCreate(metronomeTask, "metronomeTask", 256, NULL , 2, NULL);


}
