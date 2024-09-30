#include "pico/stdlib.h"
#include "pico/time.h"
#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"

void vBlinkTask(void *pvParameters) {
    while (1) {
        gpio_put(PICO_DEFAULT_LED_PIN, 1);
        sleep_ms(250);
        printf("ACENDEU!!!\n");
        gpio_put(PICO_DEFAULT_LED_PIN, 0);
        sleep_ms(250);
    }
}

int main() {
    stdio_init_all();
    gpio_init(PICO_DEFAULT_LED_PIN);
    gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);

    xTaskCreate(vBlinkTask, "Blink Task", 256, NULL, 1, NULL);

    vTaskStartScheduler();

    return 0;
}

