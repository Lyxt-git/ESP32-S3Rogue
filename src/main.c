#include "mac_spoof.h"  // Include the MAC spoofing header
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
#include <ctype.h>  // Include the header for isxdigit

static const char *TAG = "main";

// Global flag to track if a client is connected to the AP
static bool client_connected = false;

// Forward declaration
void wifi_init_ap(void);
static void wifi_event_handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data);

// UART console init
void initialize_console() {
    const int uart_num = UART_NUM_0;
    uart_driver_install(uart_num, 256, 0, 0, NULL, 0);

    // Using the deprecated function as you mentioned
    esp_vfs_dev_uart_use_driver(uart_num);
}

void app_main(void) {
    ESP_LOGI(TAG, "ESP32-S3 Rogue AP Starting...");

    // 1. Initialize NVS
    ESP_ERROR_CHECK(nvs_flash_init());

    // 2. Initialize TCP/IP stack
    ESP_ERROR_CHECK(esp_netif_init());

    // 3. Create the default event loop
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    // 4. Initialize LED
    led_init();

    // 5. Create default WiFi AP interface
    esp_netif_create_default_wifi_ap();

    // 6. Register WiFi event handler
    ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &wifi_event_handler, NULL));

    // 7. Init WiFi stack
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_AP));

    // 8. Initialize UART for stdin input
    initialize_console();

    // 9. Prompt for MAC input
    prompt_and_set_mac();

    // 10. Setup WiFi AP
    wifi_init_ap();

    //DNS Hijack
    init_dns_hijack();

    // 11. Start Web Server
    init_web_server();

    // Idle loop
    while (true) {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void wifi_init_ap(void) {
    ESP_LOGI(TAG, "Setting up WiFi AP...");

    wifi_config_t ap_config = {
        .ap = {
            .ssid = "YourAP",
            .ssid_len = strlen("YourAP"),
            .channel = 6,
            .password = "", //Leave blank for Open Network
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

    // Start blinking LED after AP is successfully started
    led_start_blink();

    ESP_LOGI(TAG, "WiFi AP started. SSID:%s Password:%s Channel:%d",
             ap_config.ap.ssid, ap_config.ap.password, ap_config.ap.channel);
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
