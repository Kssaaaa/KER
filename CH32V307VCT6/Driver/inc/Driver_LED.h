#ifndef __DRIVER_LED_H
#define __DRIVER_LED_H

#include "ch32v30x.h"

#define LED_1 GPIO_OUTDR_ODR0
#define LED_2 GPIO_OUTDR_ODR1
#define LED_3 GPIO_OUTDR_ODR8

void Driver_LED_Init(void);

void Drviver_LED_On(uint32_t led);

void Drviver_LED_Off(uint32_t led);

void Drviver_LED_Toggle(uint32_t led);

void Drviver_LED_OnAll(uint32_t leds[], uint8_t size);

void Drviver_LED_OffAll(uint32_t leds[], uint8_t size);

#endif