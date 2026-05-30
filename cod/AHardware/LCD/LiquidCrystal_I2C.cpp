#include "LiquidCrystal_I2C.h"

#include "../I2C/I2C.h"

namespace {
const uint8_t kEnableBit = 0x04;
const uint8_t kBacklightOn = 0x08;
const uint8_t kBacklightOff = 0x00;
const uint8_t kCommandMode = 0;
const uint8_t kDataMode = 1;
}

LiquidCrystal_I2C::LiquidCrystal_I2C(uint8_t address, uint8_t columns, uint8_t rows)
    : address_(address),
      columns_(columns),
      rows_(rows),
      backlight_(kBacklightOn) {
}

void LiquidCrystal_I2C::ExpanderWrite(uint8_t data) {
    I2C_Start();
    I2C_WriteByte((address_ << 1) | 0);
    I2C_WriteByte(data | backlight_);
    I2C_Stop();
}

void LiquidCrystal_I2C::PulseEnable(uint8_t data) {
    ExpanderWrite(data | kEnableBit);
    I2C_DelayUs(1);
    ExpanderWrite(data & ~kEnableBit);
    I2C_DelayUs(50);
}

void LiquidCrystal_I2C::Write4Bits(uint8_t data) {
    ExpanderWrite(data);
    PulseEnable(data);
}

void LiquidCrystal_I2C::Send(uint8_t data, uint8_t mode) {
    const uint8_t highBits = (data & 0xF0) | mode;
    const uint8_t lowBits = ((data << 4) & 0xF0) | mode;

    Write4Bits(highBits);
    Write4Bits(lowBits);
}

void LiquidCrystal_I2C::Init(void) {
    I2C_DelayMs(50);

    Write4Bits(0x03 << 4);
    I2C_DelayMs(5);

    Write4Bits(0x03 << 4);
    I2C_DelayUs(150);

    Write4Bits(0x03 << 4);
    I2C_DelayUs(150);

    Write4Bits(0x02 << 4);

    Send(0x28, kCommandMode);
    Send(0x0C, kCommandMode);
    Clear();
    Send(0x06, kCommandMode);
}

void LiquidCrystal_I2C::Backlight(void) {
    backlight_ = kBacklightOn;
    ExpanderWrite(0x00);
}

void LiquidCrystal_I2C::NoBacklight(void) {
    backlight_ = kBacklightOff;
    ExpanderWrite(0x00);
}

void LiquidCrystal_I2C::Clear(void) {
    Send(0x01, kCommandMode);
    I2C_DelayMs(2);
}

void LiquidCrystal_I2C::SetCursor(uint8_t column, uint8_t row) {
    static const uint8_t kRowOffsets[] = {0x00, 0x40};
    Send(0x80 | (column + kRowOffsets[row]), kCommandMode);
}

void LiquidCrystal_I2C::Print(const char* text) {
    while (*text != '\0') {
        Send(*text, kDataMode);
        ++text;
    }
}

void LiquidCrystal_I2C::Print(float number, int decimals) {
    int integerPart = static_cast<int>(number);

    if (integerPart < 0) {
        Send('-', kDataMode);
        integerPart = -integerPart;
    }

    if (integerPart >= 100) {
        Send('0' + (integerPart / 100), kDataMode);
    }

    if (integerPart >= 10) {
        Send('0' + ((integerPart / 10) % 10), kDataMode);
    }

    Send('0' + (integerPart % 10), kDataMode);

    if (decimals > 0) {
        Send('.', kDataMode);
        float fraction = number - static_cast<int>(number);

        if (fraction < 0) {
            fraction = -fraction;
        }

        for (int i = 0; i < decimals; ++i) {
            fraction *= 10;
            const int digit = static_cast<int>(fraction);
            Send('0' + digit, kDataMode);
            fraction -= digit;
        }
    }
}

void LiquidCrystal_I2C::ShowTemperature(float temperature) {
    SetCursor(0, 0);
    Print("Temp: ");
    Print(temperature, 1);
    Print(" C   ");
}

void LiquidCrystal_I2C::ShowSpeed(float speed) {
    SetCursor(0, 1);
    Print("Speed: ");
    Print(speed, 0);
    Print(" %   ");
}
