#include "USART.h"
#include "rccregisters.hpp"
#include "gpioaregisters.hpp"
#include "usart2registers.hpp"

void USART::Init(uint32_t baudrate)
{
    RCC::APB1ENR::USART2EN::Enable::Set();
    RCC::AHB1ENR::GPIOAEN::Enable::Set();
    
    GPIOA::MODER::MODER2::Alternate::Set();
    GPIOA::MODER::MODER3::Alternate::Set();
    GPIOA::AFRL::AFRL2::Af7::Set();
    GPIOA::AFRL::AFRL3::Af7::Set();
    
    uint32_t uartdiv = 4000000 / baudrate;
    USART2::BRR::DIV_Mantissa::Set(uartdiv >> 4);
    USART2::BRR::DIV_Fraction::Set(uartdiv & 0x0F);
    
    USART2::CR1::TE::Enable::Set();
    USART2::CR1::RE::Enable::Set();
    USART2::CR1::UE::Enable::Set();
}

void USART::SendChar(char c)
{
    while (USART2::SR::TXE::Get() == 0) {}
    USART2::DR::DRField::Set(static_cast<uint8_t>(c));
}

void USART::SendString(const char* str)
{
    while (*str)
    {
        SendChar(*str++);
    }
}