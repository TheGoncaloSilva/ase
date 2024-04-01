#pragma once

#ifndef GPIO_H
#define GPIO_H

#include <stdint.h>
#include "driver/gpio.h"

typedef struct {
    gpio_num_t pin;
    gpio_config_t config;
    int inverted_logic;
} GpioBase;

esp_err_t GpioBase_init(GpioBase *gpio);

typedef struct {
    GpioBase base;
    int state;
} GpioController;

esp_err_t GpioController_init(GpioController *gpio);

esp_err_t GpioController_set(GpioController *gpio, int state);

int GpioController_state(GpioController *gpio);

#endif /* GPIO_H */
