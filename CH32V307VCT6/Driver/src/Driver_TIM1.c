#include "Driver_TIM1.h"
#include "stdio.h"
#include "Driver_USART.h"

void Driver_TIM1_Init(void)
{
    /* 1. ����ʱ�� */
    /* 1.1 TIM1��ʱ�� */
    RCC->APB2PCENR |= RCC_TIM1EN;
    /* 1.2 GPIO��ʱ�� PA */
    RCC->APB2PCENR |= RCC_IOPAEN;

    /* 2. ��PA8���ø���������� CNF=10  MODE=11*/
    GPIOA->CFGHR |= (GPIO_CFGHR_CNF8_1 | GPIO_CFGHR_MODE8);
    GPIOA->CFGHR &= ~GPIO_CFGHR_CNF8_0;

    /* 3. ʱ����Ԫ������  Ƶ����2Hz��PWM*/
    /* 3.1 Ԥ��Ƶ */
    TIM1->PSC = 7200 - 1;
    /* 3.2 �Զ���װ�ؼĴ�����ֵ */
    TIM1->ATRLR = 5000 - 1;
    /* 3.3 �������� 0=���ϼ��� 1=���¼���*/
    TIM1->CTLR1 &= ~TIM_DIR;
    /* 3.4 �ظ��Ĵ��� RCR=4 ��ʾCNT���������5��,�Ż��������ʱ��(�ж�)*/
    TIM1->RPTCR = 4;

    /* 4. ����������� */
    /* 4.1 ��ͨ��1����Ϊ���*/
    TIM1->CHCTLR1 &= ~TIM_CC1S;
    /* 4.2 ���ò���ȽϼĴ�����ֵ */;
    TIM1->CH1CVR = 2500; /* ռ�ձ� 50% */
    /* 4.3 ����ļ��� 0=�ߵ�ƽ��Ч 1=�͵�ƽ��Ч */;
    TIM1->CCER &= TIM_CC1P;
    /* 4.4 ʹ��ͨ��1 */;
    TIM1->CCER |= TIM_CC1E;
    /* 4.5 ����ͨ��1���ģʽ: PWM1=110   */
    TIM1->CHCTLR1 |= (TIM_OC1M_2 | TIM_OC1M_1);
    TIM1->CHCTLR1 &= ~TIM_OC1M_0;
    /* 4.6 �����ʹ��(ֻ�и߼���ʱ����Ҫ����)*/
    TIM1->BDTR |= TIM_MOE;
    
    /* 4.7 ��ֹһ�����ͽ�������ж�: 1. ʹ��UGλ����һ�������¼� 2.Ȼ��Ԥ��Ƶ���ظ���������ֵ���µ�Ӱ�ӼĴ��� */;
    TIM1->CHCTLR1 |= TIM_UG;
    TIM1->INTFR &= ~TIM_UIF;
    
    /* 5. �����ж� */
    /* 5.1 ��ʱ�������ж�ʹ�� */
    TIM1->DMAINTENR |= TIM_UIE;
    /* 5.2 NVIC������ */
    
        /* NVIC���� */
    NVIC_InitTypeDef        NVIC_TIM1_UP;
    NVIC_TIM1_UP.NVIC_IRQChannel                    = TIM1_UP_IRQn;    // TIM3 �ж�
    NVIC_TIM1_UP.NVIC_IRQChannelPreemptionPriority  =  2;           // ���ȼ����úܸ�
    NVIC_TIM1_UP.NVIC_IRQChannelSubPriority         =  1;           // ���ȼ����úܸ�
    NVIC_TIM1_UP.NVIC_IRQChannelCmd                 = ENABLE;       // IRQ ͨ��ʹ��

    NVIC_Init(&NVIC_TIM1_UP);

    /* 8. ʹ�ܼ����� */
    NVIC_EnableIRQ(TIM1_UP_IRQn);
    
}

void Driver_TIM1_Start(void)
{
    TIM1->CTLR1 |= TIM_CEN;
        
             
}

void Driver_TIM1_Stop(void)
{
    TIM1->CTLR1 &= ~TIM_CEN;
}

void TIM1_UP_IRQHandler(void)
{
    // ͣ��������
    TIM1->INTFR &= ~TIM_UIF;
    Driver_TIM1_Stop();
    
    
}


    
