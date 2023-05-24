/*=============================================================================
*  Copyright (C) ".2016 " All rights reserved.
*
*  �ļ�����"Bsp_PWM_Cfg.c"
*  ���ߣ�zz  (zhangzhen123good@163.com)
*  ����ʱ�䣺2016��07��25�� 
*  �޸�ʱ�䣺
*  ˵����
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
	
#define  PWM_LEFT_CH          2	    // �Ҳ� PWM	 
#define  PWM_RIGHT_CH         1      // ��� PWM	 

	 
#define PWM_HEAD_PORT_CLK 		RCC_AHB1Periph_GPIOB
#define PWM_HEAD_PORT 				GPIOB

#define PWM_PITCH_PIN					GPIO_Pin_8         //tim4_ch3
#define PWM_LEVEL_PIN					GPIO_Pin_9         //tim4_ch4

#define  PWM_HEAD_TIM_CLK  	RCC_APB1Periph_TIM4
#define  PWM_HEAD_TIM				TIM4

#define PWM_PITCH_CH    1
#define PWM_LEVEL_CH    2


void Head_PWM_Set(uint8_t _usChannel,uint16_t _usPWM_Val);

void  PWM_Wheel_Cfg_Init(uint16_t iArr, uint16_t iPsc);	   //���ӵ������PWM��ʼ��
void  PWM_Output_Set(uint8_t ucChx, uint16_t  uiPWM_Val); //��������PWMֵ

void  PWM_Head_Cfg_Init(uint16_t iArr, uint16_t iPsc);   //ͷ�����(���)PWM��ʼ��
void  PWM_Head_Set(uint8_t _usChannel,uint16_t _usPWM_Val);//����PWMֵ

//void  SetHeadPitchPosition( float  pos ); //���ø����Ƕ�
//void  SetHeadLevelPosition( float  pos );  //����ˮƽ�Ƕ�	

void  SetHeadPitchPosition( float  pos ,float offset); //���ø����Ƕ�
void  SetHeadLevelPosition( float  pos,float offset);


#ifdef __cplusplus
}
#endif

#endif /*__BSP_PWM_CFG_H */


/************************ (C) COPYRIGHT YzRobot  *****END OF FILE****/
