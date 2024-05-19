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
#include "esp_timer.h"
#include "esp_log.h"
#include "esp_sleep.h"
#include "driver/gpio.h"
#include "TempSensorTC74.h"

/*
    ##########################################################################
    ############                  Definitions                     ############
    ##########################################################################
*/

// Pin definitions for 7-segment display
#define PIN_A GPIO_NUM_5
#define PIN_B GPIO_NUM_4
#define PIN_C GPIO_NUM_6
#define PIN_D GPIO_NUM_7
#define PIN_E GPIO_NUM_8
#define PIN_F GPIO_NUM_3
#define PIN_G GPIO_NUM_2

// Pins for MOSFET control
#define PIN_MOSFET_DSP GPIO_NUM_0

// SPI configurations
#define SDA_PIN GPIO_NUM_1
#define SCL_PIN GPIO_NUM_10
#define DEVICE_ADDRESS 0x49
#define SCL_CLK_SPEED 50000
#define TERMINAL_REFRESH_PERIOD_MS 100000   // 100 ms
#define DISPLAY_REFRESH_PERIOD_MS 10000 // 10 ms

/*
    ##########################################################################
    ############                    Variables                     ############
    ##########################################################################
*/

// Define 7-segment display combinations to represent values
const uint8_t digitMap[16] = {
    //PGFEDCBA
    0b00111111,  // 0
    0b00000110,  // 1
    0b01011011,  // 2
    0b01001111,  // 3
    0b01100110,  // 4
    0b01101101,  // 5
    0b01111101,  // 6
    0b00000111,  // 7
    0b01111111,  // 8
    0b01101111,  // 9
    0b01110111,  // A
    0b01111100,  // b
    0b00001111,  // C
    0b01011110,  // D
    0b01111001,  // E
    0b01110001   // F

};

// I2C device TC74 handle
i2c_master_dev_handle_t i2cDevHandle;

// Temperature variable
uint8_t pTemp;

/*
    ##########################################################################
    ############               Function Definitions               ############
    ##########################################################################
*/
esp_err_t setup(void);
static void terminal_temp_callback(void* arg);
void refresh_display_timer_callback(void* arg);
void display_Digit(uint8_t digit, int mosfetVal);


/*
    ##########################################################################
    ############              Functions declaration               ############
    ##########################################################################
*/
void app_main(void)
{
    printf("Hello world!\n");

    ESP_ERROR_CHECK(setup());

    i2c_master_bus_handle_t i2cBusHandle;
    tc74_init(&i2cBusHandle, &i2cDevHandle, DEVICE_ADDRESS, SDA_PIN, SCL_PIN, SCL_CLK_SPEED);

    tc74_standy(i2cDevHandle);

    // Create terminal periodic timer
    const esp_timer_create_args_t periodic_timer_args = {
            .callback = &terminal_temp_callback,
            /* name is optional, but may help identify the timer when debugging */
            .name = "periodic_terminal_temp_timer"
    };
    esp_timer_handle_t periodic_timer;
    ESP_ERROR_CHECK(esp_timer_create(&periodic_timer_args, &periodic_timer));

    // Create display refresh timer
    esp_timer_create_args_t refreshDisplayTimerArgs = {
        .callback = &refresh_display_timer_callback,
        .name = "periodic_display_temp_timer"
    };
    esp_timer_handle_t refreshDisplayTimer;
    ESP_ERROR_CHECK(esp_timer_create(&refreshDisplayTimerArgs, &refreshDisplayTimer));

    /* Start the timers */
    ESP_ERROR_CHECK(esp_timer_start_periodic(periodic_timer, TERMINAL_REFRESH_PERIOD_MS));  // in microseconds
    ESP_ERROR_CHECK(esp_timer_start_periodic(refreshDisplayTimer, DISPLAY_REFRESH_PERIOD_MS));  // in microseconds
    printf("Started timers, time since boot: %lld us\n", esp_timer_get_time());

    tc74_wakeup_and_read_temp(i2cDevHandle, &pTemp);
    while(1){
        tc74_read_temp_after_temp(i2cDevHandle, &pTemp);
        vTaskDelay(10);
    }

    tc74_free(i2cBusHandle, i2cDevHandle);
    esp_restart();

}

static void terminal_temp_callback(void* arg)
{
    //static int lastTemp = 0;
    //if(lastTemp != pTemp) {
        printf("\rTemperature: %d", pTemp);
        //lastTemp = pTemp;
    //}
}

// Function to refresh display
void refresh_display_timer_callback(void* arg) {
    static int displayFlag = 0;
    if(displayFlag == 0)
        display_Digit(pTemp % 16, 1);
    else
        display_Digit((pTemp >> 4) % 16, 0);
        
    displayFlag = !displayFlag;
}

// Function to display a digit on a 7-segment display
void display_Digit(uint8_t digit, int mosfetVal) {
  gpio_set_level(PIN_MOSFET_DSP, mosfetVal);

  gpio_set_level(PIN_A, digitMap[digit] & (1 << 0));
  gpio_set_level(PIN_B, digitMap[digit] & (1 << 1));
  gpio_set_level(PIN_C, digitMap[digit] & (1 << 2));
  gpio_set_level(PIN_D, digitMap[digit] & (1 << 3));
  gpio_set_level(PIN_E, digitMap[digit] & (1 << 4));
  gpio_set_level(PIN_F, digitMap[digit] & (1 << 5));
  gpio_set_level(PIN_G, digitMap[digit] & (1 << 6));
}

esp_err_t setup(void)
{
    esp_err_t status = ESP_OK;

    printf("Setup!\n");

    // Configure GPIO pins
    gpio_config_t io_conf;
    io_conf.intr_type = GPIO_INTR_DISABLE;
    io_conf.mode = GPIO_MODE_OUTPUT;
    io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
    io_conf.pull_up_en = GPIO_PULLUP_DISABLE;
    io_conf.pin_bit_mask = (1ULL << PIN_A) |
                            (1ULL << PIN_B) |
                            (1ULL << PIN_C) |
                            (1ULL << PIN_D) |
                            (1ULL << PIN_E) |
                            (1ULL << PIN_F) |
                            (1ULL << PIN_G) |
                            (1ULL << PIN_MOSFET_DSP);
    status |= gpio_config(&io_conf);

    return status;
}