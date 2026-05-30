#ifndef TIMER_H
#define TIMER_H

#include <cstdint>

class Timer
{
public:
    static void Init();
    static bool IsMeasurementReady();
    static void ClearFlag();

    static volatile bool sMeasurementReady;  
};

#endif