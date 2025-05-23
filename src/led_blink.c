/*MIT License

Copyright (c) 2025 Lyxt

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.*/

#include "led_blink.h"
#include "driver/gpio.h"
#include "freertos/task.h"
#include "esp_rom_gpio.h"
#include "esp_wifi.h"
#include "esp_log.h"

#define LED_PIN GPIO_NUM_2

static TaskHandle_t blink_task_handle = NULL;

// Forward declaration
static void led_blink_task(void *pvParameters);

// Initialize LED pin only (no task started yet)
void led_init(void) {
    
    esp_rom_gpio_pad_select_gpio(LED_PIN);
    gpio_set_direction(LED_PIN, GPIO_MODE_OUTPUT);
}

// Start the blinking task (called after WiFi starts)
void led_start_blink(void) {
    
    esp_rom_gpio_pad_select_gpio(LED_PIN);
    gpio_set_direction(LED_PIN, GPIO_MODE_OUTPUT);

    if (blink_task_handle == NULL) {
        xTaskCreate(led_blink_task, "led_blink_task", 2048, NULL, 5, &blink_task_handle);
    }
}

// The LED blink task
static void led_blink_task(void *pvParameters) {
    while (1) {
        int delay_time_ms = 2000; // Default to slow blink

        wifi_sta_list_t wifi_sta_list;
        esp_err_t err = esp_wifi_ap_get_sta_list(&wifi_sta_list);

        if (err == ESP_OK && wifi_sta_list.num > 0) {
            delay_time_ms = 200; // Fast blink when clients are connected
        }

        gpio_set_level(LED_PIN, 1);
        vTaskDelay(pdMS_TO_TICKS(delay_time_ms));
        gpio_set_level(LED_PIN, 0);
        vTaskDelay(pdMS_TO_TICKS(delay_time_ms));
    }
}

// Stop blinking
void led_blink_stop(void) {
    if (blink_task_handle != NULL) {
        vTaskDelete(blink_task_handle);
        blink_task_handle = NULL;
    }
    gpio_set_level(LED_PIN, 0);
}
