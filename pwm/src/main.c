#include <stdio.h>
#include "esp_err.h"
#include "driver/gpio.h"
#include "driver/ledc.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define PIN_PWM GPIO_NUM_16

esp_err_t set_pwm(void);
esp_err_t set_pwm_duty(void);

void app_main()
{
    set_pwm();
    set_pwm_duty();
    while (true)
    {
        ledc_set_fade_time_and_start(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0, 0, 1000, LEDC_FADE_WAIT_DONE);
        ledc_set_fade_time_and_start(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0, 1024, 1000, LEDC_FADE_WAIT_DONE);
    }
}
esp_err_t set_pwm(void)
{
    ledc_channel_config_t channelConfig = {0};
    channelConfig.gpio_num = PIN_PWM;
    channelConfig.speed_mode = LEDC_HIGH_SPEED_MODE;
    channelConfig.channel = LEDC_CHANNEL_0;
    channelConfig.intr_type = LEDC_INTR_DISABLE;
    channelConfig.timer_sel = LEDC_TIMER_0;
    channelConfig.duty = 0;
    channelConfig.hpoint = 0;
    channelConfig.flags.output_invert = 0;
    ledc_channel_config(&channelConfig);

    ledc_timer_config_t timerConfig = {0};
    timerConfig.speed_mode = LEDC_HIGH_SPEED_MODE;
    timerConfig.duty_resolution = LEDC_TIMER_10_BIT;
    timerConfig.timer_num = LEDC_TIMER_0;
    timerConfig.freq_hz = 20000;
    timerConfig.clk_cfg = LEDC_AUTO_CLK;
    ledc_timer_config(&timerConfig);
    return ESP_OK;
}
esp_err_t set_pwm_duty(void)
{
    ledc_fade_func_install(0);
    for (int dutyLed = 0; dutyLed < 1024; dutyLed++)
    {
        ledc_set_duty_and_update(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0, dutyLed, 0);
        // ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0, dutyLed);
        // ledc_update_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0);
        vTaskDelay(pdMS_TO_TICKS(10));
    }
    return ESP_OK;
}
