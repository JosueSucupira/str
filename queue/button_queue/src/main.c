#include "pico/stdlib.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include <stdio.h>

// Define os pinos
#define LED_PIN 25
#define SW1_PIN 14
#define SW2_PIN 15

// Cria uma fila para comunicação entre as tarefas
#define QUEUE_SIZE 10
QueueHandle_t buttonQueue;

// Estrutura para armazenar informações sobre o LED
typedef struct {
    unsigned int num;    
    unsigned int status;
} LedInfo;

// Função para a tarefa de leitura dos botões
void button_task(void *params) {
    static bool sw1_prev_state = false; // desligado 
    static bool sw2_prev_state = false; // desligado
    bool sw1_curr_state;
    bool sw2_curr_state;

    while (1) {
        sw1_curr_state = gpio_get(SW1_PIN);
        sw2_curr_state = gpio_get(SW2_PIN);

        if (sw1_curr_state == 0 && sw1_prev_state == 1) { // SW1 pressionado
            LedInfo info = { .num = LED_PIN, .status = 1 }; // Acender LED
            xQueueSend(buttonQueue, &info, portMAX_DELAY);
        }

        if (sw2_curr_state == 0 && sw2_prev_state == 1) { // SW2 pressionado
            LedInfo info = { .num = LED_PIN, .status = 0 }; // Apagar LED
            xQueueSend(buttonQueue, &info, portMAX_DELAY);
        }

        sw1_prev_state = sw1_curr_state;
        sw2_prev_state = sw2_curr_state;

        vTaskDelay(pdMS_TO_TICKS(100)); // Verifica o estado dos botões a cada 50 ms
    }
}

// Função para a tarefa de controle do LED
void led_task(void *params) {
    LedInfo info;
    while (1) {
        xQueueReceive(buttonQueue, &info, portMAX_DELAY);
        gpio_put(info.num, info.status); // Controla o LED
    }
}

int main() {
    // Inicializa a biblioteca padrão
    stdio_init_all();

    // Inicializa os pinos
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    gpio_init(SW1_PIN);
    gpio_set_dir(SW1_PIN, GPIO_IN);
    gpio_pull_up(SW1_PIN); 

    gpio_init(SW2_PIN);
    gpio_set_dir(SW2_PIN, GPIO_IN);
    gpio_pull_up(SW2_PIN); 

    // Cria a fila
    buttonQueue = xQueueCreate(QUEUE_SIZE, sizeof(LedInfo));

    // Cria as tarefas
    xTaskCreate(button_task, "Button Task", 256, NULL, 1, NULL);
    xTaskCreate(led_task, "LED Task", 256, NULL, 1, NULL);

    // Inicia o scheduler
    vTaskStartScheduler();

    while (1) {
    }

    return 0;
}
