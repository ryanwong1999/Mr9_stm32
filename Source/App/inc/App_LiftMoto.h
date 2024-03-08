/*=============================================================================
*  Copyright (C) ".2016 " All rights reserved.
*
*  �ļ�����"App_LiftMoto.h"
*  ���ߣ�
*  ����ʱ�䣺2018��07��05�� 
*  �޸�ʱ�䣺
*  ˵����
*
==============================================================================*/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __APP_LIFTMOTO_H
#define __APP_LIFTMOTO_H

#ifdef __cplusplus
extern "C" {
#endif 

/* Includes ------------------------------------------------------------------*/
#include "stdbool.h"
#include "stm32f4xx.h"

/* 
#define LIFT_UP   		1
#define LIFT_DOWN 		2
#define LIFT_STOP 		0	 
*/                                                                                                                            

#define MAX_HEIGHT    	80	 		// ��߸߶�	���ʽ��144  80  70
#define MAX_LIFT_TIM  	2000		// ���ʽ��10200   // ������߸߶�����ʱ��

/*
#define MAX_HEIGHT    95  		// ��ɽ��һ̨
#define MAX_LIFT_TIM  7200  
 
#define MAX_HEIGHT    117
#define MAX_LIFT_TIM  9100  
*/

typedef enum
{
	LIFT_STOP = 0x00,
	LIFT_UP 	= 0x01,
	LIFT_DOWN = 0x02,
}Lift_Cmd;


typedef struct
{
	uint8_t Status;
	uint8_t Limit_Switch_Flag;	// ��λ��־
	uint8_t OverCurrent_Flag;
	bool Lift_OK_flag;
	uint16_t Height;						// �߶�
	uint16_t Set_Height;
	uint16_t Lift_Tim;					// ����ʱ��
	uint8_t Init_OK_Flag;
	Lift_Cmd Cmd;
}LiftMoto_Type;


extern LiftMoto_Type Lift_Moto;

void Get_Lift_time(uint8_t _cmd);
void LiftMoto_Set(uint8_t _cmd);
uint8_t Get_LimitSwitch(void);
uint16_t Get_LiftMoto_Height(void);
uint8_t Get_OverCurFlag(uint8_t _cmd, uint16_t _cur);
void LiftMoto_GoTO_SetHeiht(uint16_t *_set_height);
void LiftMoto_Process(void);
void LiftMoto_Init(void);

#ifdef __cplusplus
}
#endif

#endif 
	 