#ifndef __ESP32_H
#define __ESP32_H

#include "Driver_USART.h"
#include "debug.h"
#include "string.h"
#include "stdio.h"

#define HAL_Delay Delay_Ms

void ESP32_Init(void);
void ESP32_Send_CMD(uint8_t *cmd, uint16_t cmdLength);
void ESP32_ReadResponse(uint8_t responeBuff[], uint16_t size);

#endif