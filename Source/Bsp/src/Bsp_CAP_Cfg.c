/*=============================================================================
*  Copyright (C) ".strftime("%Y")." All rights reserved.
*
*  文件名："Bsp_CAP_Cfg.c"
*  作者：hrx
*  创建时间：2022年3月5日 
*  修改时间：
*  说明：输入捕获初始化
*
==============================================================================*/

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include "includes.h"

#define TIM_PERIOD 						65535

#define ENCODER_TIM_PSC  			0				/* 计数器分频 */
#define ENCODER_TIM_PERIOD  	65535		/* 计数器最大值 */
#define CNT_INIT 							1000		/* 计数器初值 */

/*=============================================================================
*  函数名 ：TIM_ICP_Cfg_Init
*  作   者：hrx
*  创建时间：2022年3月5日 
*  修改时间：
*  输   入：iArr  -> 装载值
*           iPsc  ->分频系数?
*  输   出：
*  说   明：定时器捕获初始化配置
*/
void TIM_ICP_Cfg_Init(uint16_t iArr, uint16_t iPsc)
{	
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_ICInitTypeDef ICP_ICInitStructure;
	
	RCC_AHB1PeriphClockCmd(CAP_PORT_CLK, ENABLE); 	        				// 使能PORT时钟	
	RCC_APB2PeriphClockCmd(CAP_TIM_CLK, ENABLE);										// TIM时钟使能    
	
	GPIO_InitStructure.GPIO_Pin 	= CAP_LEFT_PIN| CAP_RIGHT_PIN | CAP2_LEFT_PIN | CAP2_RIGHT_PIN;	// GPIO
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AF;            				// 复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	    				// 速度100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;          				// 推挽复用输出
	GPIO_InitStructure.GPIO_PuPd 	= GPIO_PuPd_DOWN;          				// 下拉
	GPIO_Init(CAP_PORT, &GPIO_InitStructure);              					// 初始化PORT
	
	GPIO_PinAFConfig(CAP_PORT, GPIO_PinSource6, GPIO_AF_TIM8);  		// PB0复用位定时器3
	GPIO_PinAFConfig(CAP_PORT, GPIO_PinSource7, GPIO_AF_TIM8);  		// PB0复用位定时器3
	GPIO_PinAFConfig(CAP_PORT, GPIO_PinSource8, GPIO_AF_TIM8);  		// PB0复用位定时器3
	GPIO_PinAFConfig(CAP_PORT, GPIO_PinSource9, GPIO_AF_TIM8);  		// PB0复用位定时器3
	
	TIM_TimeBaseStructure.TIM_Period = iArr-1; 	
	TIM_TimeBaseStructure.TIM_Prescaler = iPsc-1;		                // 高级控制定时器时钟源TIMxCLK = HCLK=180MHz 
	                                                                // 设定定时器频率为=TIMxCLK/(TIM_Prescaler+1)=100KHz
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;		        // 对外部时钟进行采样的时钟分频,这里没有用到 */
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;	      // 计数方式
	TIM_TimeBaseInit(CAP_TIM, &TIM_TimeBaseStructure);	            // 初始化定时器TIMx, x[1,8]	
	/* 初始化TIM5输入捕获参数 */
	ICP_ICInitStructure.TIM_Channel 		= TIM_Channel_1;            // CC1S=01 	选择输入端 IC1映射到TI1上
	ICP_ICInitStructure.TIM_ICPolarity 	= TIM_ICPolarity_Rising;	  // 上升沿捕获
	ICP_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; // 映射到TI1上
	ICP_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	          // 配置输入分频,不分频 
	ICP_ICInitStructure.TIM_ICFilter 		= 0x00;                     // IC1F=0000 配置输入滤波器 不滤波
	TIM_ICInit(CAP_TIM, &ICP_ICInitStructure);
	
	ICP_ICInitStructure.TIM_Channel 		= TIM_Channel_2;            // CC1S=01 	选择输入端 IC1映射到TI1上
	ICP_ICInitStructure.TIM_ICPolarity 	= TIM_ICPolarity_Rising;	  // 上升沿捕获
	ICP_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; // 映射到TI1上
	ICP_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	          // 配置输入分频,不分频 
	ICP_ICInitStructure.TIM_ICFilter 		= 0x00;                     // IC1F=0000 配置输入滤波器 不滤波
	TIM_ICInit(CAP_TIM, &ICP_ICInitStructure);	
	/* 初始化TIM5输入捕获参数 */
	ICP_ICInitStructure.TIM_Channel 		= TIM_Channel_3;            // CC1S=01 	选择输入端 IC1映射到TI1上
	ICP_ICInitStructure.TIM_ICPolarity 	= TIM_ICPolarity_Rising;	  // 上升沿捕获
	ICP_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; // 映射到TI1上
	ICP_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	          // 配置输入分频,不分频 
	ICP_ICInitStructure.TIM_ICFilter 		= 0x00;                     // IC1F=0000 配置输入滤波器 不滤波
	TIM_ICInit(CAP_TIM, &ICP_ICInitStructure);
	
	ICP_ICInitStructure.TIM_Channel 		= TIM_Channel_4;            // CC1S=01 	选择输入端 IC1映射到TI1上
	ICP_ICInitStructure.TIM_ICPolarity 	= TIM_ICPolarity_Rising;	  // 上升沿捕获
	ICP_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; // 映射到TI1上
	ICP_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	          // 配置输入分频,不分频 
	ICP_ICInitStructure.TIM_ICFilter 		= 0x00;                     // IC1F=0000 配置输入滤波器 不滤波
	TIM_ICInit(CAP_TIM, &ICP_ICInitStructure);	 
	
	TIM_ITConfig(CAP_TIM, TIM_IT_CC1, ENABLE);											// 允许更新中断 ,允许CC1IE捕获中断	
	TIM_ITConfig(CAP_TIM, TIM_IT_CC2, ENABLE);											// 允许更新中断 ,允许CC1IE捕获中断	
	TIM_ITConfig(CAP_TIM, TIM_IT_CC3, ENABLE);											// 允许更新中断 ,允许CC1IE捕获中断	
	TIM_ITConfig(CAP_TIM, TIM_IT_CC4, ENABLE);											// 允许更新中断 ,允许CC1IE捕获中断	
	
	TIM_Cmd(CAP_TIM, DISABLE); 	                           					// 禁止定时器5
	/* NVIC 配置, 统一放置于main.c 统一处理 */
}

void TIM8_Configuration(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStruct;
    TIM_ICInitTypeDef TIM_ICInitStruct;

    /* GPIO初始化 */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

    GPIO_InitStruct.GPIO_Pin 		= GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStruct.GPIO_Mode 	= GPIO_Mode_AF;
    GPIO_InitStruct.GPIO_Speed 	= GPIO_Speed_100MHz;
    GPIO_InitStruct.GPIO_OType 	= GPIO_OType_PP;
    GPIO_InitStruct.GPIO_PuPd 	= GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOC, &GPIO_InitStruct);

    GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_TIM8);
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource7, GPIO_AF_TIM8);
    /* 时基初始化 */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);
    TIM_DeInit(TIM8);
    TIM_TimeBaseStructInit(&TIM_TimeBaseStruct);
    TIM_TimeBaseStruct.TIM_Prescaler			= ENCODER_TIM_PSC;
    TIM_TimeBaseStruct.TIM_Period 				= ENCODER_TIM_PERIOD;
    TIM_TimeBaseStruct.TIM_ClockDivision 	= TIM_CKD_DIV1;
    TIM_TimeBaseStruct.TIM_CounterMode 		= TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM8, &TIM_TimeBaseStruct);
    /* 编码器模式配置：同时捕获通道1与通道2(即4倍频)，极性均为Rising */
    TIM_EncoderInterfaceConfig(TIM8, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
    TIM_ICStructInit(&TIM_ICInitStruct);
    TIM_ICInitStruct.TIM_ICFilter = 0;
    TIM_ICInit(TIM8, &TIM_ICInitStruct);
    TIM_SetCounter(TIM8, CNT_INIT);
    TIM_ClearFlag(TIM8, TIM_IT_Update);
    TIM_ITConfig(TIM8, TIM_IT_Update, ENABLE);
    TIM_Cmd(TIM8, ENABLE);
}


int encoderNum = CNT_INIT;
int encoderOld = CNT_INIT;
int hight = 0, hight_read = 0, hight_mm = 0, reset_cnt = 0;
/* 读取定时器计数值 */
static int read_encoder(void)
{
	encoderNum = TIM_GetCounter(TIM8);	
	/* 不继续升降了 */
	if(encoderNum == encoderOld)
	{
		if(Lift_Moto.Set_Height == 0)
		{
			reset_cnt++;
			if(reset_cnt >= 100)
			{
				encoderNum = CNT_INIT;
				encoderOld = CNT_INIT;
				TIM_SetCounter(TIM8, CNT_INIT);		/* CNT设初值 */	
				reset_cnt = 0;
			}
		}		
		if(Lift_Moto.Set_Height == MAX_HEIGHT_3)
		{
			reset_cnt++;
			if(reset_cnt >= 100)
			{
//				encoderNum = 4750;
//				encoderOld = 4750;
//				TIM_SetCounter(TIM8, 4750);		/* CNT设初值 */	
				encoderNum = 4260;
				encoderOld = 4260;
				TIM_SetCounter(TIM8, 4260);		/* CNT设初值 */	
				Lift_Moto.Lift_OK_flag = true;
				reset_cnt = 0;
			}
		}
	}
	/* 还在继续升降 */
	else
	{
		reset_cnt = 0;
		/* 脉冲有大跳动 */
		if(abs(encoderNum - encoderOld) > 20) 
		{
			encoderNum = encoderOld;
			TIM_SetCounter(TIM8, encoderOld);
		}
		/* 700mm行程升降杆有时候上升的时候脉冲会回跳 */
		else if(Lift_Moto.Height < Lift_Moto.Set_Height)
		{
			if(encoderNum - encoderOld < 0)
			{
//				printf("Lift_Moto.Height: %d Lift_Moto.Set_Height: %d\r\n",Lift_Moto.Height, Lift_Moto.Set_Height);
//				printf("encoderNum: %d encoderOld: %d\r\n",encoderNum, encoderOld);
				encoderNum = encoderOld + 3;
				TIM_SetCounter(TIM8, encoderOld + 3);
			}
			else
				encoderOld = encoderNum;
		}
		else 
			encoderOld = encoderNum;
		
		if(encoderNum < CNT_INIT) 
			TIM_SetCounter(TIM8, CNT_INIT);		/* CNT设初值 */	
	}
//	printf("encoderNum: %d\r\n", encoderNum);
	return encoderNum;
}

/* 计算高度（被另一个定时器每100ms调用1次） */ 
int GetLiftHeight(void)
{
	/*读取编码器的值，正负代表旋转方向*/
	hight_read = read_encoder();
//	hight = (hight_read - 1000) * 0.0213;		/* (80/3750) = 0.0213 */
	hight = (hight_read - 1000) * 0.0215;		/* (70/3260) = 0.0218 */
	hight_mm = ((hight_read - 1000) * 0.0213) * 10;
//	printf("hight: %d       hight_mm: %d\r\n", hight, hight_mm);
	return hight;
}