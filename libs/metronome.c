#include "inc/metronome.h"

const int gpio = 2;
const int oneMinute = 60;

void metronomeTask(void *pvParameters)
{
gpio_enable(gpio, GPIO_OUTPUT);


    while(1) {
        short int dutycycle =  sdk_system_adc_read(); /* read continuous POT and set PWM duty cycle according */
        if(dutycycle < 60) dutycycle = 61;
        printf("Variação do Potênciometro: %i\n", dutycycle);

        gpio_write(gpio, 1);
        vTaskDelay(500 / oneMinute);

        gpio_write(gpio, 0);
        vTaskDelay((dutycycle / oneMinute)*1000);
    }
    }
}
