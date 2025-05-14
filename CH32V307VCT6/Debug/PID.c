#include "pid.h"
#include <math.h>

// 初始化PID控制器
void PID_Init(PIDController *pid, float Kp, float Ki, float Kd, 
              float max_output, float min_output, 
              float max_integral, float min_integral)
{
    pid->Kp = Kp;
    pid->Ki = Ki;
    pid->Kd = Kd;
    
    pid->max_output = max_output;
    pid->min_output = min_output;
    
    pid->max_integral = max_integral;
    pid->min_integral = min_integral;
    
    pid->last_error = 0.0f;
    pid->integral = 0.0f;
    pid->last_output = 0.0f;
}

// 重置PID控制器
void PID_Reset(PIDController *pid)
{
    pid->last_error = 0.0f;
    pid->integral = 0.0f;
    pid->last_output = 0.0f;
}

// 计算PID输出
float PID_Calculate(PIDController *pid, float setpoint, float measured_value)
{
    // 计算误差
    float error = setpoint - measured_value;
    
    // 比例项
    float proportional = pid->Kp * error;
    
    // 积分项(带抗积分饱和)
    pid->integral += pid->Ki * error;
    
    // 限制积分项
    if(pid->integral > pid->max_integral) {
        pid->integral = pid->max_integral;
    } else if(pid->integral < pid->min_integral) {
        pid->integral = pid->min_integral;
    }
    
    // 微分项
    float derivative = pid->Kd * (error - pid->last_error);
    pid->last_error = error;
    
    // 计算输出
    float output = proportional + pid->integral + derivative;
    
    // 限制输出
    if(output > pid->max_output) {
        output = pid->max_output;
    } else if(output < pid->min_output) {
        output = pid->min_output;
    }
    
    pid->last_output = output;
    return output;
}

// 设置PID参数
void PID_SetTunings(PIDController *pid, float Kp, float Ki, float Kd)
{
    pid->Kp = Kp;
    pid->Ki = Ki;
    pid->Kd = Kd;
}

// 设置输出限制
void PID_SetOutputLimits(PIDController *pid, float min, float max)
{
    pid->min_output = min;
    pid->max_output = max;
}

// 设置积分限制
void PID_SetIntegralLimits(PIDController *pid, float min, float max)
{
    pid->min_integral = min;
    pid->max_integral = max;
}