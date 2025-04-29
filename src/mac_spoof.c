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

#include "esp_wifi.h"
#include "mac_spoof.h"
#include <string.h>
#include <ctype.h>     // For isxdigit
#include <stdio.h>     // For fgets, sscanf
#include "esp_log.h"
#include "esp_system.h"

// Function to set the spoofed MAC address
void set_rogue_ap_mac(const char *mac) {
    uint8_t mac_addr[6];
    
    // Convert string MAC address to uint8_t array
    for (int i = 0; i < 6; i++) {
        sscanf(mac + i * 3, "%2hhx", &mac_addr[i]);
    }

    esp_err_t err = esp_wifi_set_mac(WIFI_IF_AP, mac_addr);  // Set MAC for AP interface
    if (err == ESP_OK) {
        ESP_LOGI("mac_spoof", "MAC address set to: %s", mac);
    } else {
        ESP_LOGE("mac_spoof", "Failed to set MAC address: %d", err);
    }
}

// Function to prompt and set MAC address for the Rogue AP
void prompt_and_set_mac() {
    char mac_input[18];  // Format: XX:XX:XX:XX:XX:XX
    ESP_LOGI("main", "Enter spoofed MAC (e.g., DE:AD:BE:EF:12:34):");

    if (fgets(mac_input, sizeof(mac_input), stdin)) {
        size_t len = strlen(mac_input);
        if (len > 0 && mac_input[len - 1] == '\n') {
            mac_input[len - 1] = '\0';  // Strip newline
        }

        if (validate_mac_format(mac_input)) {
            set_rogue_ap_mac(mac_input);
        } else {
            ESP_LOGW("main", "Invalid MAC format. Skipping spoof.");
        }
    } else {
        ESP_LOGE("main", "Failed to read MAC address input.");
    }
}

// Validate MAC format: must be XX:XX:XX:XX:XX:XX with hex digits
bool validate_mac_format(const char *mac) {
    if (strlen(mac) != 17) return false;

    for (int i = 0; i < 17; i++) {
        if (i % 3 == 2) {
            if (mac[i] != ':') return false;
        } else {
            if (!isxdigit((unsigned char)mac[i])) return false;
        }
    }
    return true;
}
