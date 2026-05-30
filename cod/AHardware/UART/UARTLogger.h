#ifndef UARTLOGGER_H
#define UARTLOGGER_H

#include "Interfaces.h"

class UARTLogger : public ILogger {
public:
    void Init(void) override;
    void LogTemperature(float temperature) override;
    void LogSpeed(float speed) override;

private:
    void PutChar(char ch);
    void PutString(const char* text);
};

#endif
