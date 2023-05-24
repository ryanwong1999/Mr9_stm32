/*=============================================================================
*  Copyright (C) ".2016 " All rights reserved.
*
*  文件名："Bsp_GPIO_Cfg.h"
*  作者：zz  (zhangzhen123good@163.com)
*  创建时间：2016年07月25日 
*  修改时间：
*  说明：
*
==============================================================================*/


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __APP_ULTRASONIC_H
#define __APP_ULTRASONIC_H

#ifdef __cplusplus
 extern "C" {
#endif 

	 
/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"

	 
#define ULTRA_SLOW_DIS   150    //ultrasonic stop distance	 
#define ULTRA_STOP_DIS   400    //ultrasonic stop distance
#define ULTRA_DEFAULT_DIS   2000    //ultrasonic stop distance
	 
	 
typedef struct 
{
	uint8_t Uswave_Sta ; //ultrasonic status
	uint16_t Distance;   //ultrasonic distance;
	uint16_t Out_time;
}Ultra_tpyedef;

extern Ultra_tpyedef Ultra1;
extern Ultra_tpyedef Ultra2;
extern Ultra_tpyedef Ultra3;
extern Ultra_tpyedef Ultra4;

void Ultra_StartUp(void);
void Ultra_OverTime_Process(void);
void Ultra_Param_Init(void);
void Ultra_Process(void);
uint8_t Get_Crash_Status(void);
#ifdef __cplusplus
}
#endif

#endif /*__BSP_SYSTICK_CFG_H */


/************************ (C) COPYRIGHT YzRobot  *****END OF FILE****/


