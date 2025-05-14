#include "Driver_GPIO.h"

void Driver_GPIO_Init(void)
{
    //PWM con
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	
    GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_ResetBits(GPIOB,GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7);

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	
    GPIO_InitTypeDef TB6612_STBY;
	TB6612_STBY.GPIO_Mode = GPIO_Mode_AF_PP;
	TB6612_STBY.GPIO_Pin = GPIO_Pin_6;
	TB6612_STBY.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &TB6612_STBY);
    GPIO_SetBits(GPIOA,GPIO_Pin_6);

}
