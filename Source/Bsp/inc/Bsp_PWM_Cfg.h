/*=============================================================================
*  Copyright (C) ".2016 " All rights reserved.
*
*  文件名："Bsp_PWM_Cfg.c"
*  作者：zz  (zhangzhen123good@163.com)
*  创建时间：2016年07月25日 
*  修改时间：
*  说明：
*
==============================================================================*/


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __BSP_PWM_CFG_H
#define __BSP_PWM_CFG_H

#ifdef __cplusplus
 extern "C" {
#endif 
	 
/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
	 
// GPIO -> PWM 
//PWM GPIO define ,wheel speed set
#define  PWM_WHEEL_GPIO_CLK    RCC_AHB1Periph_GPIOD
#define  PWM_WHEEL_PORT        GPIOD
	 
#define  PWM_LEFT_PIN      GPIO_Pin_12       //  
#define  PWM_RIGHT_PIN     GPIO_Pin_13     //   	 

#define  PWM_WHEEL_TIM_CLK   	RCC_APB1Periph_TIM4
#define  PWM_WHEEL_TIM        TIM4
	
#define  PWM_LEFT_CH          2	    // 右侧 PWM	 
#define  PWM_RIGHT_CH         1      // 左侧 PWM	 

	 
#define PWM_HEAD_PORT_CLK 		RCC_AHB1Periph_GPIOB
#define PWM_HEAD_PORT 				GPIOB

#define PWM_PITCH_PIN					GPIO_Pin_8         //tim4_ch3
#define PWM_LEVEL_PIN					GPIO_Pin_9         //tim4_ch4

#define  PWM_HEAD_TIM_CLK  	RCC_APB1Periph_TIM4
#define  PWM_HEAD_TIM				TIM4

#define PWM_PITCH_CH    1
#define PWM_LEVEL_CH    2


void Head_PWM_Set(uint8_t _usChannel,uint16_t _usPWM_Val);

void  PWM_Wheel_Cfg_Init(uint16_t iArr, uint16_t iPsc);	   //轮子电机控制PWM初始化
void  PWM_Output_Set(uint8_t ucChx, uint16_t  uiPWM_Val); //设置轮子PWM值

void  PWM_Head_Cfg_Init(uint16_t iArr, uint16_t iPsc);   //头部电机(舵机)PWM初始化
void  PWM_Head_Set(uint8_t _usChannel,uint16_t _usPWM_Val);//设置PWM值

//void  SetHeadPitchPosition( float  pos ); //设置俯仰角度
//void  SetHeadLevelPosition( float  pos );  //设置水平角度	

void  SetHeadPitchPosition( float  pos ,float offset); //设置俯仰角度
void  SetHeadLevelPosition( float  pos,float offset);


#ifdef __cplusplus
}
#endif

#endif /*__BSP_PWM_CFG_H */


/************************ (C) COPYRIGHT YzRobot  *****END OF FILE****/
