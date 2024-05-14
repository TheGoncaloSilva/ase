#ifndef __TEMP_SENSOR_TC74_H__INCLUDED__
#define __TEMP_SENSOR_TC74_H__INCLUDED__

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_err.h"
#include "driver/i2c_master.h"
//#include "driver/i2c.h"

#define TC74_READ_REGISTER         	0x00
#define TC74_CONFIG_REGISTER        0x01
#define TC74_STANDBY				0x80
#define TC74_READ_READY_BIT			0x40
#define TC74_WAKEUP          		0x00

esp_err_t tc74_init(i2c_master_bus_handle_t* pBusHandle,
					i2c_master_dev_handle_t* pSensorHandle,
					uint8_t sensorAddr, int sdaPin, int sclPin, uint32_t clkSpeedHz);

esp_err_t tc74_free(i2c_master_bus_handle_t busHandle,
					 i2c_master_dev_handle_t sensorHandle);

esp_err_t tc74_standy(i2c_master_dev_handle_t sensorHandle);

esp_err_t tc74_wakeup(i2c_master_dev_handle_t sensorHandle);

bool tc74_is_temperature_ready(i2c_master_dev_handle_t sensorHandle);

esp_err_t tc74_wakeup_and_read_temp(i2c_master_dev_handle_t sensorHandle, uint8_t* pTemp);

esp_err_t tc74_read_temp_after_cfg(i2c_master_dev_handle_t sensorHandle, uint8_t* pTemp);

esp_err_t tc74_read_temp_after_temp(i2c_master_dev_handle_t sensorHandle, uint8_t* pTemp);

#endif // __TEMP_SENSOR_TC74_H__INCLUDED__
