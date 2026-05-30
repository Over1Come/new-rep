#include "SpeedCalculator.h"

SpeedCalculator::SpeedCalculator(float t_min, float t_max)
    : t_min_(t_min)
    , t_max_(t_max)
    , out_min_(0.0f)
    , out_max_(100.0f)
{
}

void SpeedCalculator::SetGains(float kp, float ki, float kd)
{
    (void)kp;
    (void)ki;
    (void)kd;
}

void SpeedCalculator::SetOutputLimits(float min, float max)
{
    out_min_ = min;
    out_max_ = max;
}

float SpeedCalculator::Calculate(float setpoint, float measurement)
{
    (void)setpoint;

    float temperature = measurement;
    float speed = 0.0f;

    if (temperature <= t_min_)
    {
        speed = 0.0f;
    }
    else if (temperature >= t_max_)
    {
        speed = 100.0f;
    }
    else
    {
        float range = t_max_ - t_min_;
        float excess = temperature - t_min_;
        speed = (excess / range) * 100.0f;
    }

    if (speed < out_min_) speed = out_min_;
    if (speed > out_max_) speed = out_max_;

    return speed;
}
