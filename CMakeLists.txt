idf_component_register(SRCS "main.c"
                            "battery_monitor.c"
                    INCLUDE_DIRS "."
                    REQUIRES esp_common esp_adc driver)
