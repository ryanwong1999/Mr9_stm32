/*=============================================================================
*  Copyright (C) ".strftime("%Y")." All rights reserved.
*
*  文件名："App_LiftMoto.c"
*  作者：hrx
*  创建时间：2022年4月11日 
*  修改时间：
*  说明：PID
*
==============================================================================*/
/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include "App_PID.h"
#include "includes.h"
//#include "Bsp_TIM8Cfg.h"
//#include "Bsp_PWMCfg.h"

uint8_t PID_Sampling_Period = 0;    // PID 采样率

PID_typedef LeftPID;	// 左PID 调节
PID_typedef RightPID;	// 右PID 调节

PID_typedef PID_Stru;
PID_typedef PID_BACK_Stru;
float  Kp = 1;      	// 比例常数
float  Ti = 0;       	// 积分时间常数
float  Td = 0;       	// 微分时间常数

/*=============================================================================
*  函数名 ：IncPID_Init
*  作   者：hrx
*  创建时间：2022年4月11日 
*  修改时间：
*  输   入：
*           
*  输   出：
*  说   明：PID参数初始化
*/
void IncPID_Init(void)
{
	LeftPID.SumErr = 0;
	LeftPID.LastErr = 0;	// Error[-1]
	LeftPID.PrevErr = 0;	// Error[-2]
	
	LeftPID.KP = 0;				// 比例常数Proportion
	LeftPID.KI = 0;      	// 积分常数Integral
	LeftPID.KD = 0;    		// 微分常数Derivative
	LeftPID.SetPoint = 0;
	
	RightPID.SumErr = 0;
	RightPID.LastErr = 0;	// Error[-1]
	RightPID.PrevErr = 0;	// Error[-2]
	
	RightPID.KP = 0;   		// 比例常数Proportion
	RightPID.KI = 0;     	// 积分常数Integral
	RightPID.KD = 0;   		// 微分常数Derivative
	RightPID.SetPoint  =0;	
	
	PID_Stru.SumErr = 0;
	PID_Stru.LastErr = 0;	// Error[-1]
	PID_Stru.PrevErr = 0;	// Error[-2]
	
	PID_Stru.KP = 20;   	// 比例常数Proportion
	PID_Stru.KI = 0;     	// 积分常数Integral
	PID_Stru.KD = 30;   	// 微分常数Derivative
	PID_Stru.SetPoint  =0;	
}


/*=============================================================================
*  函数名 ：IncPIDCalc
*  作   者：hrx
*  创建时间：2022年4月11日 
*  修改时间：
*  输   入：
*           
*  输   出：
*  说   明：增量式PID 控制计算
*/
int IncPIDCalc(PID_typedef *pPid, uint16_t iNextPoint)
{
	int iError, iIncPid;
	
	iError = pPid->SetPoint - iNextPoint;	// 当前误差
	iIncPid=															// 增量计算
           pPid->KP * iError						// e[k]项
         - pPid->KI * pPid->LastErr	   	// e[k-1]
         + pPid->KD * pPid->PrevErr; 		// e[k-2]


	// 存储误差，用于下次计算
	pPid->PrevErr = pPid->LastErr; 				//存储误差，便于下次计算
	pPid->LastErr = iError;
	// 返回增量值
	return(iIncPid) ;
	
}

/*=============================================================================
*  函数名 ：LocPIDCalc
*  作   者：hrx
*  创建时间：2022年4月11日 
*  修改时间：
*  输   入：
*           
*  输   出：
*  说   明：位置式PID 控制计算
*/
int LocPIDCalc(PID_typedef *pPid, uint16_t icurPoint)
{
	int iError, dError ;

	iError = icurPoint - pPid->SetPoint;	// 当前误差

	pPid->SumErr += iError ;
	
	dError = iError -  pPid->LastErr;
	
	pPid->LastErr = iError ;
	// 返回增量值
	return(pPid->KP * iError							// 比例
		+ pPid->KI * pPid->SumErr          	// 积分
	  + pPid->KD * dError) ;							// 微分
}


/*=============================================================================
*  函数名 ：INCPID_Init
*  作   者：hrx
*  创建时间：2022年4月11日 
*  修改时间：
*  输   入：
*           
*  输   出：
*  说   明：PID参数初始化
*/
void INCPID_Init(void)
{
	LeftPID.PrevErr = 0;
	LeftPID.LastErr = 0;
	
	RightPID.PrevErr = 0;
	RightPID.LastErr = 0;
	
}

/*=============================================================================
*  函数名 ：Get_INCPID_PWM
*  作   者：hrx
*  创建时间：2022年4月11日 
*  修改时间：
*  输   入：
*           
*  输   出：
*  说   明：增量式PID 控制，PWM调节
*/
int16_t Get_INCPID_PWM(PID_typedef *pPid,uint16_t set_point, uint16_t cur_point,int16_t pwm)
{
	int iError, iIncPid;
	uint16_t set_tmp;
	pPid->SetPoint = set_point;
	
	iError = pPid->SetPoint - cur_point;	// 当前误差
	
	if(abs(set_point) >  abs(cur_point)){	// get slower
	  pPid->KP = 580/(float)(set_point + cur_point);
		pPid->KI = 500/(float)(set_point + cur_point);
		pPid->KD = 0;
	}else{       													//get faster
		pPid->KP = 580/(float)(set_point + cur_point);
		pPid->KI = 460/(float)(set_point + cur_point);
		pPid->KD = 0;
	}	

	iIncPid=															// 增量计算
           pPid->KP * iError						// e[k]项
         - pPid->KI * pPid->LastErr			// e[k-1]
         + pPid->KD * pPid->PrevErr;		// e[k-2]

	// 存储误差，用于下次计算 
	pPid->PrevErr = pPid->LastErr; 				//存储误差，便于下次计算
	pPid->LastErr = iError;
	// 返回增量值

	set_tmp = (set_point + cur_point)/2;
	if(iIncPid > 40000/set_tmp){
		iIncPid = 40000/set_tmp;
	}else if(iIncPid < -40000/set_tmp){
		iIncPid = -40000/set_tmp;
	}else{
		iIncPid = iIncPid;
	}
	
	if(iIncPid > 40){
		iIncPid = 40;
	}else if(iIncPid < -40){
		iIncPid = -40;
	}
	
	return iIncPid;
	
}


/*=============================================================================
*  函数名 ：Get_INCPID_PWM
*  作   者：hrx
*  创建时间：2022年4月11日 
*  修改时间：
*  输   入：
*           
*  输   出：
*  说   明：增量式PID 控制，自动充电PWM调节
*/
int16_t Get_Chg_INCPID(PID_typedef *pPid,uint16_t set_point, uint16_t cur_point,int16_t pwm)
{
	int iError, iIncPid;
	uint16_t set_tmp;
	pPid->SetPoint = set_point;
	
	iError = pPid->SetPoint - cur_point;	// 当前误差
	if(abs(set_point) >  abs(cur_point)){	// get slower
		pPid->KP = 1000/(float)(set_point + cur_point);
		pPid->KI = 800/(float)(set_point + cur_point);
		pPid->KD = 0;

	}else{																//get faster
		pPid->KP = 900/(float)(set_point + cur_point);
		pPid->KI = 750/(float)(set_point + cur_point);
		pPid->KD = 0;

	}
	
	iIncPid=															// 增量计算
           pPid->KP * iError	         	// e[k]项
         - pPid->KI * pPid->LastErr	   	// e[k-1]
         + pPid->KD * pPid->PrevErr; 		// e[k-2]


	pPid->PrevErr = pPid->LastErr; 				//存储误差，便于下次计算
	pPid->LastErr = iError;
	// 返回增量值

	set_tmp = (set_point + cur_point)/2;
	if(iIncPid > 40000/set_tmp){
		iIncPid = 40000/set_tmp;
	}else if(iIncPid < -40000/set_tmp){
		iIncPid = -40000/set_tmp;
	}else{
		iIncPid = iIncPid;
	}
	
	if(iIncPid > 50){
		iIncPid = 50;
	}else if(iIncPid < -50){
		iIncPid = -50;
	}
	
	return iIncPid;
	
}

/* CopyRight : Copyright(c) 1998 - 2015 YZRobot. All Rights Reserved */


