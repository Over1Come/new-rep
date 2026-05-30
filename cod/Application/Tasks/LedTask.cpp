#include "LedTask.h"

#include "FreeRTOS.h"
#include "task.h"
#include "AHardware/LED/LEDIndicator.h"
#include "Application/SharedData/SharedData.h"
#include "Interfaces.h"

namespace {
const TickType_t kLedTaskPeriod = pdMS_TO_TICKS(50);

LEDIndicator ledDriver;
ILedIndicator* leds = &ledDriver;

void LedTaskFunction(void* taskParameter) {
    (void) taskParameter;
    TickType_t lastWakeTime = xTaskGetTickCount();

    for (;;) {
        taskENTER_CRITICAL();
        const float speed = g_data.speed;
        taskEXIT_CRITICAL();

        leds->Update(speed);
        vTaskDelayUntil(&lastWakeTime, kLedTaskPeriod);
    }
}
}

void LedTask_Start(void) {
    leds->Init();
    xTaskCreate(LedTaskFunction, "LED", 256, NULL, 2, NULL);
}
