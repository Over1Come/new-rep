#include "LEDIndicator.h"

#include <stdint.h>

namespace {
#define RCC_BASE        0x40023800UL
#define RCC_AHB1ENR     (*((volatile uint32_t*)(RCC_BASE + 0x30)))
#define RCC_APB1ENR     (*((volatile uint32_t*)(RCC_BASE + 0x40)))

#define GPIOA_BASE      0x40020000UL
#define GPIOA_MODER     (*((volatile uint32_t*)(GPIOA_BASE + 0x00)))
#define GPIOA_AFRL      (*((volatile uint32_t*)(GPIOA_BASE + 0x20)))

#define GPIOC_BASE      0x40020800UL
#define GPIOC_MODER     (*((volatile uint32_t*)(GPIOC_BASE + 0x00)))
#define GPIOC_BSRR      (*((volatile uint32_t*)(GPIOC_BASE + 0x18)))

#define TIM2_BASE       0x40000000UL
#define TIM2_CR1        (*((volatile uint32_t*)(TIM2_BASE + 0x00)))
#define TIM2_EGR        (*((volatile uint32_t*)(TIM2_BASE + 0x14)))
#define TIM2_CCMR1      (*((volatile uint32_t*)(TIM2_BASE + 0x18)))
#define TIM2_CCER       (*((volatile uint32_t*)(TIM2_BASE + 0x20)))
#define TIM2_PSC        (*((volatile uint32_t*)(TIM2_BASE + 0x28)))
#define TIM2_ARR        (*((volatile uint32_t*)(TIM2_BASE + 0x2C)))
#define TIM2_CCR1       (*((volatile uint32_t*)(TIM2_BASE + 0x34)))

const uint32_t kMaxPwmDuty = 999;

void delayAfterClockEnable() {
    for (volatile int i = 0; i < 100; ++i) {
    }
}
}

void LEDIndicator::Init(void) {
    RCC_AHB1ENR |= (1UL << 0) | (1UL << 2);
    RCC_APB1ENR |= (1UL << 0);
    delayAfterClockEnable();

    GPIOA_MODER &= ~(3UL << 10);
    GPIOA_MODER |= (2UL << 10);

    GPIOA_AFRL &= ~(0xFUL << 20);
    GPIOA_AFRL |= (1UL << 20);

    GPIOC_MODER &= ~((3UL << 18) | (3UL << 16) | (3UL << 10));
    GPIOC_MODER |= (1UL << 18) | (1UL << 16) | (1UL << 10);

    GPIOC_BSRR = (1UL << 25) | (1UL << 24) | (1UL << 21);

    TIM2_PSC = 3;
    TIM2_ARR = kMaxPwmDuty;
    TIM2_CCMR1 = (TIM2_CCMR1 & ~(7 << 4)) | (6 << 4);
    TIM2_CCMR1 |= (1 << 3);
    TIM2_CCER |= (1 << 0);
    TIM2_CCR1 = 0;
    TIM2_CR1 |= (1 << 7);
    TIM2_CR1 |= (1 << 0);
    TIM2_EGR |= (1 << 0);

    delayAfterClockEnable();
}

void LEDIndicator::Update(float speed) {
    uint32_t duty = static_cast<uint32_t>(speed / 100.0f * kMaxPwmDuty);

    if (duty > kMaxPwmDuty) {
        duty = kMaxPwmDuty;
    }

    TIM2_CCR1 = duty;
    GPIOC_BSRR = (speed >= 30.0f) ? (1UL << 9) : (1UL << 25);
    GPIOC_BSRR = (speed >= 60.0f) ? (1UL << 8) : (1UL << 24);
    GPIOC_BSRR = (speed >= 90.0f) ? (1UL << 5) : (1UL << 21);
}
