#ifndef LIQUIDCRYSTAL_I2C_H
#define LIQUIDCRYSTAL_I2C_H

#include <cstdint>
#include "Interfaces.h"

class LiquidCrystal_I2C : public IDisplay {
public:
    LiquidCrystal_I2C(uint8_t address, uint8_t columns, uint8_t rows);

    void Init(void) override;
    void Clear(void) override;
    void ShowTemperature(float temperature) override;
    void ShowSpeed(float speed) override;

    void Backlight(void);
    void NoBacklight(void);
    void SetCursor(uint8_t column, uint8_t row);
    void Print(const char* text);
    void Print(float number, int decimals);

private:
    void ExpanderWrite(uint8_t data);
    void PulseEnable(uint8_t data);
    void Write4Bits(uint8_t data);
    void Send(uint8_t data, uint8_t mode);

    uint8_t address_;
    uint8_t columns_;
    uint8_t rows_;
    uint8_t backlight_;
};

#endif
