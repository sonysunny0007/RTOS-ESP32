
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"


TaskHandle_t myTask1Handle = NULL;
TaskHandle_t myTask2Handle = NULL;

int i=0;
void task1(void *arg){
    while(1){
        i=xTaskGetTickCount();
        printf("From task1!\n");
        vTaskDelay(1000/portTICK_RATE_MS);
        if(i==300){
            vTaskSuspend(myTask2Handle);
            printf("Task2 suspended!\n");
        }
        if(i==500){
            vTaskResume(myTask2Handle);
            printf("Task2 Resumed!\n");
        }
        if(i==700){
            vTaskDelete(myTask2Handle);
            printf("Task2 Deleted!\n");
        }
    }
    
}

void task2(void *arg){
    while (1){
        printf("from task2!\n");
        vTaskDelay(1000/portTICK_RATE_MS);
    }
    
    
}



void app_main(){
    xTaskCreate(task1, "task1", 4096, NULL, 10, &myTask1Handle);
    xTaskCreate(task2, "Task2", 4096, NULL, 10, &myTask2Handle);

}