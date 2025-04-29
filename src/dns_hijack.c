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

#include "dns_hijack.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "lwip/sockets.h"
#include "lwip/inet.h"
#include <string.h>
#include <arpa/inet.h>

#define DNS_PORT 53
#define DNS_RESPONSE_IP "192.168.4.1"

static const char *TAG = "dns_hijack";

// DNS header structure
typedef struct {
    uint16_t id;       // identification number
    uint16_t flags;    // DNS flags
    uint16_t qdcount;  // number of question entries
    uint16_t ancount;  // number of answer entries
    uint16_t nscount;
    uint16_t arcount;
} __attribute__((packed)) dns_header_t;

static void dns_server_task(void *pvParameters) {
    int sock;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    char buffer[512];

    // Create UDP socket
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        ESP_LOGE(TAG, "Failed to create socket");
        vTaskDelete(NULL);
        return;
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(DNS_PORT);

    if (bind(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        ESP_LOGE(TAG, "Socket bind failed");
        close(sock);
        vTaskDelete(NULL);
        return;
    }

    ESP_LOGI(TAG, "DNS Hijack server started on port %d", DNS_PORT);

    while (1) {
        int len = recvfrom(sock, buffer, sizeof(buffer), 0,
                           (struct sockaddr *)&client_addr, &client_addr_len);
        if (len < sizeof(dns_header_t)) continue;

        dns_header_t *dns = (dns_header_t *)buffer;

        // Log the query
        ESP_LOGI(TAG, "DNS query received. ID: 0x%04X", ntohs(dns->id));

        // Prepare response
        dns->flags = htons(0x8180); // Standard response, No error
        dns->ancount = htons(1);    // 1 answer
        dns->nscount = 0;
        dns->arcount = 0;

        // Find the end of the query
        int query_end = sizeof(dns_header_t);
        while (buffer[query_end] != 0 && query_end < len) {
            query_end++;
        }
        query_end += 5; // null byte + type(2) + class(2)

        // Build answer
        char *answer = buffer + query_end;
        answer[0] = 0xC0;          // Name pointer
        answer[1] = 0x0C;          // Offset to domain name
        answer[2] = 0x00;          // Type A
        answer[3] = 0x01;
        answer[4] = 0x00;          // Class IN
        answer[5] = 0x01;
        answer[6] = 0x00; answer[7] = 0x00; answer[8] = 0x00; answer[9] = 0x3C; // TTL = 60s
        answer[10] = 0x00; answer[11] = 0x04; // Data length = 4
        inet_pton(AF_INET, DNS_RESPONSE_IP, answer + 12); // Set IP

        int response_len = query_end + 16;
        sendto(sock, buffer, response_len, 0, (struct sockaddr *)&client_addr, client_addr_len);
    }

    close(sock);
    vTaskDelete(NULL);
}

void init_dns_hijack() {
    ESP_LOGI(TAG, "Starting DNS hijack task...");
    xTaskCreatePinnedToCore(dns_server_task, "dns_server_task", 4096, NULL, 5, NULL, 0);
}
