#ifndef LED_BLINK_H
#define LED_BLINK_H

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

void led_init(void);
void led_start_blink(void);   // New function to start blinking
void led_blink_stop(void);

#endif // LED_BLINK_H
