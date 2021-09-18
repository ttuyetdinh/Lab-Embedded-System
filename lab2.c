/*
    Does the ESP-IDF need the vTaskStartScheduler() routine?
    => No we don’t need to call out vTaskStartScheduler() function because this function is
already called before the app_main() function is called. Therefore, we don’t need to call it
again.
*/

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "sdkconfig.h"
#include "esp_system.h"
#include "esp_spi_flash.h"

/* Can use project configuration menu (idf.py menuconfig) to choose the GPIO to blink,
   or you can edit the following line and set a number here.
*/
#define LED_GPIO 22
#define BT_GPIO 15

void printName(void* pv);
void buttonHandle(void* pv);
bool press = false;
void app_main(void) {
    // PIN set up //
    gpio_pad_select_gpio(BT_GPIO);
    gpio_set_direction(BT_GPIO, GPIO_MODE_INPUT);  //Set the GPIO as a input
    gpio_set_pull_mode(BT_GPIO, GPIO_PULLDOWN_ONLY);
    
    gpio_set_direction(LED_GPIO, GPIO_MODE_OUTPUT);

    // Task run //
    xTaskCreate(&printName, "task1", 1024, NULL, 0, NULL);
    xTaskCreate(&buttonHandle, "task2", 1024, NULL, 0, NULL);
}
void printName(void* pv) {
    while (1) {
        printf(": Nguyen Minh Phuc 1852666\n");

        vTaskDelay(1000 / portTICK_RATE_MS);
    }
    vTaskDelete(NULL);
}

void buttonHandle(void* pv) {
    while (1) {
        if (gpio_get_level(BT_GPIO) == 1 && press == true) {
            printf("ESP32\n");
            press = false;
             gpio_set_level(LED_GPIO, 1);
        }
        else if (gpio_get_level(BT_GPIO) == 0 && press == false) {
            gpio_set_level(LED_GPIO, 0);
            press = true;
        }
        vTaskDelay(1000 / portTICK_RATE_MS);
        
    }
    vTaskDelete(NULL);
}
