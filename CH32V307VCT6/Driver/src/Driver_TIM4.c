#include "Driver_TIM4.h"
#include "stdio.h"

void Driver_TIM4_Init(void)
{
    /* 1. ����ʱ�� */
    /* 1.1 TIM4��ʱ�� */
    /* 1.2 ������(PB6) ��Ӧ�Ķ˿ڿ���ʱ��*/
    RCC->APB1PCENR |= RCC_TIM4EN;
    RCC->APB2PCENR |= RCC_IOPBEN;

    /* 2. PB6����Ϊ����: �������� CNF=01 MODE=00 */
    GPIOB->CFGLR &= ~(GPIO_CFGLR_CNF6_1 | GPIO_CFGLR_MODE6);
    GPIOB->CFGLR |= GPIO_CFGLR_CNF6_0;

    /* 3. ���ö�ʱ����ʱ������ */
    /* 3.1 Ԥ��Ƶϵ������ ��Ƶ���������ʱ��ʱ1MHz ����:1us*/
    TIM4->PSC = 72 - 1;
    /* 3.2 �Զ���װ�ؼĴ�����ֵ: �������,����������� */
    TIM4->ATRLR = 65535;
    /* 3.3 ��������: ���ϼ��� */
    TIM4->CTLR1 &= ~TIM_DIR;

    /* 4. ���벶�񲿷� */
    /* 4.1 �����źŲ��˲� */
    TIM4->CHCTLR1 &= ~TIM_IC1F;
    /* 4.2 ���ñ�Ե���: ȥIC1������ ȥIC2���½��� */
    TIM4->CCER &= ~TIM_CC1P;
    TIM4->CCER |= TIM_CC2P;
    /* 4.3 �� IC1ӳ�䵽TI1 CCMR1_CC1S=01 ������*/
    TIM4->CHCTLR1 &= ~TIM_CC1S_1;
    TIM4->CHCTLR1 |= TIM_CC1S_0;
    /* 4.4 �� IC2ӳ�䵽TI1 CCMR1_CC2S=10 �½���*/
    TIM4->CHCTLR1 |= TIM_CC2S_1;
    TIM4->CHCTLR1 &= ~TIM_CC2S_0;
    /* 4.5 IC1��IC2��Ԥ��Ƶ ������Ƶ */
    TIM4->CHCTLR1 &= ~TIM_IC1PSC;
    TIM4->CHCTLR1 &= ~TIM_IC2PSC;

    /* 4.6 ����TRGI�ź�:  TI1FP1   SMCR_TS=101*/
    TIM4->SMCFGR |= (TIM_TS_2 | TIM_TS_0);
    TIM4->SMCFGR &= ~TIM_TS_1;

    /* 4.7 ���ô�ģʽ: Ϊ��λģʽ SMCR_SMS=100*/
    TIM4->SMCFGR |= TIM_SMS_2;
    TIM4->SMCFGR &= ~(TIM_SMS_1 | TIM_SMS_0);

    /* 4.8 ʹ��ͨ��1�����ͨ��2���� */
    TIM4->CCER |= TIM_CC1E;
    TIM4->CCER |= TIM_CC2E;
}

void Driver_TIM4_Start(void)
{
    TIM4->CTLR1 |= TIM_CEN; /* ������ʹ�� */
}

void Driver_TIM4_Stop(void)
{
    TIM4->CTLR1 &= ~TIM_CEN; /* ������ʧ�� */
}


/* ����PWM������ ms*/
double Driver_TIM4_GetPWMCycle(void)
{
    return TIM4->CH1CVR / 1000.0;
}

/* ����PWM��Ƶ�� */
double Driver_TIM4_GetPWMFreq(void)
{
    return 1000000.0 / TIM4->CH1CVR;
}

// ����ռ�ձ�
double Driver_TIM4_GetDutyCycle(void)
{
    return TIM4->CH2CVR * 1.0 / TIM4->CH1CVR;
}