#ifndef __DRIVER_TIM1_H
#define __DRIVER_TIM1_H

#include "ch32v30x.h"

void TIM1_PWM_Init(void);
void PWM_SetCompare(uint16_t Compare1,uint16_t Compare2,uint16_t Compare3,uint16_t Compare4);

#endif
