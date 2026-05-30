#include "UartTask.h"

#include "FreeRTOS.h"
#include "task.h"
#include "AHardware/UART/UARTLogger.h"
#include "Application/SharedData/SharedData.h"
#include "Interfaces.h"

namespace {
const TickType_t kUartTaskPeriod = pdMS_TO_TICKS(1000);

UARTLogger uartLogger;
ILogger* logger = &uartLogger;

float lastTemperature = 0.0f;
float lastSpeed = 0.0f;

void UartTaskFunction(void* taskParameter) {
    (void) taskParameter;
    TickType_t lastWakeTime = xTaskGetTickCount();

    for (;;) {
        taskENTER_CRITICAL();
        const float temperature = g_data.temperature;
        const float speed = g_data.speed;
        taskEXIT_CRITICAL();

        if (temperature != lastTemperature) {
            logger->LogTemperature(temperature);
            lastTemperature = temperature;
        }

        if (speed != lastSpeed) {
            logger->LogSpeed(speed);
            lastSpeed = speed;
        }

        vTaskDelayUntil(&lastWakeTime, kUartTaskPeriod);
    }
}
}

void UartTask_Start(void) {
    logger->Init();
    xTaskCreate(UartTaskFunction, "UART", 256, NULL, 1, NULL);
}
