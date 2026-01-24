#include <stdio.h>
#include "driver/gpio.h"
#include "pumping.h"

#define PUMP_GPIO 23

void pumping(void)
{
    printf("Setting GPIO 23 HIGH\n");
    
    // Configure GPIO
    gpio_config_t io_conf = {
        .pin_bit_mask = (1ULL << PUMP_GPIO),
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE
    };
    gpio_config(&io_conf);
    
    // Set GPIO 23 HIGH
    gpio_set_level(PUMP_GPIO, 1);
}