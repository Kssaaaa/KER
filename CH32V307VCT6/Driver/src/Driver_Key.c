#include "Driver_Key.h"
#include "Driver_LED.h"



/**
 * @description: ��ʼ������.
 *  1. ��������Ӧ��io�����ù���ģʽ: ��������
 *  2. ���ø���Ϊ�ⲿ�ж�
 *  3. �����ⲿ�жϿ����� EXTI
 *  4. ����NVIC
 */
void Driver_Key_Init(void)
{
    /* 1. ����ʱ�� */
    /* 1.1  GPIOE*/
    RCC->APB2PCENR |= RCC_IOPERST;
    /* 1.2  AFIO*/
    RCC->APB2PCENR |= RCC_AFIOEN;

    /* 2. ���� PE10Ϊ��������: MODE=00 CNF=10  ODR=0 */
    GPIOE->CFGHR &= ~GPIO_CFGHR_MODE10;
    GPIOE->CFGHR |= GPIO_CFGHR_CNF10_1;
    GPIOE->CFGHR &= ~GPIO_CFGHR_CNF10_0;
    GPIOE->OUTDR &= ~GPIO_OUTDR_ODR10;

    /* 3. ����AFIO ����PE10����Ϊ�ⲿ�ж� EXTICR3 ?0101 */
     AFIO->EXTICR[2] &= ~AFIO_EXTICR3_EXTI10;
     AFIO->EXTICR[2] |= AFIO_EXTICR3_EXTI10_PE;

    /* 4. ����EXTI */
    /* 4.1. ���������ش��� RTSR TR10=1*/
    EXTI->RTENR |= EXTI_RTENR_TR10;
    /* 4.2 ���� LINE10, ���õ��ж����μĴ��� */
    EXTI->INTENR |= EXTI_INTENR_MR10;

    /* 5. ���� NVIC */
    NVIC_InitTypeDef        nvic;
    nvic.NVIC_IRQChannel                    = EXTI2_IRQn;    // TIM3 �ж�
    nvic.NVIC_IRQChannelPreemptionPriority  =  5;           // ���ȼ����úܸ�
    nvic.NVIC_IRQChannelSubPriority         =  5;           // ���ȼ����úܸ�
    nvic.NVIC_IRQChannelCmd                 = ENABLE;       // IRQ ͨ��ʹ��

    NVIC_Init(&nvic);
}

/**
 * @description: line 15-10���жϷ�����.
 *  һ�������°���1,���ִ��һ���������
 * @return {*}
 */
void EXTI15_10_IRQHandler(void)
{
    /* ���һ������Ҫ����жϱ�־λ */
    EXTI->INTFR |= EXTI_INTENR_MR10;

   // Delay_ms(5);
    if ((GPIOE->INDR & EXTI_INTENR_MR10) != 0)
    {
        Drviver_LED_Toggle(LED_1);
    }
}

