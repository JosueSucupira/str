#include "pico/stdlib.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "queue.h"

// Definições de pinos
#define BUTTON_PIN  16
#define LED_PIN     25

// Prototipos
void button_isr(uint gpio, uint32_t events);
void button_task(void *pvParameters);
void led_task(void *pvParameters);

SemaphoreHandle_t buttonSemaphore;
QueueHandle_t ledQueue;

// ISR do botão
void button_isr(uint gpio, uint32_t events) {
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    xSemaphoreGiveFromISR(buttonSemaphore, &xHigherPriorityTaskWoken);
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

void button_task(void *pvParameters) {
    while (1) {
        // Esperar pelo semáforo binário
        if (xSemaphoreTake(buttonSemaphore, portMAX_DELAY) == pdTRUE) {
            uint8_t toggle_command = 1;.
            xQueueSend(ledQueue, &toggle_command, portMAX_DELAY);
        }
    }
}

void led_task(void *pvParameters) {
    uint8_t received_command;
    static uint8_t led_state = 0;

    while (1) {
        // Receber comando da fila
        if (xQueueReceive(ledQueue, &received_command, portMAX_DELAY) == pdTRUE) {
            if (received_command == 1) {
                led_state ^= 1;
                gpio_put(LED_PIN, led_state); 
            }
        }
    }
}

int main(void) {
    stdio_init_all();
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    gpio_init(BUTTON_PIN);
    gpio_set_dir(BUTTON_PIN, GPIO_IN);
    gpio_pull_down(BUTTON_PIN);

    buttonSemaphore = xSemaphoreCreateBinary();
    ledQueue = xQueueCreate(10, sizeof(uint8_t));

    // Configurar a interrupção do botão
    gpio_set_irq_enabled_with_callback(BUTTON_PIN, GPIO_IRQ_EDGE_FALL, true, button_isr);

    // Criar tarefas
    xTaskCreate(button_task, "ButtonTask", 256, NULL, 1, NULL);
    xTaskCreate(led_task, "LedTask", 256, NULL, 1, NULL);

    // Iniciar o scheduler
    vTaskStartScheduler();

    while (1) {
        // Código principal
    }
}
