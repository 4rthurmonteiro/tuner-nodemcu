#include <espressif/esp_common.h>
#include <esp/uart.h>
#include <FreeRTOS.h>
#include <task.h>
#include <queue.h>
#include <timers.h>
#include <string.h>
#include <ssd1306/ssd1306.h>

#define LOAD_ICON_X 54
#define LOAD_ICON_Y 42
#define LOAD_ICON_SIZE 20

#define CIRCLE_COUNT_ICON_X 100
#define CIRCLE_COUNT_ICON_Y 52

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

TimerHandle_t fps_timer_handle = NULL; // Timer handler
TimerHandle_t font_timer_handle = NULL;

uint8_t frame_done = 0; // number of frame send.
uint8_t fps = 0; // image per second.

const font_info_t *font = NULL; // current font
font_face_t font_face = 0;

float freq = 85;


#define SECOND (1000 / portTICK_PERIOD_MS)

static void ssd1306_task(void *pvParameters)
{
    float  *frequency;
    frequency = (float *)pvParameters;
    printf("%s: Started user interface task\n", __FUNCTION__);
    vTaskDelay(SECOND);

    ssd1306_set_whole_display_lighting(&dev, false);


    while (1) {

        if(*frequency < 80){
          ssd1306_draw_string(&dev, buffer, font_builtin_fonts[DEFAULT_FONT], 34, 12, "E +", OLED_COLOR_WHITE, OLED_COLOR_BLACK);
        }

        if(*frequency >= 80.0 && *frequency < 85 ){
          ssd1306_draw_string(&dev, buffer, font_builtin_fonts[DEFAULT_FONT], 0, 12, ">> E <<", OLED_COLOR_BLACK, OLED_COLOR_WHITE);
        }

        if(*frequency >= 85.0 && *frequency < 95 ){
          ssd1306_draw_string(&dev, buffer, font_builtin_fonts[DEFAULT_FONT], 34, 12, "- E", OLED_COLOR_WHITE, OLED_COLOR_BLACK);
        }

        if(*frequency >= 95.0 && *frequency < 105 ){
          ssd1306_draw_string(&dev, buffer, font_builtin_fonts[DEFAULT_FONT], 34, 12, "A +", OLED_COLOR_WHITE, OLED_COLOR_BLACK);
        }

        if(*frequency >= 105.0 && *frequency < 115 ){
            ssd1306_draw_string(&dev, buffer, font_builtin_fonts[DEFAULT_FONT], 0, 12, ">> A <<", OLED_COLOR_BLACK, OLED_COLOR_WHITE);
        }

        if(*frequency >= 115.0 && *frequency < 125 ){
          ssd1306_draw_string(&dev, buffer, font_builtin_fonts[DEFAULT_FONT], 34, 12, "- A", OLED_COLOR_WHITE, OLED_COLOR_BLACK);
        }

        if(*frequency >= 125.0 && *frequency < 140 ){
          ssd1306_draw_string(&dev, buffer, font_builtin_fonts[DEFAULT_FONT], 34, 12, "D +", OLED_COLOR_WHITE, OLED_COLOR_BLACK);
        }

        if(*frequency >= 140.0 && *frequency < 150 ){
          ssd1306_draw_string(&dev, buffer, font_builtin_fonts[DEFAULT_FONT], 0, 12, ">> D <<", OLED_COLOR_BLACK, OLED_COLOR_WHITE);
        }

        if(*frequency >= 150.0 && *frequency < 165 ){
          ssd1306_draw_string(&dev, buffer, font_builtin_fonts[DEFAULT_FONT], 34, 12, "- D", OLED_COLOR_WHITE, OLED_COLOR_BLACK);
        }

        if(*frequency >= 165.0 && *frequency < 190 ){
          ssd1306_draw_string(&dev, buffer, font_builtin_fonts[DEFAULT_FONT], 34, 12, "G +", OLED_COLOR_WHITE, OLED_COLOR_BLACK);
        }

        if(*frequency >= 190.0 && *frequency < 200 ){
          ssd1306_draw_string(&dev, buffer, font_builtin_fonts[DEFAULT_FONT], 0, 12, ">> G <<", OLED_COLOR_BLACK, OLED_COLOR_WHITE);
        }

        if(*frequency >= 200.0 && *frequency < 215 ){
          ssd1306_draw_string(&dev, buffer, font_builtin_fonts[DEFAULT_FONT], 34, 12, "- G", OLED_COLOR_WHITE, OLED_COLOR_BLACK);
        }

        if(*frequency >= 215.0 && *frequency < 240 ){
          ssd1306_draw_string(&dev, buffer, font_builtin_fonts[DEFAULT_FONT], 34, 12, "B +", OLED_COLOR_WHITE, OLED_COLOR_BLACK);
        }

        if(*frequency >= 240.0 && *frequency < 250 ){
          ssd1306_draw_string(&dev, buffer, font_builtin_fonts[DEFAULT_FONT], 0, 12, ">> B <<", OLED_COLOR_BLACK, OLED_COLOR_WHITE);
        }

        if(*frequency >= 250.0 && *frequency < 280 ){
          ssd1306_draw_string(&dev, buffer, font_builtin_fonts[DEFAULT_FONT], 34, 12, "- B", OLED_COLOR_WHITE, OLED_COLOR_BLACK);
        }

        if(*frequency >= 280.0 && *frequency < 320 ){
          ssd1306_draw_string(&dev, buffer, font_builtin_fonts[DEFAULT_FONT], 34, 12, "E +", OLED_COLOR_WHITE, OLED_COLOR_BLACK);
        }

        if(*frequency >= 320.0 && *frequency < 330 ){
          ssd1306_draw_string(&dev, buffer, font_builtin_fonts[DEFAULT_FONT], 0, 12, ">> E <<", OLED_COLOR_BLACK, OLED_COLOR_WHITE);
        }

        if(*frequency >= 330.0){
          ssd1306_draw_string(&dev, buffer, font_builtin_fonts[DEFAULT_FONT], 34, 12, "- E", OLED_COLOR_WHITE, OLED_COLOR_BLACK);
        }


        vTaskDelay(SECOND);

        if (ssd1306_load_frame_buffer(&dev, buffer))
            goto error_loop;

        //frame_done++;
    }

error_loop:
    printf("%s: error while loading framebuffer into SSD1306\n", __func__);
    for (;;) {
        vTaskDelay(2 * SECOND);
        printf("%s: error loop\n", __FUNCTION__);
    }
}

void fps_timer(TimerHandle_t h)
{
    fps = frame_done; // Save number of frame already send to screen
    frame_done = 0;
}

void font_timer(TimerHandle_t h)
{
    do {
        if (++font_face >= font_builtin_fonts_count)
            font_face = 0;
        font = font_builtin_fonts[font_face];
    } while (!font);

    printf("Selected builtin font %d\n", font_face);
}

void user_init(void)
{
    //uncomment to test with CPU overclocked
    //sdk_system_update_cpu_freq(160);

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
    xTaskCreate(ssd1306_task, "ssd1306_task", 256, &freq, 2, NULL);


    int i;
    for(i = 0; i < 100; i ++){
      freq++;
      vTaskDelay(SECOND);

    }
/*
    fps_timer_handle = xTimerCreate("fps_timer", SECOND, pdTRUE, NULL, fps_timer);
    xTimerStart(fps_timer_handle, 0);

    font_timer_handle = xTimerCreate("font_timer", 5 * SECOND, pdTRUE, NULL, font_timer);
    xTimerStart(font_timer_handle, 0);
*/
}
