#ifndef DISPLAYFORMATTER_H
#define DISPLAYFORMATTER_H

class DisplayFormatter
{
public:
    DisplayFormatter(void);
    char* FormatTemperature(float temp, char* buffer);
    char* FormatSpeed(float speed, char* buffer);
};

#endif