#include "pico/stdlib.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include <stdio.h>

SemaphoreHandle_t printfMutex;

void taskMsg(void *pvParameters) {
    char *msg = (char *) pvParameters;
    
    for (;;) {
        if (xSemaphoreTake(printfMutex, portMAX_DELAY)) {
            for (int i = 0; msg[i] != '\0'; i++) {
                printf("%c", msg[i]);
                vTaskDelay(pdMS_TO_TICKS(10));
            }
            printf("\n");
            xSemaphoreGive(printfMutex);
        }
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

int main(void) {
    stdio_init_all();

    printfMutex = xSemaphoreCreateMutex();

    if (printfMutex != NULL) {
        xTaskCreate(taskMsg, "TaskMsg1", configMINIMAL_STACK_SIZE * 4, 
                    (void *)"---𝄞0123456789𝄞---", 1, NULL);

        xTaskCreate(taskMsg, "TaskMsg2", configMINIMAL_STACK_SIZE * 4, 
                    (void *)"***𝄞abcdefghij𝄞***", 1, NULL);

        vTaskStartScheduler();
    }

    while (1) {
    }
}
