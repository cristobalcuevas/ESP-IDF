#include <stdio.h>
#include "esp_err.h"
#include "esp_adc_cal.h"
#include "driver/gpio.h"
#include "driver/adc.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

esp_adc_cal_characteristics_t adc_chars;

int adc_raw = 0;
uint32_t voltage = 0;

esp_err_t set_adc(void);

void app_main()
{
    set_adc();
    while (true)
    {
        adc_raw = adc1_get_raw(ADC_CHANNEL_4);
        voltage = esp_adc_cal_raw_to_voltage(adc_raw, &adc_chars);
        printf("ADC val: %d \t Voltage: %d mV\n", adc_raw, voltage);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
esp_err_t set_adc(void)
{
    esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_11, ADC_WIDTH_BIT_DEFAULT, 0, &adc_chars);
    adc1_config_width(ADC_WIDTH_BIT_12);
    adc1_config_channel_atten(ADC_CHANNEL_4, ADC_ATTEN_DB_11);
    return ESP_OK;
}
