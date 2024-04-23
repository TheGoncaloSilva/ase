/*
 * SPDX-FileCopyrightText: 2010-2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_err.h"
#include "driver/i2c_master.h"

#define SDA_PIN 0
#define SCL_PIN 1
#define DEVICE_ADDRESS 0x49

void app_main(void)
{
    i2c_master_bus_config_t i2cMasterCfg = {
        .clk_source = I2C_CLK_SRC_DEFAULT,
        .i2c_port = I2C_NUM_0,
        .sda_io_num = SDA_PIN,
        .scl_io_num = SCL_PIN,
        .glitch_ignore_cnt = 7,
        .flags.enable_internal_pullup = true,
    };

    i2c_master_bus_handle_t i2cBusHandle;
    ESP_ERROR_CHECK(i2c_new_master_bus(&i2cMasterCfg, &i2cBusHandle));

    i2c_device_config_t i2cDevCfg = {
        .dev_addr_length = I2C_ADDR_BIT_LEN_7,
        .device_address = DEVICE_ADDRESS,
        .scl_speed_hz = 50000,
    };

    i2c_master_dev_handle_t i2cDevHandle;
    ESP_ERROR_CHECK(i2c_master_bus_add_device(i2cBusHandle, &i2cDevCfg, &i2cDevHandle));

    while(1){
        uint8_t buffer[2] = {0x01, 0x80};

        ESP_ERROR_CHECK(i2c_master_transmit(i2cDevHandle, buffer, sizeof(buffer), -1));
        vTaskDelay(10);
    }

    /*
    while (1)
    {
        uint8_t buffer[2] = {0x01, 0x00};
        ESP_ERROR_CHECK(i2c_master_transmit(i2cDevHandle, buffer, sizeof(buffer), -1));
        vTaskDelay(10);
    }

    uint8_t buffer[2] = {0x01, 0x00};
    ESP_ERROR_CHECK(i2c_master_transmit(i2cDevHandle, buffer, sizeof(buffer), -1));
 
    while(1)
    {
        uint8_t txBuf[1] = {0x00};
        uint8_t rxBuf[1];
    
        ESP_ERROR_CHECK(i2c_master_transmit_receive(i2cDevHandle, txBuf, sizeof(txBuf),
                                                                rxBuf, sizeof(rxBuf), -1));
        vTaskDelay(10);
    }
    */
}
