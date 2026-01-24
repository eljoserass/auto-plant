#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_comm.h"

static const char *TAG = "ROBOT_HOSE";

// Callback for received messages
static void on_data_recv(const uint8_t *mac_addr, const uint8_t *data, int len)
{
    ESP_LOGI(TAG, "Received %d bytes from %02X:%02X:%02X:%02X:%02X:%02X",
             len, mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
    ESP_LOGI(TAG, "Message: %.*s", len, data);
}

void app_main(void)
{
    printf("Starting robot-hose application...\n");

    // Initialize communication
    if (!esp_comm_init()) {
        ESP_LOGE(TAG, "Failed to initialize communication");
        return;
    }

    // Register receive callback
    esp_comm_register_recv_cb(on_data_recv);

    // Test: Send a response message every 5 seconds
    while (1) {
        const char *test_msg = "Hello from robot-hose!";
        ESP_LOGI(TAG, "Sending test message...");
        
        if (esp_comm_send(NULL, (uint8_t *)test_msg, strlen(test_msg))) {
            ESP_LOGI(TAG, "Message sent successfully");
        } else {
            ESP_LOGE(TAG, "Failed to send message");
        }

        vTaskDelay(pdMS_TO_TICKS(5000));
    }
}
