#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "pumping.h"
#include "soil-moisture.h"

void app_main(void)
{
    printf("Starting sensing-pumping application...\n");

    sensing();
    pumping();
}
