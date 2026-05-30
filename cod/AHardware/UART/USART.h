#ifndef USART_H
#define USART_H

#include <cstdint>

class USART
{
public:
    static void Init(uint32_t baudrate);
    static void SendChar(char c);
    static void SendString(const char* str);
};

#endif