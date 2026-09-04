#include "battery_monitor.h"

static adc_oneshot_unit_handle_t s_adc_handle = NULL;
static adc_cali_handle_t         s_cali_handle = NULL;

void battery_monitor_init();
void get_battery_voltage(float *voltage);
void BatteryMonitorLogging_Task(void *argument);

void battery_monitor_init() {
    // Create a FreeRTOS task for battery monitoring logging
    xTaskCreate(BatteryMonitorLogging_Task, "BatteryMonitorLogging_Task", 4096, NULL, 5, NULL);
    
    gpio_config_t io_conf = {
        .pin_bit_mask = (1ULL << LED_RED) | (1ULL << LED_GREEN),
        .mode = GPIO_MODE_INPUT_OUTPUT_OD,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE
    };
    gpio_config(&io_conf);

    // initialise ADC unit once
    adc_oneshot_unit_init_cfg_t unit_cfg = {
        .unit_id  = ADC_UNIT_1,
        .ulp_mode = ADC_ULP_MODE_DISABLE,
    };
    ESP_ERROR_CHECK(adc_oneshot_new_unit(&unit_cfg, &s_adc_handle));

    // configure the channel
    adc_oneshot_chan_cfg_t chan_cfg = {
        .atten    = ADC_ATTEN,
        .bitwidth = ADC_BITWIDTH_DEFAULT,
    };
    ESP_ERROR_CHECK(adc_oneshot_config_channel(s_adc_handle, ADC_CHANNEL, &chan_cfg));

    // calibration
    adc_cali_curve_fitting_config_t cali_cfg = {
        .unit_id  = ADC_UNIT_1,
        .chan     = ADC_CHANNEL,
        .atten    = ADC_ATTEN,
        .bitwidth = ADC_BITWIDTH_DEFAULT,
    };
    esp_err_t ret = adc_cali_create_scheme_curve_fitting(&cali_cfg, &s_cali_handle);
    if (ret != ESP_OK) {
        ESP_LOGW(LOG_TAG, "Calibration not available, raw values only");
        s_cali_handle = NULL;
    } else {
        ESP_LOGI(LOG_TAG, "ADC calibration initialized");
    }


}

void get_battery_voltage(float *voltage) {
    if (s_adc_handle == NULL) {
        ESP_LOGE(LOG_TAG, "ADC not initialized");
        *voltage = 0.0f;
        return;
    }

    int raw = 0;
    ESP_ERROR_CHECK(adc_oneshot_read(s_adc_handle, ADC_CHANNEL, &raw));

    int voltage_mv = 0;
    if (s_cali_handle) {
        adc_cali_raw_to_voltage(s_cali_handle, raw, &voltage_mv);
    }

    float rail_voltage = ((float)voltage_mv * ADC_CONVERSION_FACTOR) / 1000.0f;
    //ESP_LOGI(LOG_TAG, "Raw: %d  ADC Voltage: %d mV  Rail Voltage: %.3f V",
    //         raw, voltage_mv, rail_voltage);

    *voltage = rail_voltage;
}

void BatteryMonitorLogging_Task(void *argument) {
    TickType_t xLastWakeTime = xTaskGetTickCount();
    const TickType_t xDelay_Ticks = pdMS_TO_TICKS((uint32_t)(1000.0 / BATTERY_MONITOR_THREAD_FREQUENCY_HZ));
\
    while(1) {

        float voltage = 0.0f;
        get_battery_voltage(&voltage);


        if(voltage < 3.7f) {
            gpio_set_level(LED_RED, 0);
            gpio_set_level(LED_GREEN, 1);
            ESP_LOGE(LOG_TAG, "UNDERVOLTAGE EVENT: %.3f V", voltage);
        } else if (voltage > 3.7f && voltage < 4.2f) {
            gpio_set_level(LED_RED, 1);
            gpio_set_level(LED_GREEN, 0);
            ESP_LOGI(LOG_TAG, "Battery Voltage: %.3f V", voltage);
        } else if (voltage > 4.2f) {
            gpio_set_level(LED_RED, 0);
            gpio_set_level(LED_GREEN, 1);
            ESP_LOGE(LOG_TAG, "OVERVOLTAGE EVENT: %.3f V", voltage);
        }
        vTaskDelayUntil(&xLastWakeTime, xDelay_Ticks);
    }
}