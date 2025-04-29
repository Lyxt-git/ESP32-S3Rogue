#include "web_server.h"
#include "esp_log.h"
#include "esp_http_server.h"

static const char *TAG = "web_server";

// Placeholder HTTP GET handler
esp_err_t hello_get_handler(httpd_req_t *req) {
    const char resp[] = "Hello from ESP32 Rogue AP!";
    httpd_resp_send(req, resp, HTTPD_RESP_USE_STRLEN);

    // Return ESP_OK to indicate success
    return ESP_OK;
}

void init_web_server() {
    ESP_LOGI(TAG, "Initializing Web Server...");
    httpd_handle_t server = NULL;
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    ESP_ERROR_CHECK(httpd_start(&server, &config));

    httpd_uri_t uri_get = {
        .uri = "/",
        .method = HTTP_GET,
        .handler = hello_get_handler,
        .user_ctx = NULL
    };

    ESP_ERROR_CHECK(httpd_register_uri_handler(server, &uri_get));
    ESP_LOGI(TAG, "Web Server Initialized");
}
