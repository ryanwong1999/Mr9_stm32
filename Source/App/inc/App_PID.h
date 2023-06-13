/*****************************************************************
* CopyRight : Copyright(c) 1998 - 2015 YZRobot. All Rights Reserved
* FileName  : App_PID.h
* Author    : zhangzhen
* Date      : 2015年8月21日
*****************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __APP_PID_H
#define __APP_PID_H

#ifdef __cplusplus
 extern "C" {
#endif 
  
/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
 
                                            // 使用一个定时器作为系统基本定时器
#define PID_SAMPLE_PER              20000   // PID 采样率 宏定义   20000us -> 20ms
	 
/********************************************************
PID 结构体
********************************************************/
typedef struct 
{
 int16_t SetPoint;	// 设定目标
 int16_t SumErr;		// 积分误差
 float KP ;      		// 比例常数
 float KI ;        	// 积分常数
 float KD;       		// 微分常数
 int16_t LocErr;		// 位置差
 int16_t LastErr;		// 上一次误差
 int16_t PrevErr;		// 微分误差
}PID_typedef;
	 
extern PID_typedef LeftPID;     // 左PID 调节
extern PID_typedef RightPID;		// 右PID 调节
extern PID_typedef PID_Stru;
void IncPID_Init(void);
int IncPIDCalc(PID_typedef *pPid, uint16_t iNextPoint);
int LocPIDCalc(PID_typedef *pPid, uint16_t iNextPoint);
void PID_Set(void);
void INCPID_Init(void);
void PID_SetPoint(PID_typedef *pPid, uint16_t iSetPointVal);
int16_t Get_INCPID_PWM(PID_typedef *pPid,uint16_t set_point, uint16_t cur_point,int16_t pwm);
int16_t Get_Chg_INCPID(PID_typedef *pPid,uint16_t set_point, uint16_t cur_point,int16_t pwm);

#ifdef __cplusplus
}
#endif
  
#endif



/* CopyRight : Copyright(c) 1998 - 2015 YZRobot. All Rights Reserved */


