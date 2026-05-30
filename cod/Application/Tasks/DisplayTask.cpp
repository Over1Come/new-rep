#include "DisplayTask.h"

#include "FreeRTOS.h"
#include "task.h"
#include "AHardware/LCD/LiquidCrystal_I2C.h"
#include "Application/SharedData/SharedData.h"
#include "Interfaces.h"

namespace {
const TickType_t kDisplayTaskPeriod = pdMS_TO_TICKS(200);

LiquidCrystal_I2C lcdDriver(0x27, 16, 2);
IDisplay* display = &lcdDriver;

void DisplayTaskFunction(void* taskParameter) {
    (void) taskParameter;
    TickType_t lastWakeTime = xTaskGetTickCount();

    for (;;) {
        if (g_data.new_data) {
            taskENTER_CRITICAL();
            const float temperature = g_data.temperature;
            const float speed = g_data.speed;
            g_data.new_data = false;
            taskEXIT_CRITICAL();

            display->ShowTemperature(temperature);
            display->ShowSpeed(speed);
        }

        vTaskDelayUntil(&lastWakeTime, kDisplayTaskPeriod);
    }
}
}

void DisplayTask_Start(void) {
    display->Init();
    display->Clear();
    lcdDriver.Backlight();
    xTaskCreate(DisplayTaskFunction, "Display", 256, NULL, 2, NULL);
}
