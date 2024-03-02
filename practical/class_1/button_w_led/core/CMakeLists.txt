#idf_component_register(SRCS "main.c"
#                    INCLUDE_DIRS ".")
set(SOURCES ./Src/main.c)

idf_component_register(SRCS ${SOURCES}
                        INCLUDE_DIRS . Include
                        REQUIRES freertos 
                                    esp_common 
                                    esp_event 
                                    nvs_flash 
                                    )