#include "Timer.h"
#include "rccregisters.hpp"

#define TIM2_PSC      (*((volatile uint32_t*)0x40000028))
#define TIM2_ARR      (*((volatile uint32_t*)0x4000002C))
#define TIM2_SR       (*((volatile uint32_t*)0x40000010))
#define TIM2_CNT      (*((volatile uint32_t*)0x40000024))
#define TIM2_DIER     (*((volatile uint32_t*)0x4000000C))
#define TIM2_CR1      (*((volatile uint32_t*)0x40000000))
#define NVIC_ISER0    (*((volatile uint32_t*)0xE000E100))

volatile bool Timer::sMeasurementReady = false;

void Timer::Init()
{
    RCC::APB1ENR::TIM2EN::Enable::Set();
    
    TIM2_PSC = 16000 - 1;
    TIM2_ARR = 50 - 1;
    TIM2_SR &= ~(1 << 0);
    TIM2_CNT = 0;
    TIM2_DIER |= (1 << 0);
    
    NVIC_ISER0 |= (1 << 28);
    
    TIM2_CR1 |= (1 << 0);
}

bool Timer::IsMeasurementReady()
{
    return sMeasurementReady;
}

void Timer::ClearFlag()
{
    sMeasurementReady = false;
}

extern "C" void TIM2_IRQHandler(void)
{
    if (TIM2_SR & (1 << 0))
    {
        TIM2_SR &= ~(1 << 0);
        Timer::sMeasurementReady = true;
    }
}