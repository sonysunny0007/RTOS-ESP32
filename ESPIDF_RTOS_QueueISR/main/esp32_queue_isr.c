
#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"

#define ESP_INTR_FLAG_DEFAULT 0
#define CONFIG_BUTTTON_PIN 0
#define CONFIG_LED_PIN 2

TaskHandle_t myTask1Handle = NULL; // creating tas1 handle
TaskHandle_t myTask2Handle = NULL; // creating task2 handle
QueueHandle_t queue1;              // creating queue handle
TaskHandle_t ISR = NULL;           // creating ISR handle

// Creating interrupt function
void IRAM_ATTR button_isr_handler(void *arg)
{
    char txbuff[20];
    sprintf(txbuff, "hello from ISR");
    xQueueSendFromISR(queue1, &txbuff, 1);
}

void task1(void *arg)
{
    /*Write task1 code here*/

    char txbuff[20];                          // creating transmit buffer
    queue1 = xQueueCreate(5, sizeof(txbuff)); // creating queue queuelength, size (Total Qbuffer size 1)

    if (queue1 == 0)
    {
        printf("Failed to create queue1 = %p \n", queue1);
    }
    while (1)
    {
        sprintf(txbuff, "Hello world 1");
        if (xQueueSend(queue1, (void *)txbuff, (TickType_t)0) != 1)
        {                                                         // sending data from buffer
            printf("Could not send this message = %s\n", txbuff); // Checking the send data
        }
        vTaskDelay(500 / portTICK_RATE_MS);
        sprintf(txbuff, "Hello world 2");
        if (xQueueSend(queue1, (void *)txbuff, (TickType_t)0) != 1)
        {                                                         // sending data from buffer
            printf("Could not send this message = %s\n", txbuff); // Checking the send data
        }
        vTaskDelay(500 / portTICK_RATE_MS);
        sprintf(txbuff, "Hello world 3");
        if (xQueueSend(queue1, (void *)txbuff, (TickType_t)0) != 1)
        {                                                         // sending data from buffer
            printf("Could not send this message = %s\n", txbuff); // Checking the send data
        };
        vTaskDelay(500 / portTICK_RATE_MS);
    }

    /*if(xQueueOverwrite(queue1, (void*)txbuff)){       //sending data from buffer using overwriting method
        printf("Could not send this message = %s\n", txbuff);   //Checking the send data
    };*/

    // while(1){
    //     printf("Data waiting to be read: %d available space: %d\n", uxQueueMessagesWaiting(queue1), uxQueueSpacesAvailable(queue1));
    //     vTaskDelay(1000/portTICK_RATE_MS);
    // }
}

void task2(void *arg)
{
    /*Write task2 code here*/
    char rxbuff[20];
    char temp[20];
    printf("Task2!\n");
    // bool led_status=false;
    while (1)
    {
        if (xQueueReceive(queue1, &rxbuff, (TickType_t)5))
        { // reading recieve buffer
            strcpy(temp, rxbuff);
            // printf("The buffer message is: %s\n", temp);
            if (strcmp(temp, "hello from ISR") == 0)
            {
                gpio_set_level(CONFIG_LED_PIN, 1);
                printf("Got a data from queue! ==== %s \n", rxbuff);
                vTaskDelay(1000 / portTICK_RATE_MS);
            }
            else
            {
                gpio_set_level(CONFIG_LED_PIN, 0);
                printf("Got a data from queue! ==== %s \n", rxbuff);
                vTaskDelay(1000 / portTICK_RATE_MS);
            }
        }
    }
}

void app_main()
{

    gpio_pad_select_gpio(CONFIG_BUTTTON_PIN); // initialize button pin
    gpio_pad_select_gpio(CONFIG_LED_PIN);     // initialize led pin

    gpio_set_direction(CONFIG_BUTTTON_PIN, GPIO_MODE_INPUT); // set direction button pin
    gpio_set_direction(CONFIG_LED_PIN, GPIO_MODE_OUTPUT);    // set direction led pin

    gpio_set_intr_type(CONFIG_BUTTTON_PIN, GPIO_INTR_NEGEDGE); // enable interrupt in falling edge(1->0) for the button pin

    // Install the GPIO ISR handler service, whoch allows IRS interrupt handlers
    // Install ISR handler with default configuration
    gpio_install_isr_service(ESP_INTR_FLAG_DEFAULT);

    gpio_isr_handler_add(CONFIG_BUTTTON_PIN, button_isr_handler, NULL); // attach the interrupt service routine

    xTaskCreate(task1, "task1", 4096, NULL, 10, &myTask1Handle);                // creating task1
    xTaskCreatePinnedToCore(task2, "task2", 4096, NULL, 10, &myTask2Handle, 1); // creating task2
}