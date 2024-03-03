#include <stdio.h>
#include "main.h"

#define LOG_LEVEL_LOCAL ESP_LOG_VERBOSE
#include "esp_log.h"
#define LOG_TAG "MAIN"

// Program defines
#define LED_GPIO 9
#define BUTTON_GPIO 6

// Program Global variables
GpioController led;
GpioController button;

// Function Definitions
esp_err_t setup(void);
void loop(void);

void app_main(void)
{
    ESP_LOGI(LOG_TAG, "Creating default event loop");
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    ESP_LOGI(LOG_TAG, "Initialising NVS");
    ESP_ERROR_CHECK(nvs_flash_init());

    ESP_ERROR_CHECK(setup());

    while (true)
    {
        loop();
        vTaskDelay(110 / portTICK_PERIOD_MS); // 100ms delay
    }

}

esp_err_t setup(void)
{
    esp_err_t status = ESP_OK;

    ESP_LOGI(LOG_TAG, "Setup!");

    /* Setup LED */
    gpio_config_t led_config = {
        .pin_bit_mask = (1ULL << LED_GPIO),
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_ENABLE,
        .intr_type = GPIO_INTR_DISABLE
    };

    // Initialize the base members of GpioController
    led.base.pin = LED_GPIO;
    led.base.config = led_config;
    led.base.inverted_logic = false;
    
    // Initialize additional members specific to GpioController, if any
    led.state = 0;

    status |= GpioController_init(&led);

    GpioController_set(&led, false);

    /* Setup Button */
    gpio_config_t button_config = {
        .pin_bit_mask = (1ULL << BUTTON_GPIO),
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_ENABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE
    };

    // Initialize the base members of GpioController
    button.base.pin = BUTTON_GPIO;
    button.base.config = button_config;
    button.base.inverted_logic = false;
    
    // Initialize additional members specific to GpioController, if any
    led.state = 0;

    status |= GpioController_init(&button);

    return status;
}

void loop(void)
{
    if(GpioController_state(&button))
    {
        ESP_LOGI(LOG_TAG, "Button pressed!");
        if(GpioController_state(&led))
        {
            ESP_LOGI(LOG_TAG, "LED off");
            GpioController_set(&led, false);
        }
        else
        {
            ESP_LOGI(LOG_TAG, "LED on");
            GpioController_set(&led, true);
        }
    }
}