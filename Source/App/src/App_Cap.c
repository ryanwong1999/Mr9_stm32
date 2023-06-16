/*=============================================================================
*  Copyright (C) ".strftime("%Y")." All rights reserved.
*
*  文件名："App_Cap.c"
*  作者：hrx
*  创建时间：2022年4月11日 
*  修改时间：
*  说明：定时器捕获中断函数处理，采样电机脉冲，采用外购电机驱动器无需用到该函数
*
==============================================================================*/

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include <stdbool.h>
#include "includes.h"

int position = 0;
uint8_t SPEED_CAP_STA  = 0x00;

/*=============================================================================
*  函数名 ：Moto_Sample_Delay10us
*  作   者：hrx
*  创建时间：2022年4月11日 
*  修改时间：
*  输   入：
*           
*  输   出：
*  说   明：脉冲采样时间计数
*/
void Moto_Sample_Delay10us(void)
{
	Robot_Moto.Left_Tim10us ++;
	Robot_Moto.Right_Tim10us ++;
	Robot_Moto.Tim10us ++;
}

/*=============================================================================
*  函数名 ：TIM8_CC_IRQHandler
*  作   者：hrx
*  创建时间：2022年4月11日 
*  修改时间：
*  输   入：
*           
*  输   出：
*  说   明：定时器捕获中断函数
*/
void TIM8_CC_IRQHandler(void) 
{
//	position = TIM_GetCounter(TIM8);
  if(TIM_GetITStatus(TIM8, TIM_IT_Update) != RESET) 
	{
    position++;
		printf("position: %d\r\n", position);
    TIM_ClearITPendingBit(TIM8, TIM_IT_Update);
  }
//	if(TIM_GetITStatus(TIM8, TIM_IT_CC2) != RESET) 
//	{
//    int position = TIM_GetCounter(TIM8);
//		printf("position: %d\r\n", position);
//    TIM_ClearITPendingBit(TIM8, TIM_IT_CC2);
//  }
}

//void TIM8_CC_IRQHandler(void)
//{
//static uint16_t Left_Period_Old = 0x0000;
//static uint16_t Left_Period_New = 0x0000;

//static uint16_t Right_Period_Old = 0x0000;
//static uint16_t Right_Period_New = 0x0000;
//	
//static uint8_t left_pulse_flag = 0;
//static uint8_t right_pulse_flag = 0;
//	
//uint16_t tmp =0;
//	
//#if SYSTEM_SUPPORT_OS  //使用UCOS操作系统
//	OSIntEnter();    
//#endif
//	
//	
//	
//	// 左
//	if(TIM_GetITStatus( CAP_TIM, TIM_IT_CC_LEFT) != RESET)  // CH3 发生捕获事件
//	{			

//		Robot_Moto.Left_Pulse_Cnt ++;
//		left_pulse_flag = 1;
//    Robot_Moto.Left_Locked_Pulse ++;// 堵转检测
//			

//		// 计算脉宽
//		if((SPEED_CAP_STA&0x10) !=0x10) 
//		{
//			SPEED_CAP_STA |= 0x10;                    // bit4 = 1   ,第一次发生捕获标志位
//			Left_Period_Old = TIM_GetCapture2(CAP_TIM);  // 记录第一次捕获的计数值					
//		}else
//		{
//			Left_Period_New=TIM_GetCapture2(CAP_TIM);              // 记录捕获的时刻
//			if(Left_Period_New < Left_Period_Old) 
//			{
//				tmp = 0xFFFF ;
//			}else  tmp = 0;		
//			
//			Robot_Moto.Left_Period = (Left_Period_New + tmp) - Left_Period_Old ;
//			Left_Period_Old = Left_Period_New;					//新值赋给旧值
//			SPEED_CAP_STA |= 0x80;   // bit3 = 1
//			Robot_Moto.Left_Tim10us = 0;
//    }		
//		
//		TIM_ClearITPendingBit( CAP_TIM, TIM_IT_CC_LEFT);       // 清中断标志					
//	}
//	
//	// 右
//	if (TIM_GetITStatus(CAP_TIM, TIM_IT_CC_RIGHT) != RESET)   // CH4 发生捕获事件
//	{	
//		 Robot_Moto.Right_Pulse_Cnt ++;
//		right_pulse_flag = 1;
//		Robot_Moto.Right_Locked_Pulse ++;  				 // 自动充电堵转检测
//		// 计算脉宽
//		if((SPEED_CAP_STA&0x01) != 0x01)               // 第一次捕获
//		{
//			SPEED_CAP_STA |= 0x01 ;                      // 标记第一次捕获到上升沿
//			Right_Period_Old = TIM_GetCapture1(CAP_TIM);    // 记录第一次上升沿时间
//		}else
//		{
//			Right_Period_New = TIM_GetCapture1(CAP_TIM);    // 记录第二次上升沿时间 
//			if(Right_Period_New < Right_Period_Old)      // 如果计数器初始值大于末尾值，说明计数器有溢出
//			{
//				tmp = 0xFFFF ;
//			}else  tmp = 0;
//			Robot_Moto.Right_Period = (Right_Period_New + tmp) - Right_Period_Old ;
//			Right_Period_Old = Right_Period_New;					//新的给旧的
//			SPEED_CAP_STA |= 0x08;    // bit3 = 1
//      Robot_Moto.Right_Tim10us = 0;
//		}		
//		TIM_ClearITPendingBit( CAP_TIM, TIM_IT_CC_RIGHT);       // 清中断标志		
//	}
//	
//		// 左C2
//	if (TIM_GetITStatus(CAP_TIM, TIM_IT_CC4) != RESET)   // CH4 发生捕获事件
//	{	
//		if(left_pulse_flag == 1){
//			left_pulse_flag = 0;
//		if(LEFT_READ_C1 != 0){
//			Robot_Moto.Left_Real_Dir = -1;
//		}else{
//			Robot_Moto.Left_Real_Dir = 1;
//		}
//		
//		if(Robot_Moto.Left_Real_Dir == 1)
//		{
//			Robot_Moto.Left_Pulse_Val++ ;
//		}else if(Robot_Moto.Left_Real_Dir == -1)
//		{
//			Robot_Moto.Left_Pulse_Val-- ;
//		}
//	}
//		
//		TIM_ClearITPendingBit( CAP_TIM, TIM_IT_CC4);       // 清中断标志		
//	}	
//	// 右C2
//	if (TIM_GetITStatus(CAP_TIM, TIM_IT_CC3) != RESET)   // CH4 发生捕获事件
//	{	
//		if(right_pulse_flag == 1){
//			right_pulse_flag = 0;
//		if(RIGHT_READ_C1 != 0){
//			Robot_Moto.Right_Real_Dir = 1;
//		}else{
//			Robot_Moto.Right_Real_Dir = -1;
//		}
//		if(Robot_Moto.Right_Real_Dir == 1)              // 方向判别
//		{
//			Robot_Moto.Right_Pulse_Val++ ;
//		}else if(Robot_Moto.Right_Real_Dir == -1)   
//		{
//			Robot_Moto.Right_Pulse_Val-- ;
//		}	
//	}
//		TIM_ClearITPendingBit( CAP_TIM, TIM_IT_CC3);       // 清中断标志		
//	}	
//	
//	#ifdef SYSTEM_SUPPORT_OS	 
//	OSIntExit();  											 
//  #endif	
//}



