/**
 * @file main.c
 * @author Cristobal Cuevas
 * @date 24-04-2023
 * @brief Comunicaion UART por medio de tareas
 * @see https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/peripherals/uart.html
 */

#include <stdio.h>
#include <strings.h>
#include "esp_err.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/uart.h"

#define RXD_PIN 9
#define TXD_PIN 10
#define BUF_SIZE 1024
#define BAUD_RATE 115200
#define TASK_MEMORY 1024 * 2
#define UART UART_NUM_2

static void uart_task(void *pvParameters)
{
    uint8_t *data = (uint8_t *)malloc(BUF_SIZE);
    while (1)
    {
        bzero(data, BUF_SIZE);
        int len = uart_read_bytes(UART, data, BUF_SIZE, pdMS_TO_TICKS(100));
        if (len == 0)
        {
            continue;
        }
        uart_write_bytes(UART, (const char *)data, len);
        printf("Datos recividos: %s", data);
    }
}
static void uart_init()
{
    uart_config_t uart_config = {
        .baud_rate = BAUD_RATE,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_APB,
    };
    ESP_ERROR_CHECK(uart_param_config(UART, &uart_config));
    ESP_ERROR_CHECK(uart_set_pin(UART, TXD_PIN, RXD_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE));
    ESP_ERROR_CHECK(uart_driver_install(UART, BUF_SIZE, 0, 0, NULL, 0));
    printf("UART Iniciado");
}
void app_main()
{
    uart_init();
    xTaskCreate(uart_task, "uart_task", TASK_MEMORY, NULL, configMAX_PRIORITIES, NULL);
}