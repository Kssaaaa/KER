#include "Driver_TIM6.h"
#include "debug.h"
/**
 * @description: 给定时器6进行初始化
 * @return {*}
 */
void Driver_TIM6_Init(void)
{
    /* 1. 给定时器6开启时钟*/
    RCC->APB1PCENR |= RCC_TIM6EN;

    /* 2. 设置预分频值: 分频值7199表示7200分频。分频后频率10K，周期100us */
    TIM6->PSC = 7200 - 1;

    /*
    3. 设置自动重转载寄存器的值，决定中断发生的频率。
        假设设置为9999，表示计数器计数10000次发生一次中断。
        计数一次100us，10000次1000000us，正好1s
    */
    TIM6->ATRLR = 10000 - 1;

    /* 为了避免一启动立即进入更新中断,可以先产生一个更新事件 */
    TIM6->CTLR1 |= TIM_CTLR1_CKD;
    TIM6->SWEVGR |= TIM_UG; /* 预分频寄存器和重装载寄存器的值更新到影子寄存器 */
    // TIM6->SR &= ~TIM_SR_UIF;
    
    /* 4. 使能更新中断 */
    TIM6->DMAINTENR |= TIM_UIE;

    /* NVIC配置 */
    NVIC_InitTypeDef        NVIC_TIM6;
    NVIC_TIM6.NVIC_IRQChannel                    = TIM6_IRQn;    // TIM3 中断
    NVIC_TIM6.NVIC_IRQChannelPreemptionPriority  =  3;           // 优先级不用很高
    NVIC_TIM6.NVIC_IRQChannelSubPriority         =  3;           // 优先级不用很高
    NVIC_TIM6.NVIC_IRQChannelCmd                 = ENABLE;       // IRQ 通道使能

    NVIC_Init(&NVIC_TIM6);

    /* 8. 使能计数器 */
    TIM6->CTLR1 |= TIM_CEN;
}

/**
 * @description: TIM6的中断服务函数
 * @return {*}
 */
void TIM6_IRQHandler(void)
{
    TIM6->INTFR &= ~TIM_UIF;
}
