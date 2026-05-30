#ifndef SPEEDCALCULATOR_H
#define SPEEDCALCULATOR_H

#include "Interfaces.h"

class SpeedCalculator : public ICalculator
{
public:
    SpeedCalculator(float t_min, float t_max);

    void SetGains(float kp, float ki, float kd) override;
    void SetOutputLimits(float min, float max) override;
    float Calculate(float setpoint, float measurement) override;

private:
    float t_min_;
    float t_max_;
    float out_min_;
    float out_max_;
};

#endif
