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

#include "mac_spoof.h"
#include "esp_log.h"
#include "esp_system.h"
#include "string.h"
#include "esp_netif.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "nvs_flash.h"
#include "web_server.h"
#include "led_blink.h"
#include "esp_vfs_dev.h"
#include "driver/uart.h"
#include "dns_hijack.h"
#include "nat_routing.h"
#include <ctype.h>  // for isxdigit

static const char *TAG = "main";
static bool client_connected = false;

void wifi_init_ap(void);
void wifi_init_sta(void);
static void wifi_event_handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data);

void initialize_console() {
    const int uart_num = UART_NUM_0;
    uart_driver_install(uart_num, 256, 0, 0, NULL, 0);
    esp_vfs_dev_uart_use_driver(uart_num);
}

void wifi_init_ap(void) {
    ESP_LOGI(TAG, "Setting up WiFi AP...");

    wifi_config_t ap_config = {
        .ap = {
            .ssid = "YourAP",
            .ssid_len = strlen("YourAP"),
            .channel = 6,
            .password = "", // Open network
            .max_connection = 4,
            .authmode = WIFI_AUTH_WPA2_PSK,
            .pmf_cfg = {
                .required = false,
            },
        },
    };

    if (strlen((const char *)ap_config.ap.password) == 0) {
        ap_config.ap.authmode = WIFI_AUTH_OPEN;
    }

    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_AP, &ap_config));
    ESP_ERROR_CHECK(esp_wifi_start());

    led_start_blink();

    ESP_LOGI(TAG, "WiFi AP started. SSID:%s Password:%s Channel:%d",
             ap_config.ap.ssid, ap_config.ap.password, ap_config.ap.channel);
}

void wifi_init_sta(void) {
    ESP_LOGI(TAG, "Setting up WiFi STA...");

    wifi_config_t sta_config = {
        .sta = {
            .ssid = "YourRouterSSID",           // Replace with actual SSID
            .password = "YourRouterPassword",   // Replace with actual password
            .threshold.authmode = WIFI_AUTH_WPA2_PSK,
            .pmf_cfg = {
                .capable = true,
                .required = false
            },
        },
    };

    esp_netif_create_default_wifi_sta();  // Create default STA interface

    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &sta_config));
    ESP_ERROR_CHECK(esp_wifi_connect());

    ESP_LOGI(TAG, "STA connection initiated");
}

static void wifi_event_handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data) {
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_AP_STACONNECTED) {
        ESP_LOGI(TAG, "Client connected to AP");
        client_connected = true;
    }

    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_AP_STADISCONNECTED) {
        ESP_LOGI(TAG, "Client disconnected from AP");
        client_connected = false;
    }
}

void app_main(void) {
    ESP_LOGI(TAG, "ESP32-S3 Rogue AP Starting...");

    ESP_ERROR_CHECK(nvs_flash_init());
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    led_init();

    esp_netif_create_default_wifi_ap();  // AP interface created here

    ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &wifi_event_handler, NULL));

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_APSTA));  // Enable both AP and STA

    initialize_console();

    prompt_and_set_mac();

    wifi_init_ap();
    wifi_init_sta();

    init_nat_routing();     // ← Set up NAT after interfaces are active

    init_dns_hijack();
    init_web_server();

    while (true) {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
