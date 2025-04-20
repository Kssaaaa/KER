#ifndef __DRIVER_I2C2_H
#define __DRIVER_I2C2_H

#include "ch32v30x.h"
#include "debug.h"
#include "Driver_USART.h"
/*
        PB6->SCL
        PB7->SDA
 */
#define ACK 0
#define NACK 1

#define SCL_HIGH  (GPIOB->OUTDR |= GPIO_OUTDR_ODR6)
#define SCL_LOW  (GPIOB->OUTDR &= ~GPIO_OUTDR_ODR6)

#define SDA_HIGH  (GPIOB->OUTDR |= GPIO_OUTDR_ODR7)
#define SDA_LOW  (GPIOB->OUTDR &= ~GPIO_OUTDR_ODR7)

#define READ_SDA (GPIOB->INDR & GPIO_INDR_IDR7)


void Driver_I2C2_Init(void);

void Driver_I2C2_Start(void);

void Driver_I2C2_Stop(void);

void Driver_I2C2_Ack(void);

void Driver_I2C2_NAck(void);

uint8_t Driver_I2C2_WaitAck(void);

void Driver_I2C_SendByte(uint8_t byte);

uint8_t Driver_I2C_ReadByte(void);
uint8_t Driver_I2C_ReadByteAck(uint8_t ack);

#endif

