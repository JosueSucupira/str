#include "pico/stdlib.h"
#include "pico/time.h"
#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"

// Defina os números dos pinos GPIO para os LEDs
#define LED1_PIN 2
#define LED2_PIN 4

// Função para piscar um LED continuamente
void blink_led(void *led_param) {
    int led_pin = (int)led_param;
    while (1) {
        //gpio_set_level(led_pin, 1); // Ligue o LED
        vTaskDelay(pdMS_TO_TICKS(500)); // Aguarde 500 ms
        prinft("tarefa 1\n");
        //gpio_set_level(led_pin, 0); // Desligue o LED
        vTaskDelay(pdMS_TO_TICKS(500)); // Aguarde mais 500 ms
    }
}

// Função para piscar um LED apenas três vezes
void blink_led_three_times(void *led_param) {
    int led_pin = (int)led_param;
    for (int i = 0; i < 3; i++) {
        gpio_set_level(led_pin, 1); // Ligue o LED
        vTaskDelay(pdMS_TO_TICKS(500)); // Aguarde 500 ms
        prinft("tarefa 2\n");
        gpio_set_level(led_pin, 0); // Desligue o LED
        vTaskDelay(pdMS_TO_TICKS(500)); // Aguarde mais 500 ms
    }
    vTaskDelete(NULL); // Finalize a tarefa
}

void main() {
    // Configuração dos pinos GPIO para os LEDs
    gpio_pad_select_gpio(LED1_PIN);
    gpio_set_direction(LED1_PIN, OUTPUT);
    gpio_pad_select_gpio(LED2_PIN);
    gpio_set_direction(LED2_PIN, OUTPUT);

    // Crie as tarefas
    xTaskCreate(blink_led, "LED1 Task", 2048, (void *)LED1_PIN, 1, NULL);
    xTaskCreate(blink_led, "LED2 Task", 2048, (void *)LED2_PIN, 1, NULL);
    xTaskCreate(blink_led_three_times, "LED3 Times Task", 2048, (void *)LED1_PIN, 1, NULL);

    // Inicie o escalonador FreeRTOS
    vTaskStartScheduler();
}
