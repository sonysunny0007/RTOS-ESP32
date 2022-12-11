
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

TaskHandle_t myTask1Handle = NULL;  //creating task1 handle
TaskHandle_t myTask2Handle = NULL;  //creating task2 handle
QueueHandle_t queue1;               //creating queue handle

void task1(void *arg){
    /*Write task1 code here*/
    char txbuff[20];  //creating transmit buffer
    queue1 = xQueueCreate(5, sizeof(txbuff));    //queuelength, size (Total Space 5)
    
    if(queue1 == 0){
        printf("Failed to create queue1 = %p \n", queue1);
    }

    sprintf(txbuff, "Hello world 1");
    xQueueSend(queue1, (void*)txbuff, (TickType_t)0);   //sending data from buffer

    sprintf(txbuff, "Hello world 2");
    xQueueSend(queue1, (void*)txbuff, (TickType_t)0);   //sending data from buffer

    sprintf(txbuff, "Hello world 3");
    xQueueSend(queue1, (void*)txbuff, (TickType_t)0);   //sending data from buffer

    while(1){
        printf("Data waiting to be read: %d available space: %d\n", uxQueueMessagesWaiting(queue1), uxQueueSpacesAvailable(queue1));
        vTaskDelay(1000/portTICK_RATE_MS);
    }
    
}

void task2(void *arg){
    /*Write task2 code here*/
    
    char rxbuff[20];
    while(1){
        if(xQueueReceive(queue1, &rxbuff, (TickType_t)5)){  //reading recieve buffer
            printf("Got a data from queue1 ==== %s \n", rxbuff);
            vTaskDelay(1000/portTICK_RATE_MS);
        }
    }
    
}

void app_main(){

    

    xTaskCreate(task1, "task1", 4096, NULL, 10, &myTask1Handle);  //creating task1
    xTaskCreatePinnedToCore(task2, "task2", 4096, NULL, 10, &myTask2Handle,1);  //creating task2
    

}