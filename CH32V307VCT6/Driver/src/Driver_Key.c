#include "Driver_Key.h"
#include "Driver_LED.h"



/**
 * @description: 初始化按键.
 *  1. 给按键对应的io口设置工作模式: 下拉输入
 *  2. 配置复用为外部中断
 *  3. 配置外部中断控制器 EXTI
 *  4. 配置NVIC
 */
void Driver_Key_Init(void)
{
    /* 1. 开启时钟 */
    /* 1.1  GPIOE*/
    RCC->APB2PCENR |= RCC_IOPERST;
    /* 1.2  AFIO*/
    RCC->APB2PCENR |= RCC_AFIOEN;

    /* 2. 配置 PE10为下拉输入: MODE=00 CNF=10  ODR=0 */
    GPIOE->CFGHR &= ~GPIO_CFGHR_MODE10;
    GPIOE->CFGHR |= GPIO_CFGHR_CNF10_1;
    GPIOE->CFGHR &= ~GPIO_CFGHR_CNF10_0;
    GPIOE->OUTDR &= ~GPIO_OUTDR_ODR10;

    /* 3. 配置AFIO 配置PE10引脚为外部中断 EXTICR3 ?0101 */
     AFIO->EXTICR[2] &= ~AFIO_EXTICR3_EXTI10;
     AFIO->EXTICR[2] |= AFIO_EXTICR3_EXTI10_PE;

    /* 4. 配置EXTI */
    /* 4.1. 配置上升沿触发 RTSR TR10=1*/
    EXTI->RTENR |= EXTI_RTENR_TR10;
    /* 4.2 开启 LINE10, 配置的中断屏蔽寄存器 */
    EXTI->INTENR |= EXTI_INTENR_MR10;

    /* 5. 配置 NVIC */
    NVIC_InitTypeDef        nvic;
    nvic.NVIC_IRQChannel                    = EXTI2_IRQn;    // TIM3 中断
    nvic.NVIC_IRQChannelPreemptionPriority  =  5;           // 优先级不用很高
    nvic.NVIC_IRQChannelSubPriority         =  5;           // 优先级不用很高
    nvic.NVIC_IRQChannelCmd                 = ENABLE;       // IRQ 通道使能

    NVIC_Init(&nvic);
}

/**
 * @description: line 15-10的中断服务函数.
 *  一旦按键下按键1,则会执行一次这个函数
 * @return {*}
 */
void EXTI15_10_IRQHandler(void)
{
    /* 务必一定必须要清除中断标志位 */
    EXTI->INTFR |= EXTI_INTENR_MR10;

   // Delay_ms(5);
    if ((GPIOE->INDR & EXTI_INTENR_MR10) != 0)
    {
        Drviver_LED_Toggle(LED_1);
    }
}

