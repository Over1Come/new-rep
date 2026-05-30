#include "I2C.h"
#include "rccregisters.hpp"
#include "gpiobregisters.hpp"

static void delay_us(uint32_t us)
{
    for (uint32_t i = 0; i < us * 8; ++i)
    {
        __asm volatile("nop");
    }
}

void I2C_Init(void)
{
    RCC::AHB1ENR::GPIOBEN::Enable::Set();
    
    uint32_t moder = GPIOB::MODER::Get();
    moder &= ~((3 << 16) | (3 << 18));
    moder |= (1 << 16) | (1 << 18);
    GPIOB::MODER::Write(moder);
    
    uint32_t otyper = GPIOB::OTYPER::Get();
    otyper |= (1 << 8) | (1 << 9);
    GPIOB::OTYPER::Write(otyper);
    
    uint32_t pupdr = GPIOB::PUPDR::Get();
    pupdr &= ~((3 << 16) | (3 << 18));
    pupdr |= (1 << 16) | (1 << 18);
    GPIOB::PUPDR::Write(pupdr);
    
    GPIOB::BSRR::Write((1 << 8) | (1 << 9));
}

void I2C_Start(void)
{
    GPIOB::BSRR::Write(1 << 8);
    GPIOB::BSRR::Write(1 << 9);
    delay_us(5);
    GPIOB::BSRR::Write(1 << (9 + 16));
    delay_us(5);
    GPIOB::BSRR::Write(1 << (8 + 16));
}

void I2C_Stop(void)
{
    GPIOB::BSRR::Write(1 << (9 + 16));
    delay_us(5);
    GPIOB::BSRR::Write(1 << 8);
    delay_us(5);
    GPIOB::BSRR::Write(1 << 9);
    delay_us(5);
}

uint8_t I2C_WriteByte(uint8_t data)
{
    for (int i = 7; i >= 0; i--)
    {
        if (data & (1 << i))
        {
            GPIOB::BSRR::Write(1 << 9);
        }
        else
        {
            GPIOB::BSRR::Write(1 << (9 + 16));
        }
        delay_us(1);
        GPIOB::BSRR::Write(1 << 8);
        delay_us(3);
        GPIOB::BSRR::Write(1 << (8 + 16));
        delay_us(2);
    }
    
    GPIOB::BSRR::Write(1 << 9);
    GPIOB::MODER::Write(GPIOB::MODER::Get() & ~(3UL << (9 * 2)));
    delay_us(1);
    GPIOB::BSRR::Write(1 << 8);
    delay_us(2);
    
    uint8_t ack = !(GPIOB::IDR::Get() & (1 << 9));
    
    GPIOB::BSRR::Write(1 << (8 + 16));
    GPIOB::MODER::Write((GPIOB::MODER::Get() & ~(3UL << (9 * 2))) | (1UL << (9 * 2)));
    delay_us(1);
    
    return ack;
}

uint8_t I2C_ReadByte(uint8_t ack)
{
    uint8_t data = 0;
    
    GPIOB::BSRR::Write(1 << 9);
    GPIOB::MODER::Write(GPIOB::MODER::Get() & ~(3UL << (9 * 2)));
    delay_us(2);
    
    for (int i = 7; i >= 0; i--)
    {
        GPIOB::BSRR::Write(1 << 8);
        delay_us(5);
        if (GPIOB::IDR::Get() & (1 << 9))
        {
            data |= (1 << i);
        }
        GPIOB::BSRR::Write(1 << (8 + 16));
        delay_us(5);
    }
    
    GPIOB::MODER::Write((GPIOB::MODER::Get() & ~(3UL << (9 * 2))) | (1UL << (9 * 2)));
    
    if (ack)
    {
        GPIOB::BSRR::Write(1 << (9 + 16));
    }
    else
    {
        GPIOB::BSRR::Write(1 << 9);
    }
    
    delay_us(2);
    GPIOB::BSRR::Write(1 << 8);
    delay_us(5);
    GPIOB::BSRR::Write(1 << (8 + 16));
    delay_us(2);
    GPIOB::BSRR::Write(1 << 9);
    
    return data;
}

void I2C_DelayMs(uint32_t ms)
{
    for (uint32_t i = 0; i < ms; ++i)
    {
        for (volatile int j = 0; j < 10000; ++j)
        {
            __asm volatile("nop");
        }
    }
}

void I2C_DelayUs(uint32_t us)
{
    delay_us(us);
}