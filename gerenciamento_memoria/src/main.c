#include "pico/stdlib.h"
#include "FreeRTOS.h"
#include "task.h"
#include <stdio.h>

// Define o pino do LED (modifique de acordo com o hardware)
#define LED_PIN 25

#define TOTAL_HEAP_SIZE configTOTAL_HEAP_SIZE 
#define TARGET_HEAP_USAGE ((TOTAL_HEAP_SIZE * 91) / 100)  // 91% do heap
#define BLOCK_SIZE 4048  // Tamanho do bloco é 4048 bytes

void *allocatedMemory[32];  // Vetor para armazenar os blocos alocados (32 blocos de 4048 bytes)
int allocatedBlocks = 0;
bool allocationPhase = true;  // Define se estamos na fase de alocação

void hardware_init() {
    stdio_init_all();        
    gpio_init(LED_PIN);      
    gpio_set_dir(LED_PIN, GPIO_OUT); 
}

void vMemoryManagementTask(void *pvParameters) {
    for (;;) {
        if (allocationPhase) {
            // Fase de alocação
            if (allocatedBlocks < (TARGET_HEAP_USAGE / BLOCK_SIZE)) {
                allocatedMemory[allocatedBlocks] = pvPortMalloc(BLOCK_SIZE);
                if (allocatedMemory[allocatedBlocks] != NULL) {
                    printf("\nBloco %d alocado: %u bytes\n", allocatedBlocks, BLOCK_SIZE);
                    allocatedBlocks++;
                } else {
                    printf("Falha ao alocar memória\n");
                }
            } else {
                // Alcançou o limite de 91% de uso da heap, muda para desalocação
                allocationPhase = false;
            }
        } else {
            // Fase de desalocação
            if (allocatedBlocks > 0) {
                allocatedBlocks--;
                vPortFree(allocatedMemory[allocatedBlocks]);
                printf("\nBloco %d desalocado\n", allocatedBlocks);
                allocatedMemory[allocatedBlocks] = NULL;
            } else {
                // Todo o bloco foi desalocado, volta para a fase de alocação
                allocationPhase = true;
            }
        }
        vTaskDelay(pdMS_TO_TICKS(500));  // Tempo de espera para cada operação
    }
}

// Tarefa para monitorar o estado do heap
void vMonitorHeapTask(void *pvParameters) {
    for(;;) {
        // Obtém o tamanho livre do heap
        size_t freeHeapSize = xPortGetFreeHeapSize();
        size_t usedHeapSize = TOTAL_HEAP_SIZE - freeHeapSize;
        
        // Envia o tamanho livre e ocupado do heap pela porta serial
        printf("\nHeap livre: %u bytes\n", freeHeapSize);

        if (freeHeapSize < (TOTAL_HEAP_SIZE / 10)) {
            gpio_put(LED_PIN, 1); 
        } else {
            gpio_put(LED_PIN, 0); 
        }

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

int main() {
    hardware_init();

    // Cria a tarefa de gerenciamento de memória
    xTaskCreate(vMemoryManagementTask, "Memory Management", 256, NULL, 1, NULL);

    // Cria a tarefa de monitoramento do heap
    xTaskCreate(vMonitorHeapTask, "Heap Monitor", 256, NULL, 1, NULL);

    vTaskStartScheduler();

    while (1);
}
