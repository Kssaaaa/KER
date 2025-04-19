#ifndef __DRIVER_TIM4_H
#define __DRIVER_TIM4_H

#include "ch32v30x.h"


void Driver_TIM4_Init(void);
void Driver_TIM4_Start(void);
double Driver_TIM4_GetPWMCycle(void);
double Driver_TIM4_GetPWMFreq(void);

double Driver_TIM4_GetDutyCycle(void);

#endif
