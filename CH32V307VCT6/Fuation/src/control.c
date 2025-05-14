#include "control.h"

// PB-> 01 23 45 67
//�������Ƶ�Ԫ
// ����GPIO����
// TB6612 #1 (ǰ��)
#define TB6612_1_AIN1_GPIO_PORT  GPIOB
#define TB6612_1_AIN1_PIN        GPIO_Pin_0
#define TB6612_1_AIN2_GPIO_PORT  GPIOB
#define TB6612_1_AIN2_PIN        GPIO_Pin_1
#define TB6612_1_PWMA_GPIO_PORT  GPIOA
#define TB6612_1_PWMA_PIN        GPIO_Pin_2

#define TB6612_1_BIN1_GPIO_PORT  GPIOB
#define TB6612_1_BIN1_PIN        GPIO_Pin_2
#define TB6612_1_BIN2_GPIO_PORT  GPIOB
#define TB6612_1_BIN2_PIN        GPIO_Pin_3
#define TB6612_1_PWMB_GPIO_PORT  GPIOA
#define TB6612_1_PWMB_PIN        GPIO_Pin_3

// TB6612 #2 (����)
#define TB6612_2_AIN1_GPIO_PORT  GPIOB
#define TB6612_2_AIN1_PIN        GPIO_Pin_4
#define TB6612_2_AIN2_GPIO_PORT  GPIOB
#define TB6612_2_AIN2_PIN        GPIO_Pin_5
#define TB6612_2_PWMA_GPIO_PORT  GPIOA
#define TB6612_2_PWMA_PIN        GPIO_Pin_4

#define TB6612_2_BIN1_GPIO_PORT  GPIOB
#define TB6612_2_BIN1_PIN        GPIO_Pin_6
#define TB6612_2_BIN2_GPIO_PORT  GPIOB
#define TB6612_2_BIN2_PIN        GPIO_Pin_7
#define TB6612_2_PWMB_GPIO_PORT  GPIOA
#define TB6612_2_PWMB_PIN        GPIO_Pin_5

// STBY����(����TB6612����)
#define TB6612_STBY_GPIO_PORT    GPIOA
#define TB6612_STBY_PIN          GPIO_Pin_6


void SetMotor(uint8_t motor, int16_t speed)
{
    // speed��Χ: -100��+100
    if(speed > 1000) speed = 1000;
    if(speed < -1000) speed = -1000;
    
    switch(motor) {
        case 0: // ��ǰ��
            if(speed > 0) {
                GPIO_SetBits(TB6612_1_AIN1_GPIO_PORT, TB6612_1_AIN1_PIN);
                GPIO_ResetBits(TB6612_1_AIN2_GPIO_PORT, TB6612_1_AIN2_PIN);
                // ����PWMռ�ձ�
                TIM_SetCompare1(TIM1, speed);
            } else if(speed < 0) {
                GPIO_ResetBits(TB6612_1_AIN1_GPIO_PORT, TB6612_1_AIN1_PIN);
                GPIO_SetBits(TB6612_1_AIN2_GPIO_PORT, TB6612_1_AIN2_PIN);
                // ����PWMռ�ձ�
                TIM_SetCompare1(TIM1, -speed);
            } else {
                // �ƶ�
                GPIO_SetBits(TB6612_1_AIN1_GPIO_PORT, TB6612_1_AIN1_PIN);
                GPIO_SetBits(TB6612_1_AIN2_GPIO_PORT, TB6612_1_AIN2_PIN);
            }
            break;
            
        case 1: // ��ǰ��
            if(speed > 0) {
                GPIO_SetBits(TB6612_1_BIN1_GPIO_PORT, TB6612_1_BIN1_PIN);
                GPIO_ResetBits(TB6612_1_BIN2_GPIO_PORT, TB6612_1_BIN2_PIN);
                // ����PWMռ�ձ�
                TIM_SetCompare2(TIM1, speed);
            } else if(speed < 0) {
                GPIO_ResetBits(TB6612_1_BIN1_GPIO_PORT, TB6612_1_BIN1_PIN);
                GPIO_SetBits(TB6612_1_BIN2_GPIO_PORT, TB6612_1_BIN2_PIN);
                // ����PWMռ�ձ�
                TIM_SetCompare2(TIM1, -speed);
            } else {
                // �ƶ�
                GPIO_SetBits(TB6612_1_BIN1_GPIO_PORT, TB6612_1_BIN1_PIN);
                GPIO_SetBits(TB6612_1_BIN2_GPIO_PORT, TB6612_1_BIN2_PIN);
            }
            break;
            
        case 2: // �����
            if(speed > 0) {
                GPIO_SetBits(TB6612_2_AIN1_GPIO_PORT, TB6612_2_AIN1_PIN);
                GPIO_ResetBits(TB6612_2_AIN2_GPIO_PORT, TB6612_2_AIN2_PIN);
                // ����PWMռ�ձ�
                TIM_SetCompare3(TIM1, speed);
            } else if(speed < 0) {
                GPIO_ResetBits(TB6612_2_AIN1_GPIO_PORT, TB6612_2_AIN1_PIN);
                GPIO_SetBits(TB6612_2_AIN2_GPIO_PORT, TB6612_2_AIN2_PIN);
                // ����PWMռ�ձ�
               TIM_SetCompare3(TIM1, -speed);
            } else {
                // �ƶ�
                GPIO_SetBits(TB6612_2_AIN1_GPIO_PORT, TB6612_2_AIN1_PIN);
                GPIO_SetBits(TB6612_2_AIN2_GPIO_PORT, TB6612_2_AIN2_PIN);
            }
            break;
            
        case 3: // �Һ���
            if(speed > 0) {
                GPIO_SetBits(TB6612_2_BIN1_GPIO_PORT, TB6612_2_BIN1_PIN);
                GPIO_ResetBits(TB6612_2_BIN2_GPIO_PORT, TB6612_2_BIN2_PIN);
                // ����PWMռ�ձ�
                TIM_SetCompare4(TIM1, speed);
            } else if(speed < 0) {
                GPIO_ResetBits(TB6612_2_BIN1_GPIO_PORT, TB6612_2_BIN1_PIN);
                GPIO_SetBits(TB6612_2_BIN2_GPIO_PORT, TB6612_2_BIN2_PIN);
                // ����PWMռ�ձ�
                TIM_SetCompare4(TIM1, -speed);
            } else {
                // �ƶ�
                GPIO_SetBits(TB6612_2_BIN1_GPIO_PORT, TB6612_2_BIN1_PIN);
                GPIO_SetBits(TB6612_2_BIN2_GPIO_PORT, TB6612_2_BIN2_PIN);
            }
            break;
    }
}

// ������������
void SetAllMotors(int8_t lf_speed, int8_t rf_speed, int8_t lb_speed, int8_t rb_speed)
{
    SetMotor(0, lf_speed); // ��ǰ
    SetMotor(1, rf_speed); // ��ǰ
    SetMotor(2, lb_speed); // ���
    SetMotor(3, rb_speed); // �Һ�
}

// �ƶ���������
void BrakeAll(void)
{
    SetAllMotors(0, 0, 0, 0);
}

// ����/����������
void EnableDriver(FunctionalState state)
{
    if(state == ENABLE) {
        GPIO_SetBits(TB6612_STBY_GPIO_PORT, TB6612_STBY_PIN);
    } else {
        GPIO_ResetBits(TB6612_STBY_GPIO_PORT, TB6612_STBY_PIN);
    }
}

