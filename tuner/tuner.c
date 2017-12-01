#include <espressif/esp_common.h>
#include <esp/uart.h>
#include <FreeRTOS.h>
#include <task.h>
#include <queue.h>
#include <timers.h>
#include <string.h>
#include <ssd1306/ssd1306.h>
#include <stdlib.h>
#include "esp8266.h"

const int gpio = 2;
const int gpio_button_plus = 16;   /* gpio 0 usually has "PROGRAM" button attached */
const int gpio_button_low = 14;
const int gpio_switch = 12;
const int active = 0; /* active == 0 for active low */
//
/* Remove this line if your display connected by SPI */
#define I2C_CONNECTION

#ifdef I2C_CONNECTION
    #include <i2c/i2c.h>
#endif
#include "fonts/fonts.h"

/* Change this according to you schematics and display size */
#define DISPLAY_WIDTH  128
#define DISPLAY_HEIGHT 64


#ifdef I2C_CONNECTION
    #define PROTOCOL SSD1306_PROTO_I2C
    #define ADDR     SSD1306_I2C_ADDR_0
    #define I2C_BUS  0
    #define SCL_PIN  5
    #define SDA_PIN  4
#else
    #define PROTOCOL SSD1306_PROTO_SPI4
    #define CS_PIN   5
    #define DC_PIN   4
#endif

#define DEFAULT_FONT FONT_FACE_TERMINUS_BOLD_16X32_ISO8859_1

/* Declare device descriptor */
static const ssd1306_t dev = {
    .protocol = PROTOCOL,
#ifdef I2C_CONNECTION
.i2c_dev.bus      = I2C_BUS,
.i2c_dev.addr     = ADDR,
#else
    .cs_pin   = CS_PIN,
    .dc_pin   = DC_PIN,
#endif
    .width    = DISPLAY_WIDTH,
    .height   = DISPLAY_HEIGHT
};

/* Local frame buffer */
static uint8_t buffer[DISPLAY_WIDTH * DISPLAY_HEIGHT / 8];



uint8_t frame_done = 0; // number of frame send.
uint8_t fps = 0; // image per second.

const font_info_t *font = NULL; // current font
font_face_t font_face = 0;

float freq = 85;


#define SECOND (1000 / portTICK_PERIOD_MS)

long map(long x, long in_min, long in_max, long out_min, long out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

/* This task uses the high level GPIO API (esp_gpio.h) to blink an LED.
 *
 */
void blinkenTask(void *pvParameters)
{
    long button = 40;

    QueueHandle_t *tsqueue = (QueueHandle_t *)pvParameters;
//    gpio_enable(gpio_switch, GPIO_INPUT);

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

static void ssd1306_task(void *pvParameters)
{
  //  gpio_enable(gpio_switch, GPIO_INPUT);

    float  *frequency;
    frequency = (float *)pvParameters;
    printf("%s: Started user interface task\n", __FUNCTION__);
    vTaskDelay(SECOND);

    ssd1306_set_whole_display_lighting(&dev, false);

    while (1) {
      while(gpio_read(gpio_switch) != active)
        {
            taskYIELD();
        }

        if(*frequency < 80){
          ssd1306_draw_string(&dev, buffer, font_builtin_fonts[DEFAULT_FONT], 34, 12, "E +", OLED_COLOR_WHITE, OLED_COLOR_BLACK);
        }

        if(*frequency >= 80.0 && *frequency < 85 ){
          ssd1306_draw_string(&dev, buffer, font_builtin_fonts[DEFAULT_FONT], 34, 12, " E ", OLED_COLOR_WHITE, OLED_COLOR_BLACK);
        }

        if(*frequency >= 85.0 && *frequency < 95 ){
          ssd1306_draw_string(&dev, buffer, font_builtin_fonts[DEFAULT_FONT], 34, 12, "- E", OLED_COLOR_WHITE, OLED_COLOR_BLACK);
        }

        if(*frequency >= 95.0 && *frequency < 105 ){
          ssd1306_draw_string(&dev, buffer, font_builtin_fonts[DEFAULT_FONT], 34, 12, "A +", OLED_COLOR_WHITE, OLED_COLOR_BLACK);
        }

        if(*frequency >= 105.0 && *frequency < 115 ){
            ssd1306_draw_string(&dev, buffer, font_builtin_fonts[DEFAULT_FONT], 34, 12, " A ", OLED_COLOR_WHITE, OLED_COLOR_BLACK);
        }

        if(*frequency >= 115.0 && *frequency < 125 ){
          ssd1306_draw_string(&dev, buffer, font_builtin_fonts[DEFAULT_FONT], 34, 12, "- A", OLED_COLOR_WHITE, OLED_COLOR_BLACK);
        }

        if(*frequency >= 125.0 && *frequency < 140 ){
          ssd1306_draw_string(&dev, buffer, font_builtin_fonts[DEFAULT_FONT], 34, 12, "D +", OLED_COLOR_WHITE, OLED_COLOR_BLACK);
        }

        if(*frequency >= 140.0 && *frequency < 150 ){
          ssd1306_draw_string(&dev, buffer, font_builtin_fonts[DEFAULT_FONT], 34, 12, "D", OLED_COLOR_WHITE, OLED_COLOR_BLACK);
        }

        if(*frequency >= 150.0 && *frequency < 165 ){
          ssd1306_draw_string(&dev, buffer, font_builtin_fonts[DEFAULT_FONT], 34, 12, "- D", OLED_COLOR_WHITE, OLED_COLOR_BLACK);
        }

        if(*frequency >= 165.0 && *frequency < 190 ){
          ssd1306_draw_string(&dev, buffer, font_builtin_fonts[DEFAULT_FONT], 34, 12, "G +", OLED_COLOR_WHITE, OLED_COLOR_BLACK);
        }

        if(*frequency >= 190.0 && *frequency < 200 ){
          ssd1306_draw_string(&dev, buffer, font_builtin_fonts[DEFAULT_FONT], 34, 12, "G", OLED_COLOR_WHITE, OLED_COLOR_BLACK);
        }

        if(*frequency >= 200.0 && *frequency < 215 ){
          ssd1306_draw_string(&dev, buffer, font_builtin_fonts[DEFAULT_FONT], 34, 12, "- G", OLED_COLOR_WHITE, OLED_COLOR_BLACK);
        }

        if(*frequency >= 215.0 && *frequency < 240 ){
          ssd1306_draw_string(&dev, buffer, font_builtin_fonts[DEFAULT_FONT], 34, 12, "B +", OLED_COLOR_WHITE, OLED_COLOR_BLACK);
        }

        if(*frequency >= 240.0 && *frequency < 250 ){
          ssd1306_draw_string(&dev, buffer, font_builtin_fonts[DEFAULT_FONT], 34, 12, "B", OLED_COLOR_WHITE, OLED_COLOR_BLACK);
        }

        if(*frequency >= 250.0 && *frequency < 280 ){
          ssd1306_draw_string(&dev, buffer, font_builtin_fonts[DEFAULT_FONT], 34, 12, "- B", OLED_COLOR_WHITE, OLED_COLOR_BLACK);
        }

        if(*frequency >= 280.0 && *frequency < 320 ){
          ssd1306_draw_string(&dev, buffer, font_builtin_fonts[DEFAULT_FONT], 34, 12, "E +", OLED_COLOR_WHITE, OLED_COLOR_BLACK);
        }

        if(*frequency >= 320.0 && *frequency < 330 ){
          ssd1306_draw_string(&dev, buffer, font_builtin_fonts[DEFAULT_FONT], 34, 12, "E", OLED_COLOR_WHITE, OLED_COLOR_BLACK);
        }

        if(*frequency >= 330.0){
          ssd1306_draw_string(&dev, buffer, font_builtin_fonts[DEFAULT_FONT], 34, 12, "- E", OLED_COLOR_WHITE, OLED_COLOR_BLACK);
        }


        vTaskDelay(SECOND);

        if (ssd1306_load_frame_buffer(&dev, buffer))
            goto error_loop;
    }

error_loop:
    printf("%s: error while loading framebuffer into SSD1306\n", __func__);
    for (;;) {
        vTaskDelay(2 * SECOND);
        printf("%s: error loop\n", __FUNCTION__);
    }
}

static QueueHandle_t tsqueue;


void user_init(void)
{
    // Setup HW
    uart_set_baud(0, 115200);

    printf("SDK version:%s\n", sdk_system_get_sdk_version());

#ifdef I2C_CONNECTION
    i2c_init(I2C_BUS, SCL_PIN, SDA_PIN, I2C_FREQ_400K);
#endif

    while (ssd1306_init(&dev) != 0) {
        printf("%s: failed to init SSD1306 lcd\n", __func__);
        vTaskDelay(SECOND);
    }
    ssd1306_set_whole_display_lighting(&dev, true);
    vTaskDelay(SECOND);

    font = font_builtin_fonts[font_face];



    // Create user interface task
    tsqueue = xQueueCreate(2, sizeof(uint32_t));
    xTaskCreate(ssd1306_task, "ssd1306_task", 256, &freq, 1, NULL);
    xTaskCreate(blinkenTask, "blinkenTask", 256, &tsqueue , 2, NULL);

/*
    int i;
    for(i = 0; i < 100; i ++){
      freq++;
      vTaskDelay(SECOND);
*/
//    }
}
