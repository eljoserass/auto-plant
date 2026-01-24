#ifndef ESP_COMM_H
#define ESP_COMM_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define ESP_COMM_MAX_DATA_LEN 250

/**
 * @brief Initialize ESP communication (ESP-NOW)
 * 
 * @return true on success, false on failure
 */
bool esp_comm_init(void);

/**
 * @brief Send data to a peer
 * 
 * @param peer_mac MAC address of the peer (NULL for broadcast)
 * @param data Data to send
 * @param len Length of data
 * @return true on success, false on failure
 */
bool esp_comm_send(const uint8_t *peer_mac, const uint8_t *data, size_t len);

/**
 * @brief Callback function type for received data
 * 
 * @param mac_addr MAC address of sender
 * @param data Received data
 * @param len Length of received data
 */
typedef void (*esp_comm_recv_cb_t)(const uint8_t *mac_addr, const uint8_t *data, int len);

/**
 * @brief Register callback for received data
 * 
 * @param cb Callback function
 */
void esp_comm_register_recv_cb(esp_comm_recv_cb_t cb);

#endif // ESP_COMM_H
