#include "Driver_USART.h"

/**
 * @description: 初始化串口1
 */
void Driver_USART1_Init(void)
{
    /* 1. 开启时钟 */
    /* 1.1 串口1外设的时钟 */
    RCC->APB2PCENR |= RCC_USART1EN;
    /* 1.2 GPIO时钟 */
    RCC->APB2PCENR |= RCC_IOPAEN;

    /* 2. 配置GPIO引脚的工作模式  PA9=Tx(复用推挽 CNF=10 MODE=11)  PA10=Rx(浮空输入 CNF=01 MODE=00)*/
    GPIOA->CFGHR |= GPIO_CFGHR_CNF9_1;
    GPIOA->CFGHR &= ~GPIO_CFGHR_CNF9_0;
    GPIOA->CFGHR |= GPIO_CFGHR_MODE9;

    GPIOA->CFGHR &= ~GPIO_CFGHR_CNF10_1;
    GPIOA->CFGHR |= GPIO_CFGHR_CNF10_0;
    GPIOA->CFGHR &= ~GPIO_CFGHR_MODE10;

    /* 3. 串口的参数配置 */
    /* 3.1 配置波特率 115200 */
    USART1->BRR = 0x271;
    /* 3.2 配置一个字的长度 8位 */
    USART1->CTLR1 &= ~USART_CTLR1_M;
    /* 3.3 配置不需要校验位 */
    USART1->CTLR1 &= ~USART_CTLR1_PCE;
    /* 3.4 配置停止位的长度 */
    USART1->CTLR2 &= ~USART_CTLR2_STOP;
    /* 3.5 使能接收和发送 */
    USART1->CTLR1 |= USART_CTLR1_TE;
    USART1->CTLR1 |= USART_CTLR1_RE;

    /* 3.6 使能串口的各种中断 */
    USART1->CTLR1 |= USART_CTLR1_RXNEIE; /* 接收非空中断 */
    USART1->CTLR1 |= USART_CTLR1_IDLEIE; /* 空闲中断 */

    /* 4. 配置NVIC */
    NVIC_InitTypeDef        nvic_usart1;
    nvic_usart1.NVIC_IRQChannel                    = USART1_IRQn;    // TIM3 中断
    nvic_usart1.NVIC_IRQChannelPreemptionPriority  =  3;           // 优先级不用很高
    nvic_usart1.NVIC_IRQChannelSubPriority         =  2;           // 优先级不用很高
    nvic_usart1.NVIC_IRQChannelCmd                 = ENABLE;       // IRQ 通道使能

    NVIC_Init(&nvic_usart1);
    /* 4. 使能串口 */
    USART1->CTLR1 |= USART_CTLR1_UE;
}

/**
 * @description: 发送一个字节
 * @param {uint8_t} byte 要发送的字节
 */
void Driver_USART1_SendChar(uint8_t byte)
{
    /* 1. 等待发送寄存器为空 */
    while ((USART1->STATR & USART_STATR_TXE) == 0)
        ;

    /* 2. 数据写出到数据寄存器 */
    USART1->DATAR = byte;
}

/**
 * @description: 发送一个字符串
 * @param {uint8_t} *str 要发送的字符串
 * @param {uint16_t} len 字符串中字节的长度
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
 * @description: 接收一个字节的数据
 * @return {*} 接收到的字节
 */
uint8_t Driver_USART1_ReceiveChar(void)
{
    /* 等待数据寄存器非空 */
    while ((USART1->STATR & USART_STATR_RXNE) == 0)
        ;
    return USART1->DATAR;
}

/**
 * @description: 接收变长数据.接收到的数据存入到buff中
 * @param {uint8_t} buff 存放接收到的数据
 * @param {uint8_t} *len 存放收到的数据的字节的长度
 */
void Driver_USART1_ReceiveString(uint8_t buff[], uint8_t *len)
{
    uint8_t i = 0;
    while (1)
    {
        // 等待接收非空
        while ((USART1->STATR & USART_STATR_RXNE) == 0)
        {
            // 在等待期间, 判断是否收到空闲帧
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

/* 缓冲接收到的数据 */
uint8_t buff[100] = {0};
/* 存储接收到的字节的长度 */
uint8_t len = 0;
uint8_t isToSend = 0;
void USART1_IRQHandler(void)
{
    /* 数据接收寄存器非空 */
    if (USART1->STATR & USART_STATR_RXNE)
    {
        // 对USART_DR的读操作可以将接收非空的中断位清零。 所以不用单独清除了.
        //USART1->SR &= ~USART_SR_RXNE;
        buff[len] = USART1->DATAR;
        len++;
    }
    else if (USART1->STATR & USART_STATR_IDLE)
    {
        /* 清除空闲中断标志位: 先读sr,再读dr.就可以实现清除了 */
        USART1->STATR;
        USART1->DATAR;
        /* 变长数据接收完毕 */
        //Driver_USART1_SendString(buff, len);
        isToSend = 1;
        /* 把接收字节的长度清0 */
        // len = 0;
    }
}

// 当调用printf的时候,会自动调用这个方法来执行,只需要调用一个通过串口发送字符的函数
int fputc(int c, FILE *file)
{
    Driver_USART1_SendChar(c);
    return c;
}
