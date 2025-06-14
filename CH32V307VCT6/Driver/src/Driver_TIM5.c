#include "Driver_TIM5.h"

void Driver_TIM5_Init(void)
{
    /* 1.  开启时钟*/
    /* 1.1 定时器5的时钟 */
    RCC->APB1PCENR |= RCC_TIM5EN;
    /* 1.2 GPIO的时钟 PA */
    RCC->APB2PCENR |= RCC_IOPAEN;

    /* 2. 设置GPIO的复用推挽输出 PA1 CNF=10 MODE=11*/
    GPIOA->CFGLR |= (GPIO_CFGLR_CNF1_1 | GPIO_CFGLR_MODE1);
    GPIOA->CFGLR &= ~GPIO_CFGLR_CNF1_0;

    /* 3. 定时器配置 */
    /* 3.1 预分频器的配置 */
    TIM5->PSC = 7200 - 1;
    /* 3.2 自动重装载寄存器的配置 */
    TIM5->ATRLR = 100 - 1;
    /* 3.3 计数器的计数方向 0=向上 1=向下*/
    TIM5->CTLR1 &= ~TIM_DIR;
    /* 3.4 配置通道2的捕获比较寄存器 */
    TIM5->CH2CVR = 97;
    /* 3.5 把通道2配置为输出  CCMR1_CC2S=00 */
    TIM5->CHCTLR1 &= ~TIM_CC2S;
    /* 3.6 配置通道的输出比较模式 CCMR1_OC2M=110*/
    TIM5->CHCTLR1 |= TIM_OC2M_2;
    TIM5->CHCTLR1 |= TIM_OC2M_1;
    TIM5->CHCTLR1 &= ~TIM_OC2M_0;
    /* 3.7 使能通道2  CCER_CC2E=1 */
    TIM5->CCER |= TIM_CC2E;

    /* 3.8 设置通道的极性 0=高电平有效  1=低电平有效 */
    TIM5->CCER &= ~TIM_CC2P;
}

void Driver_TIM5_Start(void)
{
    /* 使能计数器 */
    TIM5->CTLR1 |= TIM_CEN;
}

void Driver_TIM5_Stop(void)
{
    /* 失能计数器 */
    TIM5->CTLR1 &= ~TIM_CEN;
}

void Driver_TIM5_SetDutyCycle(uint8_t dutyCycle)
{
    TIM5->CH2CVR = dutyCycle;
}