#include <stdio.h>
#include <time.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "sdkconfig.h"
#include "esp_system.h"
#include "esp_spi_flash.h"

#define duration 1000
/* Can use project configuration menu (idf.py menuconfig) to choose the GPIO to blink,
   or you can edit the following line and set a number here.
*/

void vTaskPreempt(void *pvParameters) // preemtive task
{
    char *pcTaskName;
    pcTaskName = (char *)pvParameters;
    for (;;)
    {
        printf("------------------------------\n");
        for (int i = 0; i < 200; i++)
            printf("%ld ms| %s : %d\n", clock(), pcTaskName, i);
        vTaskDelay(1000 / portTICK_RATE_MS);
    }
    vTaskDelete(NULL);
}
void vTask(void *pvParameters) // preemtive task
{
    char *pcTaskName;
    pcTaskName = (char *)pvParameters;
    for (;;)
    {
        printf("------------------------------\n");
        for (int i = 0; i < 200; i++)
            printf("%ld ms| %s : %d\n", clock(), pcTaskName, i);
        vTaskDelay(1000 / portTICK_RATE_MS);
    }
    vTaskDelete(NULL);
}

void vTaskFunction(void *pvParameters)
{
    char *pcTaskName;
    pcTaskName = (char *)pvParameters;
    for (;;)
    {

        int count = 0;
        clock_t begin = clock();
        printf("------------------------------\n");
        printf((char *)pvParameters);
        printf("\nBegin: %f\n", (double)((double)begin / CLOCKS_PER_SEC));
        while ((double)(clock() - begin) < duration)
        {
            // printf("%ld ms| %s : %d\n", clock(), pcTaskName, count);
            count++;
        }
        printf((char *)pvParameters);
        printf(" end at %f from %f\n", (double)((double)clock() / CLOCKS_PER_SEC), (double)((double)begin / CLOCKS_PER_SEC));
        vTaskDelay(1000 / portTICK_RATE_MS);
    }
    vTaskDelete(NULL);
}

static const char *pcTextForPreempTask = "Preempt 1";
static const char *pcTextForTask1 = "Preempt 2";
static const char *pcTextForTask2 = "Idle 1";
void app_main(void)
{
    printf("USE_TIME_SLICING %d\n", configUSE_TIME_SLICING);
    printf("USE_PREEMPTION %d\n", configUSE_PREEMPTION);
    xTaskCreatePinnedToCore(&vTaskFunction, "Task 1", 4096, (void *)pcTextForTask2, 1, NULL, 1);
    xTaskCreatePinnedToCore(&vTaskFunction, "Preempt 1", 4096, (void *)pcTextForPreempTask, 2, NULL, 1);
    xTaskCreatePinnedToCore(&vTaskFunction, "Preempt 2", 4096, (void *)pcTextForTask1, 2, NULL, 1);
}
