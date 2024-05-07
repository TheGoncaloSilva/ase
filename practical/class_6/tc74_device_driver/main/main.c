/*
 * SPDX-FileCopyrightText: 2010-2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */

#include <stdio.h>
#include <inttypes.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_chip_info.h"
#include "esp_flash.h"
#include "esp_system.h"
#include <TempSensorTC74.h>

#define SDA_PIN 0
#define SCL_PIN 1
#define DEVICE_ADDRESS 0x49
#define SCL_CLK_SPEED 50000

void app_main(void)
{
    printf("Hello world!\n");

    i2c_master_bus_handle_t i2cBusHandle;
    i2c_master_dev_handle_t i2cDevHandle;
    tc74_init(&i2cBusHandle, &i2cDevHandle, DEVICE_ADDRESS, SDA_PIN, SCL_PIN, SCL_CLK_SPEED);

    tc74_standy(&i2cDevHandle);

    uint8_t pTemp;
    tc74_wakeup_and_read_temp(&i2cDevHandle, pTemp);
    while(1){
        tc74_read_temp_after_temp(&i2cDevHandle, pTemp);
    }

    tc74_free(i2cBusHandle, i2cDevHandle);
    esp_restart();


}
