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

#include "dns_server.h"
#include "esp_log.h"
#include "lwip/err.h"
#include "lwip/sys.h"

static const char *TAG = "dns_server";

void init_dns_server() {
    ESP_LOGI(TAG, "Initializing DNS Server...");
    // Initialize DNS server (this is a placeholder; actual DNS handling will need lwIP or another library)
    // TODO: Implement DNS Server logic here (e.g., using lwIP DNS server or custom DNS logic)
    ESP_LOGI(TAG, "DNS Server Initialized");
}
