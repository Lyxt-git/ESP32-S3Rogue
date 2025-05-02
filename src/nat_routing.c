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

#include "nat_routing.h"
#include "esp_log.h"
#include "esp_wifi.h"
#include "esp_netif.h"
#include "esp_netif_types.h"
#include "esp_netif_ip_addr.h"
#include "esp_netif_net_stack.h"
#include "esp_netif_nat.h" // The esp_netif_nat.h has been remove from the ESP-IDF v5.3.3, need to implement ourself
#include "esp_err.h"

static const char *TAG = "nat_routing";

void init_nat_routing() {
    ESP_LOGI(TAG, "Initializing NAT Routing...");

    // Set Wi-Fi to AP+STA mode
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_APSTA));

    // Get default netif handles
    esp_netif_t *ap_netif = esp_netif_get_handle_from_ifkey("WIFI_AP_DEF");
    esp_netif_t *sta_netif = esp_netif_get_handle_from_ifkey("WIFI_STA_DEF");

    if (!ap_netif || !sta_netif) {
        ESP_LOGE(TAG, "Failed to retrieve netif handles (AP or STA).");
        return;
    }

    // Enable NAT on STA interface (acts as upstream)
    esp_err_t err = esp_netif_nat_enable(sta_netif);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to enable NAT on STA: %s", esp_err_to_name(err));
        return;
    }
    ESP_LOGI(TAG, "NAT enabled on STA interface.");

    // Set AP interface to forward packets to STA (default netif)
    err = esp_netif_set_default_netif(ap_netif);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to set AP as default netif: %s", esp_err_to_name(err));
        return;
    }

    // Start DHCP server on AP interface
    err = esp_netif_dhcps_start(ap_netif);
    if (err != ESP_OK && err != ESP_ERR_ESP_NETIF_DHCP_ALREADY_STARTED) {
        ESP_LOGE(TAG, "Failed to start DHCP server on AP: %s", esp_err_to_name(err));
        return;
    }

    ESP_LOGI(TAG, "NAT Routing successfully initialized.");
}
