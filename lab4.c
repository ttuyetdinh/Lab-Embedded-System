#include <stdio.h>
#include <string.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "freertos/queue.h"
#define functionalTaskNumber 3
#define delay 1000
#define xTicksToWait 10000

typedef struct Packet
{
    int Id;
    int type;
    int functionRecieved;
} Packet;

static QueueHandle_t queue;
static const char *pcTextForTask0 = "Reception task";
static const char *pcTextForTask1 = "Functional task 1";
static const char *pcTextForTask2 = "Functional task 2";
static const char *pcTextForTask3 = "Functional task 3";

void vReceptionTask(void *pv)
{
    for (int i = 0;; i++)
    {
        Packet packet;
        packet.Id = i;
        if (i % 2 == 0)
            packet.type = 0;
        else if (i % 3 == 0)
            packet.type = 1;
        else if (i % 5 == 0)
            packet.type = 2;
        else
            packet.type = -1;
        packet.functionRecieved = 0;

        xQueueSend(queue, &packet, xTicksToWait);
        vTaskDelay(delay / portTICK_RATE_MS);
    }
    vTaskDelete(NULL);
}

void vFunctionalTask(void *pv)
{
    Packet packet;
    char *pcTaskName;
    pcTaskName = (char *)pv;
    for (;;)
    {
        if (xQueueReceive(queue, (void *)&packet, xTicksToWait) == pdTRUE)
        {
            if (((packet.type == 0) && (!strcmp(pcTaskName, "Functional task 1"))) || ((packet.type == 1) && (!strcmp(pcTaskName, "Functional task 2"))) || ((packet.type == 2) && (!strcmp(pcTaskName, "Functional task 3"))))
                printf("%s received and is executing packet id %d \n", pcTaskName, packet.Id);
            else
            {
                packet.functionRecieved += 1;
                printf(" Wrong packet at:%s |PacketId: %d \n", pcTaskName, packet.Id);
                if (functionalTaskNumber > packet.functionRecieved)
                {
                    xQueueSendToFront(queue, &packet, xTicksToWait);
                }

                else
                    printf("Error: no functional task executes request id %d \n", packet.Id);
            }
        }
        vTaskDelay(delay / portTICK_RATE_MS);
    }
    vTaskDelete(NULL);
}

void app_main(void)
{
    queue = xQueueCreate(10, sizeof(Packet));
    xTaskCreatePinnedToCore(&vReceptionTask, "Reception task", 4096, (void *)pcTextForTask0, 2, NULL, 1);
    xTaskCreatePinnedToCore(&vFunctionalTask, "Task 1", 4096, (void *)pcTextForTask1, 1, NULL, 1);
    xTaskCreatePinnedToCore(&vFunctionalTask, "Task 2", 4096, (void *)pcTextForTask2, 1, NULL, 1);
    xTaskCreatePinnedToCore(&vFunctionalTask, "Task 3", 4096, (void *)pcTextForTask3, 1, NULL, 1);
}