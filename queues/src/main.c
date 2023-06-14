#include <stdio.h>
#include "esp_err.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

TaskHandle_t myTaskHandle = NULL;
QueueHandle_t GlobalQueue = 0;

#define STACK_SIZE 1024 * 2

void vTaskSend(void *pvParameters);
void vTaskReceive(void *pvParameters);

void app_main()
{
    GlobalQueue = xQueueCreate(20, sizeof(uint32_t));
    xTaskCreate(&vTaskSend, "vTaskSend", STACK_SIZE, NULL, 1, &myTaskHandle);
    xTaskCreate(&vTaskReceive, "vTaskReceive", STACK_SIZE, NULL, 1, &myTaskHandle);
}
void vTaskSend(void *pvParameters)
{
    while (1)
    {
        for (size_t i = 0; i < 8; i++)
        {
            vTaskDelay(pdMS_TO_TICKS(200));
            printf("Enviando %i a la queue\n", i);
            if (!xQueueSend(GlobalQueue, &i, pdMS_TO_TICKS(100)))
            {
                printf("Error al enviar %i a la queue\n", i);
            }
            vTaskDelay(pdMS_TO_TICKS(200));
        }
        vTaskDelay(pdMS_TO_TICKS(5000));
    }
}
void vTaskReceive(void *pvParameters)
{
    int receivedValue = 0;

    while (1)
    {
        if (!xQueueReceive(GlobalQueue, &receivedValue, pdMS_TO_TICKS(100)))
        {
            printf("Error al recibir valores\n");
        }
        else
        {
            vTaskDelay(pdMS_TO_TICKS(1000));
            printf("valores recibidos: %i\n", receivedValue);
            vTaskDelay(pdMS_TO_TICKS(1000));
        }
    }
    vTaskDelay(pdMS_TO_TICKS(100));
}
