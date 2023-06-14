#include <stdio.h>
#include "esp_err.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

#define STACK_SIZE 1024 * 2

TaskHandle_t myTaskHandle = NULL;
SemaphoreHandle_t xMutex = 0;

void temperature(void *pvParameters);
void humidity(void *pvParameters);
esp_err_t shared_resource(char *message);

void app_main()
{
    xMutex = xSemaphoreCreateMutex();
    xTaskCreate(&temperature, "temperature", STACK_SIZE, NULL, configMAX_PRIORITIES, &myTaskHandle);
    xTaskCreate(&humidity, "humidity", STACK_SIZE, NULL, configMAX_PRIORITIES, &myTaskHandle);
}
esp_err_t shared_resource(char *message)
{
    printf(message);
    return ESP_OK;
}
void temperature(void *pvParameters)
{
    while (1)
    {
        printf("Reading temperature \n");
        if (xSemaphoreTake(xMutex, portMAX_DELAY) == pdPASS)
        {
            shared_resource("Temperature is 25 degrees \n");
            xSemaphoreGive(xMutex);
        }
        else
        {
            printf("Writing temperature time out \n");
        }
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
void humidity(void *pvParameters)
{
    while (1)
    {
        printf("Reading humidity \n");
        if (xSemaphoreTake(xMutex, portMAX_DELAY) == pdPASS)
        {
            shared_resource("Humidity is 50 \n");
            xSemaphoreGive(xMutex);
        }
        else
        {
            printf("Writing humidity time out \n");
        }
        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}
