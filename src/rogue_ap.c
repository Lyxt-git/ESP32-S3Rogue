#include "rogue_ap.h"
#include "esp_log.h"
#include "esp_wifi.h"

static const char *TAG = "rogue_ap";

void init_rogue_ap() {
    ESP_LOGI(TAG, "Initializing Rogue AP...");
    // Initialize WiFi in station mode (joining a hotspot)
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_APSTA));  // Station and AP modes
    ESP_ERROR_CHECK(esp_wifi_start());

    ESP_LOGI(TAG, "Rogue AP Initialized");
}
