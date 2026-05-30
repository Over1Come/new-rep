#ifndef UARTFORMATTER_H
#define UARTFORMATTER_H

class UARTFormatter
{
public:
    UARTFormatter(int precision_temp, int precision_speed);
    char* FormatTemperature(float temp, char* buffer);
    char* FormatSpeed(float speed, char* buffer);
    void SetTemperaturePrecision(int precision);
    void SetSpeedPrecision(int precision);

private:
    char* FormatFloat(float value, char* buffer, int precision);
    int temp_precision_;
    int speed_precision_;
};

#endif