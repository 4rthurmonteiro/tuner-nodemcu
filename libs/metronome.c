/* The classic "blink" example
 *
 * This sample code is in the public domain.
 */
#include <stdlib.h>
#include "espressif/esp_common.h"
#include "esp/uart.h"
#include "FreeRTOS.h"
#include "task.h"
#include "esp8266.h"

const int volplus = 4; // port d2
const int volless = 5; // port d3
const int gpio = 2;
const int oneMinute = 60;

/* This task uses the high level GPIO API (esp_gpio.h) to blink an LED.
 *
 */
int contador = 60;

void blinkenTask(void *pvParameters)
{
    gpio_enable(gpio, GPIO_OUTPUT);
    gpio_enable(volplus, GPIO_INPUT);

    while(1) {
        //short int dutycycle =  sdk_system_adc_read(); /* read continuous POT and set PWM duty cycle according */
        //if(dutycycle < 60) dutycycle = 61;
        //printf("Variação do Potênciometro: %i\n", dutycycle);
        if(volplus == 1) {
          contador++;
          vTaskDelay(250);
        }

        if(volless == 1) {
          contador--;
          vTaskDelay(250);
        }

        if(contador > 200) contador == 200;

        if(contador < 60) contador = 60;

        gpio_write(gpio, 1);
        vTaskDelay(500 / oneMinute);

        gpio_write(gpio, 0);
        vTaskDelay((contador / oneMinute)*1000);
    }
}

void user_init(void)
{
    uart_set_baud(0, 115200);
    xTaskCreate(blinkenTask, "blinkenTask", 256, NULL, 2, NULL);
    //xTaskCreate(blinkenRegisterTask, "blinkenRegisterTask", 256, NULL, 2, NULL);
}

