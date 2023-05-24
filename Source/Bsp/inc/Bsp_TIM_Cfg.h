/*=============================================================================
*  Copyright (C) ".2016 " All rights reserved.
*
*  文件名："Bsp_TIM2_Cfg.h"
*  作者：zz  (zhangzhen123good@163.com)
*  创建时间：2016年07月25日 
*  修改时间：
*  说明：
*
==============================================================================*/


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __BSP_TIM2_CFG_H
#define __BSP_TIM2_CFG_H

#ifdef __cplusplus
 extern "C" {
#endif 

	 
/* Includes ------------------------------------------------------------------*/
#include "stdbool.h"
#include "stm32f4xx.h"
	 
#define BASIC_TIM_CLK       		RCC_APB1Periph_TIM2
#define BASIC_TIM           		TIM2
	 
void  TIM2_Cfg_Init( uint16_t iArr, uint16_t iPsc );
void TIMx_Cfg_Init( uint32_t rcc_periph,TIM_TypeDef* TIMx,uint16_t iArr, uint16_t iPsc );
void TIM7_Cfg_Init(u16 arr,u16 psc);

	 
	 
#ifdef __cplusplus
}
#endif

#endif /*__BSP_TIM2_CFG_H */


/************************ (C) COPYRIGHT YzRobot  *****END OF FILE****/


