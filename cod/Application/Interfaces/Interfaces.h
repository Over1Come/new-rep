#ifndef INTERFACES_H
#define INTERFACES_H

class ISensor
{
public:
    virtual ~ISensor() = default;
    virtual bool Init() = 0;
    virtual float ReadTemperature() = 0;
};

class IFilter
{
public:
    virtual ~IFilter() = default;
    virtual float Update(float raw_value) = 0;
};

class ICalculator
{
public:
    virtual ~ICalculator() = default;
    virtual void SetGains(float kp, float ki, float kd) = 0;
    virtual void SetOutputLimits(float min, float max) = 0;
    virtual float Calculate(float setpoint, float measurement) = 0;
};

class IDisplay
{
public:
    virtual ~IDisplay() = default;
    virtual void Init() = 0;
    virtual void ShowTemperature(float temp) = 0;
    virtual void ShowSpeed(float speed) = 0;
    virtual void Clear() = 0;
};

class ILedIndicator
{
public:
    virtual ~ILedIndicator() = default;
    virtual void Init() = 0;
    virtual void Update(float speed) = 0;
};

class ILogger
{
public:
    virtual ~ILogger() = default;
    virtual void Init() = 0;
    virtual void LogTemperature(float temp) = 0;
    virtual void LogSpeed(float speed) = 0;
};

#endif