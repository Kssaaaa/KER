#include "Driver_TIM4.h"
#include "stdio.h"

void Driver_TIM4_Init(void)
{
    /* 1. 开启时钟 */
    /* 1.1 TIM4的时钟 */
    /* 1.2 把引脚(PB6) 对应的端口开启时钟*/
    RCC->APB1PCENR |= RCC_TIM4EN;
    RCC->APB2PCENR |= RCC_IOPBEN;

    /* 2. PB6设置为输入: 浮空输入 CNF=01 MODE=00 */
    GPIOB->CFGLR &= ~(GPIO_CFGLR_CNF6_1 | GPIO_CFGLR_MODE6);
    GPIOB->CFGLR |= GPIO_CFGLR_CNF6_0;

    /* 3. 配置定时器的时基部分 */
    /* 3.1 预分频系数配置 分频后计数器的时钟时1MHz 周期:1us*/
    TIM4->PSC = 72 - 1;
    /* 3.2 自动重装载寄存器的值: 设置最大,尽量避免溢出 */
    TIM4->ATRLR = 65535;
    /* 3.3 计数方向: 向上计数 */
    TIM4->CTLR1 &= ~TIM_DIR;

    /* 4. 输入捕获部分 */
    /* 4.1 输入信号不滤波 */
    TIM4->CHCTLR1 &= ~TIM_IC1F;
    /* 4.2 配置边缘检测: 去IC1上升沿 去IC2的下降沿 */
    TIM4->CCER &= ~TIM_CC1P;
    TIM4->CCER |= TIM_CC2P;
    /* 4.3 把 IC1映射到TI1 CCMR1_CC1S=01 上升沿*/
    TIM4->CHCTLR1 &= ~TIM_CC1S_1;
    TIM4->CHCTLR1 |= TIM_CC1S_0;
    /* 4.4 把 IC2映射到TI1 CCMR1_CC2S=10 下降沿*/
    TIM4->CHCTLR1 |= TIM_CC2S_1;
    TIM4->CHCTLR1 &= ~TIM_CC2S_0;
    /* 4.5 IC1和IC2的预分频 都不分频 */
    TIM4->CHCTLR1 &= ~TIM_IC1PSC;
    TIM4->CHCTLR1 &= ~TIM_IC2PSC;

    /* 4.6 配置TRGI信号:  TI1FP1   SMCR_TS=101*/
    TIM4->SMCFGR |= (TIM_TS_2 | TIM_TS_0);
    TIM4->SMCFGR &= ~TIM_TS_1;

    /* 4.7 配置从模式: 为复位模式 SMCR_SMS=100*/
    TIM4->SMCFGR |= TIM_SMS_2;
    TIM4->SMCFGR &= ~(TIM_SMS_1 | TIM_SMS_0);

    /* 4.8 使能通道1捕获和通道2捕获 */
    TIM4->CCER |= TIM_CC1E;
    TIM4->CCER |= TIM_CC2E;
}

void Driver_TIM4_Start(void)
{
    TIM4->CTLR1 |= TIM_CEN; /* 计数器使能 */
}

void Driver_TIM4_Stop(void)
{
    TIM4->CTLR1 &= ~TIM_CEN; /* 计数器失能 */
}


/* 返回PWM的周期 ms*/
double Driver_TIM4_GetPWMCycle(void)
{
    return TIM4->CH1CVR / 1000.0;
}

/* 返回PWM的频率 */
double Driver_TIM4_GetPWMFreq(void)
{
    return 1000000.0 / TIM4->CH1CVR;
}

// 返回占空比
double Driver_TIM4_GetDutyCycle(void)
{
    return TIM4->CH2CVR * 1.0 / TIM4->CH1CVR;
}