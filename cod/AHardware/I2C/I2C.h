#ifndef I2C_H
#define I2C_H

#include <cstdint>

void I2C_Init(void);
void I2C_Start(void);
void I2C_Stop(void);
uint8_t I2C_WriteByte(uint8_t data);
uint8_t I2C_ReadByte(uint8_t ack);
void I2C_DelayMs(uint32_t ms);
void I2C_DelayUs(uint32_t us);

#endif