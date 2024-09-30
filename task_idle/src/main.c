#include "pico/stdlib.h"
#include "pico/time.h"
#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"

#define LED1_PIN 1
#define LED2_PIN 6

int count = 0;
volatile uint32_t ulIdleCycleCount = 0UL;
volatile uint32_t lastTotalTicks = 0UL;
TickType_t xLastIdleTime = 0;

void task1(void *pvParameters);
void task2(void *pvParameters);
void vApplicationIdleHook(void);
void vCPUUsage(void *pvParameters);
void busy_wait(uint32_t milliseconds);

int main() {
    stdio_init_all();
    
    gpio_init(LED1_PIN);
    gpio_set_dir(LED1_PIN, GPIO_OUT);
    gpio_init(LED2_PIN);
    gpio_set_dir(LED2_PIN, GPIO_OUT);

    
    xTaskCreate(task1, "Task1", 1024, NULL, 1, NULL);
    xTaskCreate(task2, "Task2", 1024, NULL, 1, NULL);
    xTaskCreate(vCPUUsage, "CPUUsage", 1024, NULL, 1, NULL);
  
    vTaskStartScheduler();

    return 0;
}

void task1(void *pvParameters) {
     TickType_t Last_Time = xTaskGetTickCount();
    for(;;){
        //printf("\nTASK 1 \n");
        gpio_put(LED1_PIN, 1);
        busy_wait(20);
        //vTaskDelayUntil( &Last_Time, pdMS_TO_TICKS(150)); 
        gpio_put(LED1_PIN, 0);
        //vTaskDelayUntil( &Last_Time, pdMS_TO_TICKS(150));
        busy_wait(20);
        vTaskDelayUntil( &Last_Time, pdMS_TO_TICKS(150));
    }
}

void task2(void *pvParameters) {
    TickType_t Last_Time = xTaskGetTickCount();
    for(;;){
        //printf("\nTASK 2 \n");
        gpio_put(LED2_PIN, 1);
        //vTaskDelayUntil( &Last_Time, pdMS_TO_TICKS(250));
        busy_wait(20);
        gpio_put(LED2_PIN, 0);
        //vTaskDelayUntil( &Last_Time, pdMS_TO_TICKS(250)); 
        busy_wait(20);
        vTaskDelayUntil( &Last_Time, pdMS_TO_TICKS(500));
    }
}

void vApplicationIdleHook(void) {
    TickType_t xCurrentTime = xTaskGetTickCount();
    // Incrementa ulIdleCycleCount apenas se um tick tiver passado
    if ((xCurrentTime - xLastIdleTime) >= 1) { // 1 tick é a menor unidade de tempo no FreeRTOS
        ulIdleCycleCount++;
        xLastIdleTime = xCurrentTime;
    }
}

void vCPUUsage(void *pvParameters) {
    TickType_t Last_Time = xTaskGetTickCount();
    
    while (1) {
        TickType_t totalTicks = xTaskGetTickCount();
        uint32_t currentTotalTicks = totalTicks - lastTotalTicks;
        float cpuUsage = 100.0 * (1.0 - ((float)ulIdleCycleCount / (float)currentTotalTicks));

        printf("CPU USAGE: %.2f%%\n", cpuUsage);

        ulIdleCycleCount = 0;
        lastTotalTicks = totalTicks;
        vTaskDelayUntil(&Last_Time, pdMS_TO_TICKS(1000));
    }
}

void busy_wait(uint32_t milliseconds) {
    TickType_t start_time = xTaskGetTickCount();
    TickType_t delay_ticks = pdMS_TO_TICKS(milliseconds);

    while ((xTaskGetTickCount() - start_time) < delay_ticks) {
    }
}
