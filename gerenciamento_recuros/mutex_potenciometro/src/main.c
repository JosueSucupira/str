#include "pico/stdlib.h"
#include "FreeRTOS.h"
#include "task.h"
#include <stdio.h>
#include "semphr.h"
#include "hardware/adc.h"

#define ADC_PIN 26

typedef struct{
    uint16_t potentiometer_value;
}SensorData;

SensorData sensorData;
SemaphoreHandle_t structMutex;

void adc_task(void *params) {
    uint16_t adc_value;
    while (1) {
        adc_value = adc_read();
        if (xSemaphoreTake(structMutex, portMAX_DELAY)){
            sensorData.potentiometer_value = adc_value;
            xSemaphoreGive(structMutex);
        }
        vTaskDelay(pdMS_TO_TICKS(50));
    }
}

void taskSendToSerial(void *pvParameters) {
    while (1) {
        uint16_t value_to_send;
        const float conversion_factor = 3.3f / (1 << 12);
        if (xSemaphoreTake(structMutex, portMAX_DELAY)){
            value_to_send = sensorData.potentiometer_value;
            xSemaphoreGive(structMutex);
        }
        printf("Voltage: %.1f\n", value_to_send*conversion_factor);

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

int main() {
    stdio_init_all();

    structMutex = xSemaphoreCreateMutex();

    adc_init();
    adc_gpio_init(ADC_PIN);
    adc_select_input(0);

    xTaskCreate(adc_task, "ADC Task", 256, NULL, 2, NULL);
    xTaskCreate(taskSendToSerial, "Serial Task", 256, NULL, 1, NULL);

    vTaskStartScheduler();

    while (1) {
    }

    return 0;
}
