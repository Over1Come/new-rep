#ifndef BME280_H
#define BME280_H

#include <cstdint>
#include "Interfaces.h"

class BME280 : public ISensor {
public:
    explicit BME280(uint8_t i2cAddress);

    bool Init() override;
    float ReadTemperature() override;

    void begin();
    int32_t readTemperature100();

private:
    void writeReg(uint8_t reg, uint8_t value);
    void readRegs(uint8_t reg, uint8_t* buffer, uint8_t length);
    void readCalibrationData();

    uint8_t address_;
    uint16_t digT1_;
    int16_t digT2_;
    int16_t digT3_;
    int32_t tFine_;
};

#endif
