#include "UARTFormatter.h"

UARTFormatter::UARTFormatter(int precision_temp, int precision_speed)
    : temp_precision_(precision_temp)
    , speed_precision_(precision_speed)
{
}

char* UARTFormatter::FormatFloat(float value, char* buffer, int precision)
{
    char* p = buffer;
    
    if (value < 0)
    {
        *p++ = '-';
        value = -value;
    }
    
    int int_part = (int)value;
    
    if (int_part >= 100) *p++ = '0' + (int_part / 100);
    if (int_part >= 10) *p++ = '0' + (int_part / 10);
    *p++ = '0' + (int_part % 10);
    
    if (precision > 0)
    {
        *p++ = '.';
        float frac = value - (int)value;
        if (frac < 0) frac = -frac;
        
        for (int i = 0; i < precision; i++)
        {
            frac *= 10;
            int digit = (int)frac;
            *p++ = '0' + digit;
            frac -= digit;
        }
    }
    
    return p;
}

char* UARTFormatter::FormatTemperature(float temp, char* buffer)
{
    char* p = buffer;
    
    const char* prefix = "Temperature: ";
    while (*prefix) *p++ = *prefix++;
    
    p = FormatFloat(temp, p, temp_precision_);
    
    *p++ = ' ';
    *p++ = 'C';
    *p++ = '\r';
    *p++ = '\n';
    *p = '\0';
    
    return buffer;
}

char* UARTFormatter::FormatSpeed(float speed, char* buffer)
{
    char* p = buffer;
    
    const char* prefix = "Speed: ";
    while (*prefix) *p++ = *prefix++;
    
    p = FormatFloat(speed, p, speed_precision_);
    
    *p++ = ' ';
    *p++ = '%';
    *p++ = '\r';
    *p++ = '\n';
    *p = '\0';
    
    return buffer;
}

void UARTFormatter::SetTemperaturePrecision(int precision)
{
    temp_precision_ = precision;
}

void UARTFormatter::SetSpeedPrecision(int precision)
{
    speed_precision_ = precision;
}