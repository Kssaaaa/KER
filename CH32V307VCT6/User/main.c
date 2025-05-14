/********************************** (C) COPYRIGHT *******************************
* File Name          : main.c
* Author             : WCH
* Version            : V1.0.0
* Date               : 2021/06/06
* Description        : Main program body.
*********************************************************************************
* Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
* Attention: This software (modified or not) and binary are used for 
* microcontroller manufactured by Nanjing Qinheng Microelectronics.
*******************************************************************************/

/*
 *@Note
 USART Print debugging routine:
 USART1_Tx(PA9).
 This example demonstrates using USART1(PA9) as a print debug port output.

*/

#include "debug.h"
#include "Driver_USART.h"
#include "atk_ms6050.h"
/* Global typedef */

/* Global define */

/* Global Variable */


/*********************************************************************
 * @fn      main
 *
 * @brief   Main program.
 *
 * @return  none
 */
int main(void)
{
	SystemInit();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	SystemCoreClockUpdate();
	Delay_Init();
	//USART_Printf_Init(115200);	
	Driver_USART1_Init();

	int a =4, tem_Flag=4,GA_Flag=4,AX_Flag=4;
	int16_t tem,gx,gy,gz,ax,ay,az;
	a=atk_ms6050_init();
	tem_Flag=atk_ms6050_get_temperature(&tem);
	GA_Flag=atk_ms6050_get_gyroscope(&gx,&gy,&gz);
	AX_Flag=atk_ms6050_get_accelerometer(&ax,&ay,&az);
	if(a==0){printf("mpu6050初始化成功");}
	else{printf("mpu6050初始化失败");}
	if(tem_Flag==0){printf("mpu6050温度模块初始化成功");}
	else{printf("mpu6050温度初始化失败");}
	if(GA_Flag==0){printf("mpu6050陀螺仪模块初始化成功");}
	else{printf("mpu6050陀螺仪初始化失败");}
	if(AX_Flag==0){printf("mpu6050加速度模块初始化成功");}
	else{printf("mpu6050加速度初始化失败");}

	while(1)
    {
	atk_ms6050_get_gyroscope(&gx,&gy,&gz);
	}
}

