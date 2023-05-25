/*=============================================================================
*  Copyright (C) ".strftime("%Y")." All rights reserved.
*
*  �ļ�����"App_LiftMoto.c"
*  ���ߣ�hrx
*  ����ʱ�䣺2022��4��11�� 
*  �޸�ʱ�䣺
*  ˵����PID
*
==============================================================================*/
/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include "App_PID.h"
#include "includes.h"
//#include "Bsp_TIM8Cfg.h"
//#include "Bsp_PWMCfg.h"

uint8_t PID_Sampling_Period = 0;    // PID ������

PID_typedef LeftPID;	// ��PID ����
PID_typedef RightPID;	// ��PID ����

PID_typedef PID_Stru;
PID_typedef PID_BACK_Stru;
float  Kp = 1;      	// ��������
float  Ti = 0;       	// ����ʱ�䳣��
float  Td = 0;       	// ΢��ʱ�䳣��

/*=============================================================================
*  ������ ��IncPID_Init
*  ��   �ߣ�hrx
*  ����ʱ�䣺2022��4��11�� 
*  �޸�ʱ�䣺
*  ��   �룺
*           
*  ��   ����
*  ˵   ����PID������ʼ��
*/
void IncPID_Init(void)
{
	LeftPID.SumErr = 0;
	LeftPID.LastErr = 0;	// Error[-1]
	LeftPID.PrevErr = 0;	// Error[-2]
	
	LeftPID.KP = 0;				// ��������Proportion
	LeftPID.KI = 0;      	// ���ֳ���Integral
	LeftPID.KD = 0;    		// ΢�ֳ���Derivative
	LeftPID.SetPoint = 0;
	
	RightPID.SumErr = 0;
	RightPID.LastErr = 0;	// Error[-1]
	RightPID.PrevErr = 0;	// Error[-2]
	
	RightPID.KP = 0;   		// ��������Proportion
	RightPID.KI = 0;     	// ���ֳ���Integral
	RightPID.KD = 0;   		// ΢�ֳ���Derivative
	RightPID.SetPoint  =0;	
	
	PID_Stru.SumErr = 0;
	PID_Stru.LastErr = 0;	// Error[-1]
	PID_Stru.PrevErr = 0;	// Error[-2]
	
	PID_Stru.KP = 20;   	// ��������Proportion
	PID_Stru.KI = 0;     	// ���ֳ���Integral
	PID_Stru.KD = 30;   	// ΢�ֳ���Derivative
	PID_Stru.SetPoint  =0;	
}


/*=============================================================================
*  ������ ��IncPIDCalc
*  ��   �ߣ�hrx
*  ����ʱ�䣺2022��4��11�� 
*  �޸�ʱ�䣺
*  ��   �룺
*           
*  ��   ����
*  ˵   ��������ʽPID ���Ƽ���
*/
int IncPIDCalc(PID_typedef *pPid, uint16_t iNextPoint)
{
	int iError, iIncPid;
	
	iError = pPid->SetPoint - iNextPoint;	// ��ǰ���
	iIncPid=															// ��������
           pPid->KP * iError						// e[k]��
         - pPid->KI * pPid->LastErr	   	// e[k-1]
         + pPid->KD * pPid->PrevErr; 		// e[k-2]


	// �洢�������´μ���
	pPid->PrevErr = pPid->LastErr; 				//�洢�������´μ���
	pPid->LastErr = iError;
	// ��������ֵ
	return(iIncPid) ;
	
}

/*=============================================================================
*  ������ ��LocPIDCalc
*  ��   �ߣ�hrx
*  ����ʱ�䣺2022��4��11�� 
*  �޸�ʱ�䣺
*  ��   �룺
*           
*  ��   ����
*  ˵   ����λ��ʽPID ���Ƽ���
*/
int LocPIDCalc(PID_typedef *pPid, uint16_t icurPoint)
{
	int iError, dError ;

	iError = icurPoint - pPid->SetPoint;	// ��ǰ���

	pPid->SumErr += iError ;
	
	dError = iError -  pPid->LastErr;
	
	pPid->LastErr = iError ;
	// ��������ֵ
	return(pPid->KP * iError							// ����
		+ pPid->KI * pPid->SumErr          	// ����
	  + pPid->KD * dError) ;							// ΢��
}


/*=============================================================================
*  ������ ��INCPID_Init
*  ��   �ߣ�hrx
*  ����ʱ�䣺2022��4��11�� 
*  �޸�ʱ�䣺
*  ��   �룺
*           
*  ��   ����
*  ˵   ����PID������ʼ��
*/
void INCPID_Init(void)
{
	LeftPID.PrevErr = 0;
	LeftPID.LastErr = 0;
	
	RightPID.PrevErr = 0;
	RightPID.LastErr = 0;
	
}

/*=============================================================================
*  ������ ��Get_INCPID_PWM
*  ��   �ߣ�hrx
*  ����ʱ�䣺2022��4��11�� 
*  �޸�ʱ�䣺
*  ��   �룺
*           
*  ��   ����
*  ˵   ��������ʽPID ���ƣ�PWM����
*/
int16_t Get_INCPID_PWM(PID_typedef *pPid,uint16_t set_point, uint16_t cur_point,int16_t pwm)
{
	int iError, iIncPid;
	uint16_t set_tmp;
	pPid->SetPoint = set_point;
	
	iError = pPid->SetPoint - cur_point;	// ��ǰ���
	
	if(abs(set_point) >  abs(cur_point)){	// get slower
	  pPid->KP = 580/(float)(set_point + cur_point);
		pPid->KI = 500/(float)(set_point + cur_point);
		pPid->KD = 0;
	}else{       													//get faster
		pPid->KP = 580/(float)(set_point + cur_point);
		pPid->KI = 460/(float)(set_point + cur_point);
		pPid->KD = 0;
	}	

	iIncPid=															// ��������
           pPid->KP * iError						// e[k]��
         - pPid->KI * pPid->LastErr			// e[k-1]
         + pPid->KD * pPid->PrevErr;		// e[k-2]

	// �洢�������´μ��� 
	pPid->PrevErr = pPid->LastErr; 				//�洢�������´μ���
	pPid->LastErr = iError;
	// ��������ֵ

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
*  ������ ��Get_INCPID_PWM
*  ��   �ߣ�hrx
*  ����ʱ�䣺2022��4��11�� 
*  �޸�ʱ�䣺
*  ��   �룺
*           
*  ��   ����
*  ˵   ��������ʽPID ���ƣ��Զ����PWM����
*/
int16_t Get_Chg_INCPID(PID_typedef *pPid,uint16_t set_point, uint16_t cur_point,int16_t pwm)
{
	int iError, iIncPid;
	uint16_t set_tmp;
	pPid->SetPoint = set_point;
	
	iError = pPid->SetPoint - cur_point;	// ��ǰ���
	if(abs(set_point) >  abs(cur_point)){	// get slower
		pPid->KP = 1000/(float)(set_point + cur_point);
		pPid->KI = 800/(float)(set_point + cur_point);
		pPid->KD = 0;

	}else{																//get faster
		pPid->KP = 900/(float)(set_point + cur_point);
		pPid->KI = 750/(float)(set_point + cur_point);
		pPid->KD = 0;

	}
	
	iIncPid=															// ��������
           pPid->KP * iError	         	// e[k]��
         - pPid->KI * pPid->LastErr	   	// e[k-1]
         + pPid->KD * pPid->PrevErr; 		// e[k-2]


	pPid->PrevErr = pPid->LastErr; 				//�洢�������´μ���
	pPid->LastErr = iError;
	// ��������ֵ

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


