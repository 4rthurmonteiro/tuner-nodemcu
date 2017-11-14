#ifndef METRONOME_H

#include <stdlib.h>
#include "inc/esp_common.h"
#include "esp/uart.h"
#include "FreeRTOS.h"
#include "task.h"
#include "esp8266.h"

#define METRONOME_H

void metronomeTask(void *pvParameters);


#endif
