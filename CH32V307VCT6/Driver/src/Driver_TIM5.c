#include "Driver_TIM5.h"

void Driver_TIM5_Init(void)
{
    /* 1.  ����ʱ��*/
    /* 1.1 ��ʱ��5��ʱ�� */
    RCC->APB1PCENR |= RCC_TIM5EN;
    /* 1.2 GPIO��ʱ�� PA */
    RCC->APB2PCENR |= RCC_IOPAEN;

    /* 2. ����GPIO�ĸ���������� PA1 CNF=10 MODE=11*/
    GPIOA->CFGLR |= (GPIO_CFGLR_CNF1_1 | GPIO_CFGLR_MODE1);
    GPIOA->CFGLR &= ~GPIO_CFGLR_CNF1_0;

    /* 3. ��ʱ������ */
    /* 3.1 Ԥ��Ƶ�������� */
    TIM5->PSC = 7200 - 1;
    /* 3.2 �Զ���װ�ؼĴ��������� */
    TIM5->ATRLR = 100 - 1;
    /* 3.3 �������ļ������� 0=���� 1=����*/
    TIM5->CTLR1 &= ~TIM_DIR;
    /* 3.4 ����ͨ��2�Ĳ���ȽϼĴ��� */
    TIM5->CH2CVR = 97;
    /* 3.5 ��ͨ��2����Ϊ���  CCMR1_CC2S=00 */
    TIM5->CHCTLR1 &= ~TIM_CC2S;
    /* 3.6 ����ͨ��������Ƚ�ģʽ CCMR1_OC2M=110*/
    TIM5->CHCTLR1 |= TIM_OC2M_2;
    TIM5->CHCTLR1 |= TIM_OC2M_1;
    TIM5->CHCTLR1 &= ~TIM_OC2M_0;
    /* 3.7 ʹ��ͨ��2  CCER_CC2E=1 */
    TIM5->CCER |= TIM_CC2E;

    /* 3.8 ����ͨ���ļ��� 0=�ߵ�ƽ��Ч  1=�͵�ƽ��Ч */
    TIM5->CCER &= ~TIM_CC2P;
}

void Driver_TIM5_Start(void)
{
    /* ʹ�ܼ����� */
    TIM5->CTLR1 |= TIM_CEN;
}

void Driver_TIM5_Stop(void)
{
    /* ʧ�ܼ����� */
    TIM5->CTLR1 &= ~TIM_CEN;
}

void Driver_TIM5_SetDutyCycle(uint8_t dutyCycle)
{
    TIM5->CH2CVR = dutyCycle;
}