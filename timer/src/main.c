/**
 * @file main.c
 * @author Cristobal Cuevas
 * @date 24-04-2023
 * @brief Timer incorporated in freertos
 * @see
 */

#include <stdio.h>
#include "esp_err.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/timers.h"

#define LED_PIN GPIO_NUM_2

TimerHandle_t xTimers;

uint8_t led_value = 0;
uint8_t interval = 1000;

esp_err_t init_led(void);
esp_err_t blink_led(void);
void vTimerCallback(TimerHandle_t pxTimer);
esp_err_t set_timer(void);

void app_main()
{
    init_led();
    set_timer();
}

esp_err_t init_led()
{
    gpio_reset_pin(LED_PIN);
    gpio_set_direction(LED_PIN, GPIO_MODE_OUTPUT);
    return ESP_OK;
}
esp_err_t blink_led()
{
    led_value = !led_value;
    gpio_set_level(LED_PIN, led_value);
    return ESP_OK;
}
void vTimerCallback(TimerHandle_t pxTimer)
{
    printf("Fui llamada por el timer");
    blink_led();
}
esp_err_t set_timer(void)
{
    printf("Configuracion de timer iniciada");
    xTimers = xTimerCreate("Timer", (pdMS_TO_TICKS(interval)), true, NULL, vTimerCallback);
    if (xTimers == NULL)
    {
        printf("El timer no fue creado");
    }
    else
    {
        if (xTimerStart(xTimers, 0) != pdPASS)
        {
            printf("El timer no paso a estado activo");
        }
    }
    return ESP_OK;
}