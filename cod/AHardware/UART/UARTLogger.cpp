#include "UARTLogger.h"

#include "UARTFormatter.h"
#include "USART.h"

void UARTLogger::Init(void) {
    USART::Init(9600);
}

void UARTLogger::PutChar(char ch) {
    USART::SendChar(ch);
}

void UARTLogger::PutString(const char* text) {
    USART::SendString(text);
}

void UARTLogger::LogTemperature(float temperature) {
    char buffer[48];
    UARTFormatter formatter(1, 0);

    formatter.FormatTemperature(temperature, buffer);
    PutString(buffer);
}

void UARTLogger::LogSpeed(float speed) {
    char buffer[32];
    UARTFormatter formatter(1, 0);

    formatter.FormatSpeed(speed, buffer);
    PutString(buffer);
}
