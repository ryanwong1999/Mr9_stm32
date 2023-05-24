/*=============================================================================
*  Copyright (C) "2016." All rights reserved.
*
*  文件名："Bsp_TIM_Cfg.c"
*  作者：hrx
*  创建时间：2022年3月5日 
*  修改时间：
*  说明：定时器初始化
*
==============================================================================*/

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include <stdbool.h>
#include "Bsp_TIM_Cfg.h"


/*=============================================================================
*  函数名 ：TIM2_Cfg_Init
*  作   者：
*  创建时间：2022年3月5日 
*  修改时间：
*  输   入：iArr  -> 装载值
*           iPsc  ->分频系数
*  输   出：
*  说   明：TIM2初始化
*/
void TIM2_Cfg_Init( uint16_t iArr, uint16_t iPsc )
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	
	RCC_APB1PeriphClockCmd(BASIC_TIM_CLK, ENABLE); 	
	
	//定时器时钟源TIMxCLK = 2 * PCLK1  
	//				PCLK1 = HCLK / 4 
	//				=> TIMxCLK=HCLK/2=SystemCoreClock/2=90MHz
	// 设定定时器频率为=TIMxCLK/(TIM_Prescaler+1)=10000Hz
	TIM_TimeBaseStructure.TIM_Period = iArr-1;                 // 自动重载
	TIM_TimeBaseStructure.TIM_Prescaler = iPsc-1;	             // 定时器分频
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;// 向上计数
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;		 // 对外部时钟进行采样的时钟分频,这里没有用到 */
	TIM_TimeBaseInit(BASIC_TIM, &TIM_TimeBaseStructure);
	
	TIM_ClearFlag(BASIC_TIM, TIM_FLAG_Update);	    // 清除定时器更新中断标志位
	TIM_ITConfig(BASIC_TIM,TIM_IT_Update,ENABLE);		// 开启定时器更新中断
	TIM_Cmd(BASIC_TIM, ENABLE);			                // 使能定时器
}

/*=============================================================================
*  函数名 ：TIMx_Cfg_Init
*  作   者：
*  创建时间：2022年3月5日 
*  修改时间：
*  输   入：rcc_periph -> 定时器时钟
             TIMx ->  定时器选择
            iArr  -> 装载值
*           iPsc  ->分频系数
*  输   出：
*  说   明：TIM初始化，该函数适用所有的定时器初始化
*/ 
void TIMx_Cfg_Init( uint32_t rcc_periph,TIM_TypeDef* TIMx,uint16_t iArr, uint16_t iPsc )
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

	if(TIMx == TIM1 || TIMx == TIM8)
	{
		RCC_APB2PeriphClockCmd(rcc_periph, ENABLE);
	}
	else
	{
		RCC_APB1PeriphClockCmd(rcc_periph, ENABLE); 	
	}
	
	//定时器时钟源TIMxCLK = 2 * PCLK1  
	//				PCLK1 = HCLK / 4 
	//				=> TIMxCLK=HCLK/2=SystemCoreClock/2=90MHz
	// 设定定时器频率为=TIMxCLK/(TIM_Prescaler+1)=10000Hz
	TIM_TimeBaseStructure.TIM_Period = iArr-1;                 // 自动重载
	TIM_TimeBaseStructure.TIM_Prescaler = iPsc-1;	             // 定时器分频
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;// 向上计数
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;		 // 对外部时钟进行采样的时钟分频,这里没有用到 */
	TIM_TimeBaseInit(TIMx, &TIM_TimeBaseStructure);
	
	TIM_ClearFlag(TIMx, TIM_FLAG_Update);	    // 清除定时器更新中断标志位
	TIM_ITConfig(TIMx,TIM_IT_Update,ENABLE);		// 开启定时器更新中断
	TIM_Cmd(TIMx, ENABLE);			                // 使能定时器
}


/*=============================================================================
*  函数名 ：TIM7_Cfg_Init
*  作   者：
*  创建时间：2022年3月5日 
*  修改时间：
*  输   入：iArr  -> 装载值
*           iPsc  ->分频系数
*  输   出：
*  说   明：TIM7初始化
*/ 
void TIM7_Cfg_Init(u16 arr,u16 psc)
{	
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);//TIM7时钟使能    
	
	//定时器TIM7初始化
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM7, &TIM_TimeBaseStructure); //根据指定的参数初始化TIMx的时间基数单位
 
	TIM_ITConfig(TIM7,TIM_IT_Update,ENABLE ); //使能指定的TIM7中断,允许更新中断
 	TIM_Cmd(TIM7, DISABLE);  //关闭TIM7 
}
