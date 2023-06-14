#include <stdio.h>
#include "driver/gpio.h"
#include "esp_err.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_chip_info.h"
#include "esp_idf_version.h"
#include "esp_system.h"
#include "inttypes.h"
//#include "esp_spi_flash.h"
#include "esp_flash.h" // V5

#define LED_PIN GPIO_NUM_2
#define BLINK_TIME 1000
uint8_t led_value = 0;

void printed_info();
esp_err_t init_led(void);
esp_err_t blink_led(void);

void app_main()
{
    printed_info();
    init_led();
    while (1)
    {
        vTaskDelay(BLINK_TIME / portTICK_PERIOD_MS);
        blink_led();
        printf("LED value: %d\n", led_value);
    }
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
void printed_info()
{
    /* Print chip information */
    esp_chip_info_t chip_info;
    uint32_t flash_size;
    esp_chip_info(&chip_info);
    printf("This is %s chip with %d CPU core(s), WiFi%s%s%s, ",
           CONFIG_IDF_TARGET,
           chip_info.cores,
           (chip_info.features & CHIP_FEATURE_BT) ? "/BT" : "",
           (chip_info.features & CHIP_FEATURE_BLE) ? "/BLE" : "",
           (chip_info.features & CHIP_FEATURE_IEEE802154) ? ", 802.15.4 (Zigbee/Thread)" : "");

    unsigned major_rev = chip_info.revision / 100;
    unsigned minor_rev = chip_info.revision % 100;
    printf("silicon revision v%d.%d, ", major_rev, minor_rev);
    if (esp_flash_get_size(NULL, &flash_size) != ESP_OK)
    {
        printf("Get flash size failed");
        return;
    }
    printf("IDF version is %s.\n",esp_get_idf_version());

    printf("%" PRIu32 "MB %s flash.\n", flash_size / (uint32_t)(1024 * 1024),
           (chip_info.features & CHIP_FEATURE_EMB_FLASH) ? "embedded" : "external");

    printf("Minimum free heap size: %" PRIu32 " bytes.\n", esp_get_minimum_free_heap_size());
}