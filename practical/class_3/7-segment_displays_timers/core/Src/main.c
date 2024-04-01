#include <stdio.h>
#include "main.h"

#define LOG_LEVEL_LOCAL ESP_LOG_VERBOSE
#include "esp_log.h"
#define LOG_TAG "MAIN"

// Timer handles
esp_event_handler_t incrementCounterTimer;
esp_timer_handle_t refreshDisplayTimer;

// Global counter variable
volatile char counter = 0;

// Function to display a digit on a 7-segment display
void displayDigit(uint8_t digit, gpio_num_t mosfetPin) {
  gpio_set_level(PIN_MOSFET_DSP_1, mosfetPin == PIN_MOSFET_DSP_1 ? 1 : 0);
  gpio_set_level(PIN_MOSFET_DSP_2, mosfetPin == PIN_MOSFET_DSP_2 ? 1 : 0);

  gpio_set_level(PIN_A, digitMap[digit] & (1 << 0));
  gpio_set_level(PIN_B, digitMap[digit] & (1 << 1));
  gpio_set_level(PIN_C, digitMap[digit] & (1 << 2));
  gpio_set_level(PIN_D, digitMap[digit] & (1 << 3));
  gpio_set_level(PIN_E, digitMap[digit] & (1 << 4));
  gpio_set_level(PIN_F, digitMap[digit] & (1 << 5));
  gpio_set_level(PIN_G, digitMap[digit] & (1 << 6));
  //gpio_set_level(PIN_DP, digitMap[digit] & (1 << 7));
  gpio_set_level(PIN_DP, 0);
}

esp_err_t setup(void)
{
    esp_err_t status = ESP_OK;

    ESP_LOGI(LOG_TAG, "Setup!");

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
                            (1ULL << PIN_DP) |
                            (1ULL << PIN_MOSFET_DSP_1) |
                            (1ULL << PIN_MOSFET_DSP_2);
    status |= gpio_config(&io_conf);

    // Initial state
    status |= gpio_set_level(PIN_MOSFET_DSP_1, 0);
    status |= gpio_set_level(PIN_MOSFET_DSP_2, 0);

    return status;
}

// Function to increment counter
void incrementCounterTimerCallback(void* arg) {
    counter = (counter + 1) % MAX_COUNTER_VALUE;
    ESP_LOGI(LOG_TAG, "Counter: %d", counter);
}

// Function to refresh display
void refreshDisplayTimerCallback(void* arg) {
    static int displayFlag = 0;
    if(displayFlag == 0)
        displayDigit(counter % 16, PIN_MOSFET_DSP_2);
    else
        displayDigit((counter >> 4) % 16, PIN_MOSFET_DSP_1);
        
    displayFlag = !displayFlag;
}

void app_main(void)
{
    ESP_LOGI(LOG_TAG, "Creating default event loop");
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    ESP_LOGI(LOG_TAG, "Initialising NVS");
    ESP_ERROR_CHECK(nvs_flash_init());

    ESP_ERROR_CHECK(setup());

    // Create timers
    esp_timer_create_args_t incrementCounterTimerArgs = {
        .callback = &incrementCounterTimerCallback,
        .name = "Increment Counter Timer"
    };
    ESP_ERROR_CHECK(esp_timer_create(&incrementCounterTimerArgs, &incrementCounterTimer));
    ESP_ERROR_CHECK(esp_timer_start_periodic(incrementCounterTimer, 1000000)); // 1 second

    esp_timer_create_args_t refreshDisplayTimerArgs = {
        .callback = &refreshDisplayTimerCallback,
        .name = "Refresh Display Timer"
    };
    ESP_ERROR_CHECK(esp_timer_create(&refreshDisplayTimerArgs, &refreshDisplayTimer));
    ESP_ERROR_CHECK(esp_timer_start_periodic(refreshDisplayTimer, 10000)); // 10ms
}