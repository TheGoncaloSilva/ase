#pragma once

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#define pdSECOND pdMS_TO_TICKS(1000)

#include "esp_event.h"
#include "nvs_flash.h"

#include "Gpio.h"

/*
    ##########################################################################
    ############                  Definitions                     ############
    ##########################################################################
*/

// Pin definitions for 7-segment display
#define PIN_A GPIO_NUM_1
#define PIN_B GPIO_NUM_0
#define PIN_C GPIO_NUM_6
#define PIN_D GPIO_NUM_7
#define PIN_E GPIO_NUM_8
#define PIN_F GPIO_NUM_3
#define PIN_G GPIO_NUM_2
#define PIN_DP GPIO_NUM_5

// Pins for MOSFET control
#define PIN_MOSFET_DSP_1 GPIO_NUM_10
#define PIN_MOSFET_DSP_2 GPIO_NUM_4

// Maximum counter value
#define MAX_COUNTER_VALUE 256;

/*
    ##########################################################################
    ############               Function Definitions               ############
    ##########################################################################
*/

// Function Definitions
esp_err_t setup(void);
void displayDigit(uint8_t digit, gpio_num_t mosfetPin);
void incrementCounterTask(void *pvParameters);
void refreshDisplayTask(void *pvParameters);

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
    0b01111100,  // B
    0b00111001,  // C
    0b01011110,  // D
    0b01111001,  // E
    0b01110001   // F

};

// Global counter variable
volatile char counter = 0;