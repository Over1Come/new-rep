#include "BME280.h"
#include "../I2C/I2C.h"

namespace {
const uint8_t kBme280RegReset = 0xE0;
const uint8_t kBme280RegCtrlMeas = 0xF4;
const uint8_t kBme280RegTempMsb = 0xFA;
const uint8_t kBme280ResetCommand = 0xB6;
const uint8_t kBme280CalibrationReg = 0x88;
const int kResetDelayCycles = 100000;
const int kMeasureDelayCycles = 50000;
}

BME280::BME280(uint8_t i2cAddress)
    : address_(i2cAddress),
      digT1_(0),
      digT2_(0),
      digT3_(0),
      tFine_(0) {
}

bool BME280::Init() {
    begin();
    return true;
}

float BME280::ReadTemperature() {
    return readTemperature100() / 100.0f;
}

void BME280::begin() {
    writeReg(kBme280RegReset, kBme280ResetCommand);

    for (volatile int i = 0; i < kResetDelayCycles; ++i) {
        __asm volatile("nop");
    }

    readCalibrationData();
    writeReg(kBme280RegCtrlMeas, (0x01 << 5) | (0x00 << 2) | 0x01);
}

int32_t BME280::readTemperature100() {
    writeReg(kBme280RegCtrlMeas, (0x01 << 5) | (0x00 << 2) | 0x01);

    for (volatile int i = 0; i < kMeasureDelayCycles; ++i) {
        __asm volatile("nop");
    }

    uint8_t data[3];
    readRegs(kBme280RegTempMsb, data, 3);

    const int32_t adcTemperature =
        (static_cast<int32_t>(data[0]) << 12) |
        (static_cast<int32_t>(data[1]) << 4) |
        (static_cast<int32_t>(data[2]) >> 4);

    const int32_t var1 =
        (((adcTemperature >> 3) - (static_cast<int32_t>(digT1_) << 1)) *
         static_cast<int32_t>(digT2_)) >> 11;

    const int32_t var2 =
        (((((adcTemperature >> 4) - static_cast<int32_t>(digT1_)) *
           ((adcTemperature >> 4) - static_cast<int32_t>(digT1_))) >> 12) *
         static_cast<int32_t>(digT3_)) >> 14;

    tFine_ = var1 + var2;
    return (tFine_ * 5 + 128) >> 8;
}

void BME280::writeReg(uint8_t reg, uint8_t value) {
    I2C_Start();
    I2C_WriteByte(address_ << 1);
    I2C_WriteByte(reg);
    I2C_WriteByte(value);
    I2C_Stop();
}

void BME280::readRegs(uint8_t reg, uint8_t* buffer, uint8_t length) {
    I2C_Start();
    I2C_WriteByte(address_ << 1);
    I2C_WriteByte(reg);
    I2C_Start();
    I2C_WriteByte((address_ << 1) | 1);

    for (uint8_t i = 0; i < length - 1; ++i) {
        buffer[i] = I2C_ReadByte(1);
    }

    buffer[length - 1] = I2C_ReadByte(0);
    I2C_Stop();
}

void BME280::readCalibrationData() {
    uint8_t calibration[6];
    readRegs(kBme280CalibrationReg, calibration, 6);

    digT1_ = static_cast<uint16_t>(calibration[0] | (calibration[1] << 8));
    digT2_ = static_cast<int16_t>(calibration[2] | (calibration[3] << 8));
    digT3_ = static_cast<int16_t>(calibration[4] | (calibration[5] << 8));
}
