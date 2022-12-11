
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

#define ESP_INR_FLAG_DEFAULT 0
#define CONFIG_BUTTON_PIN 0
#define CONFIG_LED_PIN 2

TaskHandle_t ISR = NULL; // creating task handle

// creating interrupt function
void IRAM_ATTR button_isr_handler(void *arg)
{
    vTaskResume(ISR);
}

void button_task(void *arg)
{
    /*Write task1 code here*/
    printf("Button Task!\n");
    bool led_status = false;
    while (1)
    {
        vTaskSuspend(NULL);
        led_status = !led_status;
        gpio_set_level(CONFIG_LED_PIN, led_status);
        printf("Button pressed!\n");
    }
}

void app_main()
{

    // initializing gpio pins
    gpio_pad_select_gpio(CONFIG_BUTTON_PIN);
    gpio_pad_select_gpio(CONFIG_LED_PIN);

    // set the correct direction
    gpio_set_direction(CONFIG_BUTTON_PIN, GPIO_MODE_INPUT);
    gpio_set_direction(CONFIG_LED_PIN, GPIO_MODE_OUTPUT);

    // enable interrupt on falling edge(1->0) for the button pin
    gpio_set_intr_type(CONFIG_BUTTON_PIN, GPIO_INTR_NEGEDGE);

    // Install the dirver's GPIO ISR handler service, which allows per-pin GPIO interrupt handlers
    // Install ISR service with default configuration
    gpio_install_isr_service(ESP_INTR_FLAG_EDGE);

    // attach the interrupt service routine
    gpio_isr_handler_add(CONFIG_BUTTON_PIN, button_isr_handler, NULL);

    xTaskCreate(button_task, "button_task", 4096, NULL, 10, &ISR); // creating task1
}