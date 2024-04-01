#include "Gpio.h"

esp_err_t GpioBase_init(GpioBase *gpio) {
    esp_err_t status = ESP_OK;

    status |= gpio_config(&(gpio->config));

    return status;
}

esp_err_t GpioController_init(GpioController *gpio) {
    esp_err_t status = GpioBase_init(&(gpio->base));

    if (ESP_OK == status) {
        status |= GpioController_set(gpio, gpio->base.inverted_logic);
    }

    return status;
}

esp_err_t GpioController_set(GpioController *gpio, int state) {
    gpio->state = state;

    return gpio_set_level(gpio->base.pin,
                          gpio->base.inverted_logic ? 
                            !state : 
                            state);
}

int GpioController_state(GpioController *gpio) {
    if(gpio->base.config.mode == GPIO_MODE_INPUT)
        gpio->state = gpio_get_level(gpio->base.pin);
    
    return gpio->state;
}
