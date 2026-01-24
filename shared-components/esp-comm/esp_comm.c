#include "esp_comm.h"
#include <string.h>
#include "esp_log.h"
#include "esp_wifi.h"
#include "esp_now.h"
#include "nvs_flash.h"
#include "esp_netif.h"
#include "esp_mac.h"
#include "esp_wifi_types.h"

static const char *TAG = "ESP_COMM";
static esp_comm_recv_cb_t recv_callback = NULL;

// ESP-NOW receive callback
static void espnow_recv_cb(const esp_now_recv_info_t *recv_info, const uint8_t *data, int len)
{
    if (recv_callback != NULL) {
        recv_callback(recv_info->src_addr, data, len);
    }
}

// ESP-NOW send callback
static void espnow_send_cb(const wifi_tx_info_t *tx_info, esp_now_send_status_t status)
{
    ESP_LOGI(TAG, "Send status to %02X:%02X:%02X:%02X:%02X:%02X: %s",
             tx_info->des_addr[0], tx_info->des_addr[1], tx_info->des_addr[2],
             tx_info->des_addr[3], tx_info->des_addr[4], tx_info->des_addr[5],
             status == ESP_NOW_SEND_SUCCESS ? "Success" : "Fail");
}

bool esp_comm_init(void)
{
    // Initialize NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    // Initialize network interface
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    // Initialize WiFi in station mode (required for ESP-NOW)
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_start());

    // Print MAC address
    uint8_t mac[6];
    esp_wifi_get_mac(WIFI_IF_STA, mac);
    ESP_LOGI(TAG, "MAC Address: %02X:%02X:%02X:%02X:%02X:%02X",
             mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);

    // Initialize ESP-NOW
    if (esp_now_init() != ESP_OK) {
        ESP_LOGE(TAG, "ESP-NOW init failed");
        return false;
    }

    // Register callbacks
    esp_now_register_recv_cb(espnow_recv_cb);
    esp_now_register_send_cb(espnow_send_cb);

    ESP_LOGI(TAG, "ESP-NOW initialized successfully");
    return true;
}

bool esp_comm_send(const uint8_t *peer_mac, const uint8_t *data, size_t len)
{
    if (len > ESP_COMM_MAX_DATA_LEN) {
        ESP_LOGE(TAG, "Data length exceeds maximum");
        return false;
    }

    // If peer_mac is NULL, use broadcast address
    uint8_t broadcast_mac[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
    const uint8_t *target_mac = peer_mac ? peer_mac : broadcast_mac;

    // Check if peer is already added, if not add it
    if (!esp_now_is_peer_exist(target_mac)) {
        esp_now_peer_info_t peer_info = {};
        memcpy(peer_info.peer_addr, target_mac, 6);
        peer_info.channel = 0;  // Use current channel
        peer_info.ifidx = WIFI_IF_STA;
        peer_info.encrypt = false;

        if (esp_now_add_peer(&peer_info) != ESP_OK) {
            ESP_LOGE(TAG, "Failed to add peer");
            return false;
        }
    }

    // Send data
    esp_err_t result = esp_now_send(target_mac, data, len);
    if (result != ESP_OK) {
        ESP_LOGE(TAG, "Send error: %d", result);
        return false;
    }

    return true;
}

void esp_comm_register_recv_cb(esp_comm_recv_cb_t cb)
{
    recv_callback = cb;
}
