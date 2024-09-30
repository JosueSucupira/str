#include "pico/stdlib.h"
#include "pico/time.h"
#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"

#define LED1_PIN 22
#define LED2_PIN 27
#define DELAY_MS 150
int count = 0;
// Protótipos das funções das tarefas
void task1(void *pvParameters);
void task2(void *pvParameters);

int main() {
    stdio_init_all();
    
    gpio_init(LED1_PIN);
    gpio_set_dir(LED1_PIN, GPIO_OUT);
    gpio_init(LED2_PIN);
    gpio_set_dir(LED2_PIN, GPIO_OUT);

    
    xTaskCreate(task1, "Task1", 1024, NULL, 1, NULL);
    xTaskCreate(task2, "Task2", 1024, NULL, 1, NULL);
    
  
    vTaskStartScheduler();

    return 0;
}


void task1(void *pvParameters) {
    for(;;){
        gpio_put(LED1_PIN, 1);
        sleep_ms(DELAY_MS); 
        gpio_put(LED1_PIN, 0); 
        sleep_ms(DELAY_MS);
    }
}

void task2(void *pvParameters) {
  
    for(;;){
        gpio_put(LED2_PIN, 0); 
        sleep_ms(DELAY_MS); 
        gpio_put(LED2_PIN, 1); 
        sleep_ms(DELAY_MS); 
        count++;
     	if(count == 3){
     	    break;
     	}
    }
   gpio_put(LED2_PIN, 0);
   vTaskDelete(NULL);
}
