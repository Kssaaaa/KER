#include "Driver_TIM6.h"
#include "debug.h"
/**
 * @description: ����ʱ��6���г�ʼ��
 * @return {*}
 */
void Driver_TIM6_Init(void)
{
    /* 1. ����ʱ��6����ʱ��*/
    RCC->APB1PCENR |= RCC_TIM6EN;

    /* 2. ����Ԥ��Ƶֵ: ��Ƶֵ7199��ʾ7200��Ƶ����Ƶ��Ƶ��10K������100us */
    TIM6->PSC = 7200 - 1;

    /*
    3. �����Զ���ת�ؼĴ�����ֵ�������жϷ�����Ƶ�ʡ�
        ��������Ϊ9999����ʾ����������10000�η���һ���жϡ�
        ����һ��100us��10000��1000000us������1s
    */
    TIM6->ATRLR = 10000 - 1;

    /* Ϊ�˱���һ����������������ж�,�����Ȳ���һ�������¼� */
    TIM6->CTLR1 |= TIM_CTLR1_CKD;
    TIM6->SWEVGR |= TIM_UG; /* Ԥ��Ƶ�Ĵ�������װ�ؼĴ�����ֵ���µ�Ӱ�ӼĴ��� */
    // TIM6->SR &= ~TIM_SR_UIF;
    
    /* 4. ʹ�ܸ����ж� */
    TIM6->DMAINTENR |= TIM_UIE;

    /* NVIC���� */
    NVIC_InitTypeDef        NVIC_TIM6;
    NVIC_TIM6.NVIC_IRQChannel                    = TIM6_IRQn;    // TIM3 �ж�
    NVIC_TIM6.NVIC_IRQChannelPreemptionPriority  =  3;           // ���ȼ����úܸ�
    NVIC_TIM6.NVIC_IRQChannelSubPriority         =  3;           // ���ȼ����úܸ�
    NVIC_TIM6.NVIC_IRQChannelCmd                 = ENABLE;       // IRQ ͨ��ʹ��

    NVIC_Init(&NVIC_TIM6);

    /* 8. ʹ�ܼ����� */
    TIM6->CTLR1 |= TIM_CEN;
}

/**
 * @description: TIM6���жϷ�����
 * @return {*}
 */
void TIM6_IRQHandler(void)
{
    TIM6->INTFR &= ~TIM_UIF;
}
