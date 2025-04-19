#include "Driver_USART.h"

/**
 * @description: ��ʼ������1
 */
void Driver_USART1_Init(void)
{
    /* 1. ����ʱ�� */
    /* 1.1 ����1�����ʱ�� */
    RCC->APB2PCENR |= RCC_USART1EN;
    /* 1.2 GPIOʱ�� */
    RCC->APB2PCENR |= RCC_IOPAEN;

    /* 2. ����GPIO���ŵĹ���ģʽ  PA9=Tx(�������� CNF=10 MODE=11)  PA10=Rx(�������� CNF=01 MODE=00)*/
    GPIOA->CFGHR |= GPIO_CFGHR_CNF9_1;
    GPIOA->CFGHR &= ~GPIO_CFGHR_CNF9_0;
    GPIOA->CFGHR |= GPIO_CFGHR_MODE9;

    GPIOA->CFGHR &= ~GPIO_CFGHR_CNF10_1;
    GPIOA->CFGHR |= GPIO_CFGHR_CNF10_0;
    GPIOA->CFGHR &= ~GPIO_CFGHR_MODE10;

    /* 3. ���ڵĲ������� */
    /* 3.1 ���ò����� 115200 */
    USART1->BRR = 0x271;
    /* 3.2 ����һ���ֵĳ��� 8λ */
    USART1->CTLR1 &= ~USART_CTLR1_M;
    /* 3.3 ���ò���ҪУ��λ */
    USART1->CTLR1 &= ~USART_CTLR1_PCE;
    /* 3.4 ����ֹͣλ�ĳ��� */
    USART1->CTLR2 &= ~USART_CTLR2_STOP;
    /* 3.5 ʹ�ܽ��պͷ��� */
    USART1->CTLR1 |= USART_CTLR1_TE;
    USART1->CTLR1 |= USART_CTLR1_RE;

    /* 3.6 ʹ�ܴ��ڵĸ����ж� */
    USART1->CTLR1 |= USART_CTLR1_RXNEIE; /* ���շǿ��ж� */
    USART1->CTLR1 |= USART_CTLR1_IDLEIE; /* �����ж� */

    /* 4. ����NVIC */
    NVIC_InitTypeDef        nvic_usart1;
    nvic_usart1.NVIC_IRQChannel                    = USART1_IRQn;    // TIM3 �ж�
    nvic_usart1.NVIC_IRQChannelPreemptionPriority  =  3;           // ���ȼ����úܸ�
    nvic_usart1.NVIC_IRQChannelSubPriority         =  2;           // ���ȼ����úܸ�
    nvic_usart1.NVIC_IRQChannelCmd                 = ENABLE;       // IRQ ͨ��ʹ��

    NVIC_Init(&nvic_usart1);
    /* 4. ʹ�ܴ��� */
    USART1->CTLR1 |= USART_CTLR1_UE;
}

/**
 * @description: ����һ���ֽ�
 * @param {uint8_t} byte Ҫ���͵��ֽ�
 */
void Driver_USART1_SendChar(uint8_t byte)
{
    /* 1. �ȴ����ͼĴ���Ϊ�� */
    while ((USART1->STATR & USART_STATR_TXE) == 0)
        ;

    /* 2. ����д�������ݼĴ��� */
    USART1->DATAR = byte;
}

/**
 * @description: ����һ���ַ���
 * @param {uint8_t} *str Ҫ���͵��ַ���
 * @param {uint16_t} len �ַ������ֽڵĳ���
 * @return {*}
 */
void Driver_USART1_SendString(uint8_t *str, uint16_t len)
{
    for (uint16_t i = 0; i < len; i++)
    {
        Driver_USART1_SendChar(str[i]);
    }
}

/**
 * @description: ����һ���ֽڵ�����
 * @return {*} ���յ����ֽ�
 */
uint8_t Driver_USART1_ReceiveChar(void)
{
    /* �ȴ����ݼĴ����ǿ� */
    while ((USART1->STATR & USART_STATR_RXNE) == 0)
        ;
    return USART1->DATAR;
}

/**
 * @description: ���ձ䳤����.���յ������ݴ��뵽buff��
 * @param {uint8_t} buff ��Ž��յ�������
 * @param {uint8_t} *len ����յ������ݵ��ֽڵĳ���
 */
void Driver_USART1_ReceiveString(uint8_t buff[], uint8_t *len)
{
    uint8_t i = 0;
    while (1)
    {
        // �ȴ����շǿ�
        while ((USART1->STATR & USART_STATR_RXNE) == 0)
        {
            // �ڵȴ��ڼ�, �ж��Ƿ��յ�����֡
            if (USART1->STATR & USART_STATR_IDLE)
            {
                
                *len = i;
                return;
            }
        }
        buff[i] = USART1->DATAR;
        i++;
    }
}

/* ������յ������� */
uint8_t buff[100] = {0};
/* �洢���յ����ֽڵĳ��� */
uint8_t len = 0;
uint8_t isToSend = 0;
void USART1_IRQHandler(void)
{
    /* ���ݽ��ռĴ����ǿ� */
    if (USART1->STATR & USART_STATR_RXNE)
    {
        // ��USART_DR�Ķ��������Խ����շǿյ��ж�λ���㡣 ���Բ��õ��������.
        //USART1->SR &= ~USART_SR_RXNE;
        buff[len] = USART1->DATAR;
        len++;
    }
    else if (USART1->STATR & USART_STATR_IDLE)
    {
        /* ��������жϱ�־λ: �ȶ�sr,�ٶ�dr.�Ϳ���ʵ������� */
        USART1->STATR;
        USART1->DATAR;
        /* �䳤���ݽ������ */
        //Driver_USART1_SendString(buff, len);
        isToSend = 1;
        /* �ѽ����ֽڵĳ�����0 */
        // len = 0;
    }
}

// ������printf��ʱ��,���Զ��������������ִ��,ֻ��Ҫ����һ��ͨ�����ڷ����ַ��ĺ���
int fputc(int c, FILE *file)
{
    Driver_USART1_SendChar(c);
    return c;
}
