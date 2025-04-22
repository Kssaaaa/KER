#ifndef __DRVIER_USART_H
#define __DRVIER_USART_H

#include "ch32v30x.h"
#include "stdio.h"
#include "debug.h"

#define HAL_MAX_DELAY  0xFFFFFFFF

void Driver_USART1_Init(void);

void Driver_USART2_Init(void);

void MX_USART2_UART_Init(void);

void Driver_USART1_SendChar(uint8_t byte);

void Driver_USART1_SendString(uint8_t *str, uint16_t len);

uint8_t Driver_USART1_ReceiveChar(void);

void Driver_USART1_ReceiveString(uint8_t buff[], uint8_t *len);

void UART_Transmit(USART_TypeDef* USARTx, uint8_t *pData, uint16_t Size, uint32_t Timeout);

#endif

