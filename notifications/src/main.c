#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define STACK_SIZE 1024 * 2

TaskHandle_t receiverHandler = NULL;
TaskHandle_t senderHandler = NULL;

void sender(void *pvParameters);
void receiver(void *pvParameters);

void app_main(void)
{
    xTaskCreate(&receiver, "receiver", STACK_SIZE, NULL, configMAX_PRIORITIES, &receiverHandler);
    xTaskCreate(&sender, "sender", STACK_SIZE, NULL, configMAX_PRIORITIES, &senderHandler);
}
void sender(void *pvParameters)
{
    while (1)
    {
        xTaskNotifyGive(receiverHandler);
        xTaskNotifyGive(receiverHandler);
        vTaskDelay(5000 / portTICK_PERIOD_MS);
    }
}
void receiver(void *pvParameters)
{
    while (1)
    {
        int count = ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
        printf("Received notification %d times.\n", count);
        printf("------------------------------\n");
    }
}