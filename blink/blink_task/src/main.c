#include <stdio.h>
#include "driver/gpio.h"
#include "esp_err.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

TaskHandle_t myTaskHandle = NULL;

#define STACK_SIZE 1024
#define LED_PIN GPIO_NUM_2
#define BLINK_TIME 1000

esp_err_t init_led(void);
void vTaskLed(void *pvParameters);

void app_main()
{
    init_led();
    xTaskCreate(&vTaskLed, "vTaskLed", STACK_SIZE, NULL, 1, &myTaskHandle);
    vTaskDelay(pdMS_TO_TICKS(100));
}
esp_err_t init_led()
{
    gpio_pad_select_gpio(LED_PIN);
    gpio_reset_pin(LED_PIN);
    gpio_set_direction(LED_PIN, GPIO_MODE_OUTPUT);
    return ESP_OK;
}

void vTaskLed(void *pvParameters)
{
    gpio_set_level(LED_PIN, 0);
    vTaskDelay(pdMS_TO_TICKS(BLINK_TIME));
    gpio_set_level(LED_PIN, 1);
    vTaskDelay(pdMS_TO_TICKS(BLINK_TIME));
}