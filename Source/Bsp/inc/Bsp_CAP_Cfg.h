/*=============================================================================
*  Copyright (C) ".2016 " All rights reserved.
*
*  文件名："Bsp_TIM4_ICP_Cfg.h"
*  作者：zz  (zhangzhen123good@163.com)
*  创建时间：2016年07月25日 
*  修改时间：
*  说明：
*
==============================================================================*/


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __BSP_TIM3_ICP_CFG_H
#define __BSP_TIM3_ICP_CFG_H

#ifdef __cplusplus
 extern "C" {
#endif 

	 
/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
	
// 输入捕获GPIO	 
#define  CAP_PORT_CLK  RCC_AHB1Periph_GPIOC
#define  CAP_PORT      GPIOC
	 
#define  CAP_LEFT_PIN      GPIO_Pin_7
#define  CAP_RIGHT_PIN      GPIO_Pin_6
	 
#define  CAP2_LEFT_PIN      GPIO_Pin_9
#define  CAP2_RIGHT_PIN     GPIO_Pin_8
//  输入捕获定时器	 
#define  CAP_TIM_CLK   RCC_APB2Periph_TIM8
#define  CAP_TIM       TIM8
#define  CAP_TIM_AF    GPIO_AF_TIM8

//HALL输入
#define MOTOR_HALL1    GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_6)
#define MOTOR_HALL2    GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_7)
	 
#define  TIM_IT_CC_LEFT	   TIM_IT_CC2
#define  TIM_IT_CC_RIGHT   TIM_IT_CC1
	
#define LEFT_READ_C1       GPIO_ReadInputDataBit(CAP_PORT,CAP_LEFT_PIN)
#define LEFT_READ_C2       GPIO_ReadInputDataBit(CAP_PORT,CAP2_LEFT_PIN)
#define RIGHT_READ_C1       GPIO_ReadInputDataBit(CAP_PORT,CAP_RIGHT_PIN)
#define RIGHT_READ_C2       GPIO_ReadInputDataBit(CAP_PORT,CAP2_RIGHT_PIN)
//#define  CAP_ARR      0xFFFF
	 
	 
	 

void TIM_ICP_Cfg_Init(uint16_t iArr, uint16_t iPsc);
void TIM8_Configuration(void);
int32_t GetLiftHeight(void);
//void TIM8_CC_IRQHandler(void);
#ifdef __cplusplus
}
#endif

#endif /*__BSP_SYSTICK_CFG_H */


/************************ (C) COPYRIGHT YzRobot  *****END OF FILE****/

