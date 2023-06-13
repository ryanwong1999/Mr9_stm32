/*=============================================================================
*  Copyright (C) ".2016 " All rights reserved.
*
*  文件名："App_System.h"
*  作者：zz  (zhangzhen123good@163.com)
*  创建时间：2016年07月25日 
*  修改时间：
*  说明：
*
==============================================================================*/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __APP_SYSTEM_H
#define __APP_SYSTEM_H

#ifdef __cplusplus
extern "C" {
#endif 
	 
/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include <stdbool.h> 
#include "stm32f4xx.h"

typedef struct
{
	int16_t Level;
	int16_t Pitch;
}HeadPose_Type;

typedef struct
{
	uint32_t charger;      //充电
	uint32_t power;        //电量
}Power_Type;
	 
typedef struct
{
	uint8_t BeepMode;
	uint16_t BeepTime;
}Beep_Type;

//enum BEEP
//{
//	BEEP_TIPS1=0x01,
//	BEEP_TIPS2=0x02,
//	BEEP_TIPS3=0x03,
//	BEEP_TIPS4=0x04
//};

typedef struct
{
	uint8_t Sys_Dev_ID;         		//设备ID 号
	uint16_t Sys_Version;        		//系统版本
	uint8_t PC_Run_Flag;        		//PC机 运行标志
	uint8_t Ultra_sta;          		//障碍标志
	uint8_t Obs_IR_Front;
	uint8_t Obs_IR_Bottom;
  uint8_t Ultra_Disable_Flag;
	uint8_t IR_Front_Disable_Flag;
	uint8_t IR_Bottom_Disable_Flag;
	uint8_t Crash_Flag;  						//碰撞标志
	uint8_t Voice_flag;
	uint8_t PowerOff_flag;
	uint8_t StopDisable_flag;				//解除停止
	uint8_t Stop_flag;							//停止
	uint8_t CarLight_flag;					//车灯开关标志位
	
	bool Set_PWM_Task_Flag;
  bool Move_Speed_Task_flag;	
	bool Psc_Task_flag;
	bool Lift_Task_flag;
	bool AutoCharge_task_flag;  		//自动充电任务标记
	bool GetGuideSta_task_flag;
	bool AGV_Guide_task_flag;
	bool Remote_flag;
	bool Mergency_Stop_flag;
	bool Comm_break_flag;

//  uint16_t Comm_TimeOut_cnt;
	uint16_t Speed_Timeout_cnt;
	uint16_t Odom_Timeout_cnt;
	uint8_t Last_Task;
  uint8_t Out_en_flag;
	uint8_t Beep_en_flag;
	
	Power_Type mSysPower;
	HeadPose_Type mHeadPose;				// 头部姿态
	Beep_Type mBeepStatus;
}Sys_Type;


extern Sys_Type Robot_Sys;

void RCC_Configuration(void);
void NVIC_Configuration(void);
void System_Board_Init(void);
void System_StartTips(void);

#ifdef __cplusplus
}
#endif

#endif /* */


/************************ (C) COPYRIGHT YzRobot  *****END OF FILE****/







