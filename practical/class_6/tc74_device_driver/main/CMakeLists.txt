idf_component_register(SRCS "main.c"
                    INCLUDE_DIRS .
                    REQUIRES freertos 
                            driver
                            esp_common 
                            esp_event 
                            esp_timer
                            nvs_flash 
                            TempSensorTC74)
