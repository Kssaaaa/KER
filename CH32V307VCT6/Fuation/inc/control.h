#ifndef __CONTROL_H
#define __CONTROL_H

#include "ch32v30x.h"

void SetAllMotors(int8_t lf_speed, int8_t rf_speed, int8_t lb_speed, int8_t rb_speed);
void BrakeAll(void);
void EnableDriver(FunctionalState state);

#endif