#include "pico/stdlib.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include <stdio.h>
#include "hardware/adc.h"


// Define os pinos
#define LED_PIN 25
#define ADC_PIN 26

// Cria uma fila para comunicação entre as tarefas
#define QUEUE_SIZE 10
QueueHandle_t adcQueue;

// Função para a tarefa de leitura do ADC
void adc_task(void *params) {
    uint16_t adc_value;
    while (1) {
        adc_value = adc_read();
        xQueueSend(adcQueue, &adc_value, portMAX_DELAY);
        vTaskDelay(pdMS_TO_TICKS(300)); // Atraso de 300 ms
    }
}

// Função para a tarefa de controle do LED
void led_task(void *params) {
    uint16_t adc_value;
    while (1) {
        if (xQueueReceive(adcQueue, &adc_value, portMAX_DELAY)) {
            if (adc_value >= 2047) {
                gpio_put(LED_PIN, 1); // Acende o LED
            } else {
                gpio_put(LED_PIN, 0); // Apaga o LED
            }
        }
    }
}


int main() {
    // Inicializa a biblioteca padrão e o ADC
    stdio_init_all();
    adc_init();
    adc_gpio_init(ADC_PIN);
    
    // Configura o ADC para ler no canal 0
    adc_select_input(0);

    // Configura o LED como saída
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    // Cria a fila
    adcQueue = xQueueCreate(QUEUE_SIZE, sizeof(uint16_t));
    if (adcQueue == NULL) {
        printf("Erro ao criar a fila.\n");
        return -1;
    }

    // Cria as tarefas
    xTaskCreate(adc_task, "ADC Task", 256, NULL, 1, NULL);
    xTaskCreate(led_task, "LED Task", 256, NULL, 1, NULL);

    // Inicia o scheduler
    vTaskStartScheduler();

    while (1) {
        // O código não deve chegar aqui
    }

    return 0;
}
