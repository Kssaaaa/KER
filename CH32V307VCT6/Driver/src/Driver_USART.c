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
 * @description: ��ʼ������2
 */
void Driver_USART2_Init(void)
{
    /* 1. ����ʱ�� */
    /* 1.1 ����1�����ʱ�� */
    RCC->APB2PCENR |= RCC_USART2EN;
    /* 1.2 GPIOʱ�� */
    RCC->APB2PCENR |= RCC_IOPAEN;

    /* 2. ����GPIO���ŵĹ���ģʽ  PA9=Tx(�������� CNF=10 MODE=11)  PA10=Rx(�������� CNF=01 MODE=00)*/
    /* 2. ����GPIO���ŵĹ���ģʽ  PA2=Tx(�������� CNF=10 MODE=11)  PA3=Rx(�������� CNF=01 MODE=00)*/
    GPIOA->CFGLR |= GPIO_CFGLR_CNF2_1;
    GPIOA->CFGLR &= ~GPIO_CFGLR_CNF2_0;
    GPIOA->CFGLR |= GPIO_CFGLR_MODE2;

    GPIOA->CFGLR &= ~GPIO_CFGLR_CNF3_1;
    GPIOA->CFGLR |= GPIO_CFGLR_CNF3_0;
    GPIOA->CFGLR &= ~GPIO_CFGLR_MODE3;

    /* 3. ���ڵĲ������� */
    /* 3.1 ���ò����� 115200 */
    USART2->BRR = 0x271;
    /* 3.2 ����һ���ֵĳ��� 8λ */
    USART2->CTLR1 &= ~USART_CTLR1_M;
    /* 3.3 ���ò���ҪУ��λ */
    USART2->CTLR1 &= ~USART_CTLR1_PCE;
    /* 3.4 ����ֹͣλ�ĳ��� */
    USART2->CTLR2 &= ~USART_CTLR2_STOP;
    /* 3.5 ʹ�ܽ��պͷ��� */
    USART2->CTLR1 |= USART_CTLR1_TE;
    USART2->CTLR1 |= USART_CTLR1_RE;

    /* 3.6 ʹ�ܴ��ڵĸ����ж� */
    USART2->CTLR1 |= USART_CTLR1_RXNEIE; /* ���շǿ��ж� */
    USART2->CTLR1 |= USART_CTLR1_IDLEIE; /* �����ж� */

    /* 4. ����NVIC */
    NVIC_InitTypeDef        nvic_usart2;
    nvic_usart2.NVIC_IRQChannel                    = USART1_IRQn;    // TIM3 �ж�
    nvic_usart2.NVIC_IRQChannelPreemptionPriority  =  3;           // ���ȼ����úܸ�
    nvic_usart2.NVIC_IRQChannelSubPriority         =  2;           // ���ȼ����úܸ�
    nvic_usart2.NVIC_IRQChannelCmd                 = ENABLE;       // IRQ ͨ��ʹ��

    NVIC_Init(&nvic_usart2);
    /* 4. ʹ�ܴ��� */
    USART2->CTLR1 |= USART_CTLR1_UE;
}
//ʹ�ùٷ���
void USART2_UART_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};
    USART_InitTypeDef USART_InitStructure = {0};

    /* 1. ʹ��ʱ�� */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  // USART2_TX(PA2), USART2_RX(PA3) �� GPIOA
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE); // USART2 �� APB1 �豸

    /* 2. ���� USART2 �� TX(PA2) �� RX(PA3) */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;          // TX (PA2)
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;    // �����������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  // ����
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;          // RX (PA3)
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; // ��������
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* 3. ���� USART2 ���� */
    USART_InitStructure.USART_BaudRate = 115200;       // ������ 115200
    USART_InitStructure.USART_WordLength = USART_WordLength_8b; // 8λ����
    USART_InitStructure.USART_StopBits = USART_StopBits_1;      // 1λֹͣλ
    USART_InitStructure.USART_Parity = USART_Parity_No;         // ��У��
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; // ��Ӳ������
    USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx; // ����+����ģʽ

    USART_Init(USART2, &USART_InitStructure);          // ��ʼ�� USART2

    /* 4. ʹ�� USART2 */
    USART_Cmd(USART2, ENABLE);
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

/**
 * @brief �� HAL_UART_Transmit������ʽ���ͣ�
 * @param USARTx: �������裨USART1, USART2...��
 * @param pData: ���������ݻ�����
 * @param Size: ���ݳ���
 * @param Timeout: ��ʱʱ�䣨ms����HAL_MAX_DELAY��0xFFFFFFFF����ʾ���޵ȴ�
 * @retval ��
 */
void UART_Transmit(USART_TypeDef* USARTx, uint8_t *pData, uint16_t Size, uint32_t Timeout)
{
    uint32_t startTick = GetTick();  // ��ȡ��ǰϵͳʱ�䣨������ʵ�� GetTick()��
    
    while (Size--)
    {
        USART_SendData(USARTx, *pData++);  // ����һ���ֽ�
        
        // �ȴ�������ɣ���ʱ��
        while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET)
        {
            if (Timeout != HAL_MAX_DELAY)
            {
                if ((GetTick() - startTick) >= Timeout)
                {
                    return;  // ��ʱ�˳�
                }
            }
        }
    }
}