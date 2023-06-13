/*****************************************************************
* CopyRight : Copyright(c) 1998 - 2015 YZRobot. All Rights Reserved
* FileName  : App_PID.h
* Author    : zhangzhen
* Date      : 2015��8��21��
*****************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __APP_PID_H
#define __APP_PID_H

#ifdef __cplusplus
 extern "C" {
#endif 
  
/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
 
                                            // ʹ��һ����ʱ����Ϊϵͳ������ʱ��
#define PID_SAMPLE_PER              20000   // PID ������ �궨��   20000us -> 20ms
	 
/********************************************************
PID �ṹ��
********************************************************/
typedef struct 
{
 int16_t SetPoint;	// �趨Ŀ��
 int16_t SumErr;		// �������
 float KP ;      		// ��������
 float KI ;        	// ���ֳ���
 float KD;       		// ΢�ֳ���
 int16_t LocErr;		// λ�ò�
 int16_t LastErr;		// ��һ�����
 int16_t PrevErr;		// ΢�����
}PID_typedef;
	 
extern PID_typedef LeftPID;     // ��PID ����
extern PID_typedef RightPID;		// ��PID ����
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


