#ifndef __PID_H__
#define __PID_H__

#include <stdint.h>

typedef struct {
    float Kp;           // ����ϵ��
    float Ki;           // ����ϵ��
    float Kd;           // ΢��ϵ��
    
    float max_output;   // ����������
    float min_output;   // ��С�������
    
    float max_integral; // ����������(�����ֱ���)
    float min_integral; // ����������
    
    float last_error;   // ��һ�����
    float integral;     // �����ۻ�ֵ
    float last_output;  // ��һ�����(��ѡ)
} PIDController;

// ��ʼ��PID������
void PID_Init(PIDController *pid, float Kp, float Ki, float Kd, 
              float max_output, float min_output, 
              float max_integral, float min_integral);

// ����PID������(������ֺ��ϴ����)
void PID_Reset(PIDController *pid);

// ����PID���
float PID_Calculate(PIDController *pid, float setpoint, float measured_value);

// ����PID����
void PID_SetTunings(PIDController *pid, float Kp, float Ki, float Kd);

// �����������
void PID_SetOutputLimits(PIDController *pid, float min, float max);

// ���û�������
void PID_SetIntegralLimits(PIDController *pid, float min, float max);

#endif 