
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"


TaskHandle_t myTask1Handle = NULL;     //creating task1 handle
TaskHandle_t myTask2Handle = NULL;     //creating task2 handle

int i=0;
void task1(void *arg){
    /*Write task1 code here*/
    while (1){
        printf("from task1!\n");
        vTaskDelay(1000/portTICK_RATE_MS);
    }
    
}

void task2(void *arg){
    /*write task2 code here*/
    while (1){
        printf("from task2!\n");
        vTaskDelay(1000/portTICK_RATE_MS);
    }
    
    
}



void app_main(){
    xTaskCreate(task1, "task1", 4096, NULL, 10, &myTask1Handle);  //creating task1
    xTaskCreate(task2, "Task2", 4096, NULL, 10, &myTask2Handle);  //creating task2

}