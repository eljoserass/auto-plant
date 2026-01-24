#include <stdio.h>
#include "soil-moisture.h"
#include "hardware-config.h"
#include "driver/gpio.h"
#include "driver/adc.h"
#include "esp_adc/adc_oneshot.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

void sensing(void)
{
    // init sensor system
    sensor_group_t my_sensor_grup = {
        .sensors = {{"A0", -1, 100, SENSOR_A0, 0}, {"A1", -1, 100, SENSOR_A1, 0}},
        .power_pin = GROUP_A_POWER_PIN,
        .last_read_time = 0
    };

    // Configure power pin as output
    gpio_config_t io_conf = {
        .pin_bit_mask = (1ULL << my_sensor_grup.power_pin),
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE
    };
    gpio_config(&io_conf);

    // Set power pin high to enable sensors
    gpio_set_level(my_sensor_grup.power_pin, 1);

    // Initialize ADC for analog sensor reading
    adc_oneshot_unit_handle_t adc1_handle;
    adc_oneshot_unit_init_cfg_t init_config = {
        .unit_id = ADC_UNIT_1,
    };
    adc_oneshot_new_unit(&init_config, &adc1_handle);

    // Configure ADC channels for sensors (GPIO13=ADC1_CH4, GPIO14=ADC1_CH5)
    adc_oneshot_chan_cfg_t config = {
        .bitwidth = ADC_BITWIDTH_DEFAULT,
        .atten = ADC_ATTEN_DB_11,  // Full scale voltage 3.3V
    };
    
    // Configure channel for SENSOR_A0 (GPIO13 = ADC1_CHANNEL_4)
    adc_oneshot_config_channel(adc1_handle, ADC_CHANNEL_4, &config);
    
    // Configure channel for SENSOR_A1 (GPIO14 = ADC1_CHANNEL_5)
    adc_oneshot_config_channel(adc1_handle, ADC_CHANNEL_5, &config);

    printf("Soil moisture sensors initialized\n");

    // Continuous reading loop for debugging
    while (1) {
        int adc_raw_a0, adc_raw_a1;
        
        // Read sensor A0
        adc_oneshot_read(adc1_handle, ADC_CHANNEL_4, &adc_raw_a0);
        
        // Read sensor A1
        adc_oneshot_read(adc1_handle, ADC_CHANNEL_5, &adc_raw_a1);
        
        printf("Sensor A0 (GPIO13): %d | Sensor A1 (GPIO14): %d\n", adc_raw_a0, adc_raw_a1);
        
        // Wait 1 second before next reading
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
