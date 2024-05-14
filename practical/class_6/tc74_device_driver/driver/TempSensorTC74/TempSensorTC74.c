#include "TempSensorTC74.h"

esp_err_t tc74_init(i2c_master_bus_handle_t* pBusHandle,
					i2c_master_dev_handle_t* pSensorHandle,
					uint8_t sensorAddr, int sdaPin, int sclPin, uint32_t clkSpeedHz){
                        
    i2c_master_bus_config_t i2cMasterCfg = {
        .clk_source = I2C_CLK_SRC_DEFAULT,
        .i2c_port = I2C_NUM_0,
        .sda_io_num = sdaPin,
        .scl_io_num = sclPin,
        .glitch_ignore_cnt = 7,
        .flags.enable_internal_pullup = true,
    };

    esp_err_t ret = i2c_new_master_bus(&i2cMasterCfg, pBusHandle);
    if (ret != ESP_OK)
        return ret;
    
    i2c_device_config_t i2cDevCfg = {
        .dev_addr_length = I2C_ADDR_BIT_LEN_7,
        .device_address = sensorAddr,
        .scl_speed_hz = clkSpeedHz,
    };

    return i2c_master_bus_add_device(*pBusHandle, &i2cDevCfg, pSensorHandle);
}

esp_err_t tc74_free(i2c_master_bus_handle_t busHandle,
					 i2c_master_dev_handle_t sensorHandle){
    esp_err_t ret = i2c_del_master_bus(busHandle);
    if (ret != ESP_OK)
        return ret;

    return i2c_master_bus_rm_device(sensorHandle);
}

esp_err_t tc74_standy(i2c_master_dev_handle_t sensorHandle){
    // Each value has 8 bits and it's big-endian
    uint8_t buffer[2] = {TC74_CONFIG_REGISTER, TC74_STANDBY};
    return i2c_master_transmit(sensorHandle, buffer, sizeof(buffer), -1);
}

esp_err_t tc74_wakeup(i2c_master_dev_handle_t sensorHandle){
    uint8_t buffer[2] = {TC74_CONFIG_REGISTER, TC74_WAKEUP};
    return i2c_master_transmit(sensorHandle, buffer, sizeof(buffer), -1);
}

bool tc74_is_temperature_ready(i2c_master_dev_handle_t sensorHandle){
    // Read the config register
    uint8_t configReg[1] = {TC74_CONFIG_REGISTER};
    uint8_t buffer = 0;
    esp_err_t ret = i2c_master_transmit_receive(sensorHandle, configReg, sizeof(configReg), &buffer, sizeof(buffer), -1);
    if(ret != ESP_OK)
        return false;

    // Check if the read ready bit is set
    return buffer & TC74_READ_READY_BIT;
}

esp_err_t tc74_wakeup_and_read_temp(i2c_master_dev_handle_t sensorHandle, uint8_t* pTemp){
    esp_err_t ret = tc74_wakeup(sensorHandle);
    if (ret != ESP_OK)
        return ret;

    while(!tc74_is_temperature_ready(sensorHandle))
        vTaskDelay(10);

    return tc74_read_temp_after_cfg(sensorHandle, pTemp);
}

esp_err_t tc74_read_temp_after_cfg(i2c_master_dev_handle_t sensorHandle, uint8_t* pTemp){
    // Read the temperature register
    uint8_t tempReg[1] = {TC74_READ_REGISTER};
    return i2c_master_transmit_receive(sensorHandle, tempReg, sizeof(tempReg), pTemp, sizeof(*pTemp), -1);
}

esp_err_t tc74_read_temp_after_temp(i2c_master_dev_handle_t sensorHandle, uint8_t* pTemp){
    return i2c_master_receive(sensorHandle, pTemp, sizeof(*pTemp), -1);
}