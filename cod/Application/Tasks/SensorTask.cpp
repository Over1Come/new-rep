#include "SensorTask.h"

#include "FreeRTOS.h"
#include "task.h"
#include "AHardware/BME280/BME280.h"
#include "Application/Calculator/SpeedCalculator.h"
#include "Application/Filter/LowPassFilter.h"
#include "Application/SharedData/SharedData.h"

namespace {
const TickType_t kSensorTaskPeriod = pdMS_TO_TICKS(100);
const float kDefaultSetpoint = 25.0f;

BME280 bme280(0x76);
LowPassFilter temperatureFilter(0.1f, 2.0f);
SpeedCalculator speedCalculator(23.0f, 35.0f);

struct SensorTaskContext {
    ISensor* sensor;
    IFilter* filter;
    ICalculator* calculator;
};

SensorTaskContext defaultContext = {
    &bme280,
    &temperatureFilter,
    &speedCalculator
};

void SensorTaskFunction(void* taskParameter) {
    SensorTaskContext* context = static_cast<SensorTaskContext*>(taskParameter);
    TickType_t lastWakeTime = xTaskGetTickCount();

    for (;;) {
        const float rawTemperature = context->sensor->ReadTemperature();
        const float filteredTemperature = context->filter->Update(rawTemperature);
        const float speed = context->calculator->Calculate(kDefaultSetpoint, filteredTemperature);

        taskENTER_CRITICAL();
        g_data.temperature = filteredTemperature;
        g_data.speed = speed;
        g_data.new_data = true;
        taskEXIT_CRITICAL();

        vTaskDelayUntil(&lastWakeTime, kSensorTaskPeriod);
    }
}
}

void SensorTask_Start(ISensor& sensor, IFilter& filter, ICalculator& calculator) {
    static SensorTaskContext context;
    context.sensor = &sensor;
    context.filter = &filter;
    context.calculator = &calculator;

    context.sensor->Init();
    xTaskCreate(SensorTaskFunction, "Sensor", 256, &context, 3, NULL);
}

void SensorTask_Start(void) {
    SensorTask_Start(*defaultContext.sensor, *defaultContext.filter, *defaultContext.calculator);
}
