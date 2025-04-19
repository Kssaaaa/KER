#include "Driver_TIM1.h"
#include "stdio.h"
#include "Driver_USART.h"

void Driver_TIM1_Init(void)
{
    /* 1. 开启时钟 */
    /* 1.1 TIM1的时钟 */
    RCC->APB2PCENR |= RCC_TIM1EN;
    /* 1.2 GPIO的时钟 PA */
    RCC->APB2PCENR |= RCC_IOPAEN;

    /* 2. 给PA8配置复用推挽输出 CNF=10  MODE=11*/
    GPIOA->CFGHR |= (GPIO_CFGHR_CNF8_1 | GPIO_CFGHR_MODE8);
    GPIOA->CFGHR &= ~GPIO_CFGHR_CNF8_0;

    /* 3. 时基单元的配置  频率是2Hz的PWM*/
    /* 3.1 预分频 */
    TIM1->PSC = 7200 - 1;
    /* 3.2 自动重装载寄存器的值 */
    TIM1->ATRLR = 5000 - 1;
    /* 3.3 计数方向 0=向上计数 1=向下计数*/
    TIM1->CTLR1 &= ~TIM_DIR;
    /* 3.4 重复寄存器 RCR=4 表示CNT计数器溢出5次,才会产生更新时间(中断)*/
    TIM1->RPTCR = 4;

    /* 4. 输出部分配置 */
    /* 4.1 把通道1配置为输出*/
    TIM1->CHCTLR1 &= ~TIM_CC1S;
    /* 4.2 配置捕获比较寄存器的值 */;
    TIM1->CH1CVR = 2500; /* 占空比 50% */
    /* 4.3 输出的极性 0=高电平有效 1=低电平有效 */;
    TIM1->CCER &= TIM_CC1P;
    /* 4.4 使能通道1 */;
    TIM1->CCER |= TIM_CC1E;
    /* 4.5 配置通道1输出模式: PWM1=110   */
    TIM1->CHCTLR1 |= (TIM_OC1M_2 | TIM_OC1M_1);
    TIM1->CHCTLR1 &= ~TIM_OC1M_0;
    /* 4.6 主输出使能(只有高级定时器需要配置)*/
    TIM1->BDTR |= TIM_MOE;
    
    /* 4.7 防止一启动就进入更新中断: 1. 使用UG位产生一个更新事件 2.然后预分频和重复计数器的值更新到影子寄存器 */;
    TIM1->CHCTLR1 |= TIM_UG;
    TIM1->INTFR &= ~TIM_UIF;
    
    /* 5. 配置中断 */
    /* 5.1 定时器更新中断使能 */
    TIM1->DMAINTENR |= TIM_UIE;
    /* 5.2 NVIC的配置 */
    
        /* NVIC配置 */
    NVIC_InitTypeDef        NVIC_TIM1_UP;
    NVIC_TIM1_UP.NVIC_IRQChannel                    = TIM1_UP_IRQn;    // TIM3 中断
    NVIC_TIM1_UP.NVIC_IRQChannelPreemptionPriority  =  2;           // 优先级不用很高
    NVIC_TIM1_UP.NVIC_IRQChannelSubPriority         =  1;           // 优先级不用很高
    NVIC_TIM1_UP.NVIC_IRQChannelCmd                 = ENABLE;       // IRQ 通道使能

    NVIC_Init(&NVIC_TIM1_UP);

    /* 8. 使能计数器 */
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
    // 停掉计数器
    TIM1->INTFR &= ~TIM_UIF;
    Driver_TIM1_Stop();
    
    
}


    
