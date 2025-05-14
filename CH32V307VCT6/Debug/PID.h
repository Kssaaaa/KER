#ifndef __PID_H__
#define __PID_H__

#include <stdint.h>

typedef struct {
    float Kp;           // 比例系数
    float Ki;           // 积分系数
    float Kd;           // 微分系数
    
    float max_output;   // 最大输出限制
    float min_output;   // 最小输出限制
    
    float max_integral; // 积分项上限(抗积分饱和)
    float min_integral; // 积分项下限
    
    float last_error;   // 上一次误差
    float integral;     // 积分累积值
    float last_output;  // 上一次输出(可选)
} PIDController;

// 初始化PID控制器
void PID_Init(PIDController *pid, float Kp, float Ki, float Kd, 
              float max_output, float min_output, 
              float max_integral, float min_integral);

// 重置PID控制器(清除积分和上次误差)
void PID_Reset(PIDController *pid);

// 计算PID输出
float PID_Calculate(PIDController *pid, float setpoint, float measured_value);

// 设置PID参数
void PID_SetTunings(PIDController *pid, float Kp, float Ki, float Kd);

// 设置输出限制
void PID_SetOutputLimits(PIDController *pid, float min, float max);

// 设置积分限制
void PID_SetIntegralLimits(PIDController *pid, float min, float max);

#endif 