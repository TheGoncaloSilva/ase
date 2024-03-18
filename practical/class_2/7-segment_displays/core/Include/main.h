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

/*
// Pin definitions for 7-segment display
#define PIN_A GPIO_NUM_1
#define PIN_B GPIO_NUM_2
#define PIN_C GPIO_NUM_5
#define PIN_D GPIO_NUM_7
#define PIN_E GPIO_NUM_8
#define PIN_F GPIO_NUM_3
#define PIN_G GPIO_NUM_2
#define PIN_DP GPIO_NUM_6

// Pins for MOSFET control
#define PIN_MOSFET_DSP_1 GPIO_NUM_10
#define PIN_MOSFET_DSP_2 GPIO_NUM_4

// Function Definitions
esp_err_t setup(void);
void loop(void);
void displayDigit(uint8_t digit, gpio_num_t mosfetPin);

// Define 7-segment display combinations to represent values
const uint8_t digitMap[10] = {
  0b11111100,  // 0
  0b01100000,  // 1
  0b11011010,  // 2
  0b11110010,  // 3
  0b01100110,  // 4
  0b10110110,  // 5
  0b10111110,  // 6
  0b11100000,  // 7
  0b11111110,  // 8
  0b11110110   // 9
};*/