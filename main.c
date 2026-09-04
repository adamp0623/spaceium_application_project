#include <stdio.h>
#include "driver/gpio.h"

#include "battery_monitor.h"


#define CORE_THREAD_FREQUENCY_HZ 1

void core_init(void);

void core_init(void) {
    vTaskDelay(200); 

    // Initialize the battery monitor
    battery_monitor_init();
}

void app_main(void)
{
    core_init();

    TickType_t xLastWakeTime = xTaskGetTickCount();
    const TickType_t xDelay_Ticks = pdMS_TO_TICKS((uint32_t)(1000.0 / CORE_THREAD_FREQUENCY_HZ));

    while(1) {

        vTaskDelayUntil(&xLastWakeTime, xDelay_Ticks); 
    }
}
