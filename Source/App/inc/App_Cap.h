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
#ifndef __APP_CAP_H
#define __APP_CAP_H

#ifdef __cplusplus
 extern "C" {
#endif 

/* Includes ------------------------------------------------------------------*/
#include "stdbool.h"
#include "stm32f4xx.h"

extern uint8_t SPEED_CAP_STA;
	 
void Moto_Sample_Delay10us(void);
/*
typedef struct
{
	//uint32_t Left_Cnt;     // 
	//uint32_t Right_Cnt;	   // 
	
	uint16_t Left_Period;
	uint16_t Right_Period;
	
	uint16_t Left_Pulse_Val ;
	uint16_t Right_Pulse_Val ;

}SPEED_ICP_Type;
	 
*/
//extern SPEED_ICP_Type  Moto_Speed_ICP ;
/*
extern uint8_t SPEED_CAP_STA; 
extern uint16_t Left_Period;
extern uint16_t Right_Period;

extern int  Left_count ;
extern int  Right_count ; 
*/
/*
extern int Adjust_left_count ;			
extern int Adjust_right_count  ;
*/
	 
#ifdef __cplusplus
}
#endif

#endif /*__BSP_TIM2_CFG_H */
	 