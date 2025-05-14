#include "pid.h"
#include <math.h>

// ��ʼ��PID������
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

// ����PID������
void PID_Reset(PIDController *pid)
{
    pid->last_error = 0.0f;
    pid->integral = 0.0f;
    pid->last_output = 0.0f;
}

// ����PID���
float PID_Calculate(PIDController *pid, float setpoint, float measured_value)
{
    // �������
    float error = setpoint - measured_value;
    
    // ������
    float proportional = pid->Kp * error;
    
    // ������(�������ֱ���)
    pid->integral += pid->Ki * error;
    
    // ���ƻ�����
    if(pid->integral > pid->max_integral) {
        pid->integral = pid->max_integral;
    } else if(pid->integral < pid->min_integral) {
        pid->integral = pid->min_integral;
    }
    
    // ΢����
    float derivative = pid->Kd * (error - pid->last_error);
    pid->last_error = error;
    
    // �������
    float output = proportional + pid->integral + derivative;
    
    // �������
    if(output > pid->max_output) {
        output = pid->max_output;
    } else if(output < pid->min_output) {
        output = pid->min_output;
    }
    
    pid->last_output = output;
    return output;
}

// ����PID����
void PID_SetTunings(PIDController *pid, float Kp, float Ki, float Kd)
{
    pid->Kp = Kp;
    pid->Ki = Ki;
    pid->Kd = Kd;
}

// �����������
void PID_SetOutputLimits(PIDController *pid, float min, float max)
{
    pid->min_output = min;
    pid->max_output = max;
}

// ���û�������
void PID_SetIntegralLimits(PIDController *pid, float min, float max)
{
    pid->min_integral = min;
    pid->max_integral = max;
}