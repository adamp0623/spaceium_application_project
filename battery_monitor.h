#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_adc/adc_oneshot.h"
#include "esp_adc/adc_cali.h"
#include "esp_adc/adc_cali_scheme.h"
#include "driver/gpio.h"

#ifndef BATTERY_MONITOR_H_
#define BATTERY_MONITOR_H_

#define LED_RED GPIO_NUM_5
#define LED_GREEN GPIO_NUM_6

#define BATTERY_MONITOR_THREAD_FREQUENCY_HZ 20

#define LOG_TAG "BATT_MONITOR"

// ADC attenuation and channel configuration
#define ADC_CHANNEL     ADC_CHANNEL_0
// This attenuation can measure 0 - 1050 mV
#define ADC_ATTEN       ADC_ATTEN_DB_2_5
#define ADC_CONVERSION_FACTOR 4.5f

void battery_monitor_init(void);

#endif