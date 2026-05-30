#include "DisplayFormatter.h"

DisplayFormatter::DisplayFormatter(void) {}

char* DisplayFormatter::FormatTemperature(float temp, char* buffer)
{
    char* p = buffer;
    
    int int_part = (int)temp;
    int frac_part = (int)((temp - int_part) * 10);
    if (frac_part < 0) frac_part = -frac_part;
    
    *p++ = 'T';
    *p++ = ':';
    *p++ = ' ';
    
    if (int_part < 0)
    {
        *p++ = '-';
        int_part = -int_part;
    }
    
    if (int_part >= 10) *p++ = '0' + (int_part / 10);
    *p++ = '0' + (int_part % 10);
    *p++ = '.';
    *p++ = '0' + frac_part;
    *p++ = ' ';
    *p++ = 'C';
    *p = '\0';
    
    return buffer;
}

char* DisplayFormatter::FormatSpeed(float speed, char* buffer)
{
    char* p = buffer;
    int speed_int = (int)speed;
    
    const char* prefix = "Speed: ";
    while (*prefix) *p++ = *prefix++;
    
    if (speed_int >= 100) *p++ = '0' + (speed_int / 100);
    if (speed_int >= 10) *p++ = '0' + ((speed_int / 10) % 10);
    *p++ = '0' + (speed_int % 10);
    *p++ = '%';
    *p = '\0';
    
    return buffer;
}