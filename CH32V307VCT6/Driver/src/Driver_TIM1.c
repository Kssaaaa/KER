#include "Driver_TIM1.h"
#include "stdio.h"
#include "Driver_USART.h"


void TIM1_PWM_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
		
    GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
    TIM_InternalClockConfig(TIM1);//�����ڲ�ʱ��  ��ͨ��һ��ͨ��
	
	TIM_TimeBaseInitTypeDef Timebasestructer;
	Timebasestructer.TIM_ClockDivision=TIM_CKD_DIV1 ;    	// ʱ��Ϊ72MHz   ѡ��1  Ϊ����Ƶ  ��72 000 000 Hz
	Timebasestructer.TIM_CounterMode=TIM_CounterMode_Up; //  ����ģʽ   Ϊ���ϼ���  ����0��ʼ
	Timebasestructer.TIM_Period=1000 -1;                		//   ARR    �Զ���װֵ ����������ʱ  �ʹ����ж�
	Timebasestructer.TIM_Prescaler=96 -1;             	 //   PSC     Ԥ��Ƶ     ��72MHz Ƶ������Ϊ10k Hz
	Timebasestructer.TIM_RepetitionCounter=0;    
    TIM_TimeBaseInit( TIM1, &Timebasestructer);
    TIM_ARRPreloadConfig(TIM1,ENABLE);
	//��ʱ����Ԫһ��    ����һ���ж�

	TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_OCStructInit(&TIM_OCInitStructure);//����Ĭ����
	
	TIM_OCInitStructure.TIM_OCMode      = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OCPolarity  = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 0;		//CCR
	
	TIM_OC1Init(TIM1, &TIM_OCInitStructure);
    TIM_OC2Init(TIM1, &TIM_OCInitStructure);
	TIM_OC3Init(TIM1, &TIM_OCInitStructure);
	TIM_OC4Init(TIM1, &TIM_OCInitStructure);//�����ĸ�PWMͨ��
	
    TIM_CtrlPWMOutputs(TIM1,ENABLE);
    TIM_Cmd(TIM1, ENABLE);                  //��TIIM3
 
}
 //�����ٶ�
void PWM_SetCompare(uint16_t Compare1,uint16_t Compare2,uint16_t Compare3,uint16_t Compare4)
{
	TIM_SetCompare1(TIM1, Compare1);
	TIM_SetCompare2(TIM1, Compare2);
	TIM_SetCompare3(TIM1, Compare3);
	TIM_SetCompare4(TIM1, Compare4);
}



    
