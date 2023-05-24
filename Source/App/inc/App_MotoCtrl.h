/*=============================================================================
*  Copyright (C) ".2016 " All rights reserved.
*
*  �ļ�����"App_MotoCtrl.h"
*  ���ߣ�zz  (zhangzhen123good@163.com)
*  ����ʱ�䣺2016��07��25�� 
*  �޸�ʱ�䣺
*  ˵����
*
==============================================================================*/


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __APP_MOTOCTRL_H
#define __APP_MOTOCTRL_H

#ifdef __cplusplus
 extern "C" {
#endif 

	 
/* Includes ------------------------------------------------------------------*/
#include <stdbool.h>
#include "stm32f4xx.h"
#include "Bsp_PWM_Cfg.h"

#define MAX_PWM  900
#define MIN_PWM  50
//  ����ṹ��
typedef struct
{
	
//	int	Left_Speed ;		  // �ٶ�
	//int Right_Speed;     //
	uint8_t Startup_Flag;
	uint8_t PID_Flag;
	bool Stop_Flag;
	uint8_t Slow_Flag;
	
	uint16_t Set_Period_L;
	uint16_t Set_Period_R;
	 
	
	uint16_t Default_Period_L;
	uint16_t Default_Period_R;
	
	uint16_t Left_Tim10us ;
	uint16_t Right_Tim10us;
	uint16_t Tim10us;
	uint16_t Left_Period;   //��������
	uint16_t Right_Period;  //�ҵ������
	
	float Left_Speed;     //�����ٶ�  m/s
	float Right_Speed;  //�����ٶ�  m/s
	float Lear;        //���ٶ�    m/s
	float Angular;      //���ٶ� rad/s
	int16_t Real_Lear;        //���ٶ�    m/s
	int16_t Real_Angular;      //���ٶ� rad/s
	uint16_t Left_Period_Set;   //��������
	uint16_t Right_Period_Set;  //�ҵ������
	
	int16_t  Left_PWM_Set;
	int16_t  Right_PWM_Set; 
	int16_t  Left_PWM_Value;
	int16_t  Right_PWM_Value;
	int16_t Left_PWM_Last;
	int16_t Right_PWM_Last;
	
	int16_t Left_Pulse_Val ;  //����������,���ڸ����ػ������
	int16_t Right_Pulse_Val ;  //�ҵ��������

	int8_t Left_Locked_Pulse;  //��ת�������,�������Ӷ�ת���
	int8_t Right_Locked_Pulse;		//��ת�������
	
	int8_t Left_Pulse_Cnt;    //�����������,���ڿ��ư��Լ�� 
	int8_t Right_Pulse_Cnt;   //�ҵ���������
	
	int8_t  Left_Dir ;       // ��������
	int8_t  Right_Dir ;      // �ҵ������	
	
	int8_t  Left_Real_Dir ;       // ��������ͨ���������жϵķ���
	int8_t  Right_Real_Dir ;      // �ҵ������	��ͨ���������жϵķ���
	
	int8_t  Robot_Dir;            // �����˵� �˶�����   0 ͣ  1 ǰ 2 ǰ��  3 ��  4 �Һ�  5 �� 6 ����  7 �� 8 ��ǰ
	int8_t  Robot_Dir_per;				//ǰһ�� ״̬
	uint8_t	Turn_Angle; 
	
	uint8_t Robot_Move_Cmd;
	
	uint8_t Locked_Flag;

}Moto_Typedef ;	 


#define MOTO_LEFT_CH      PWM_LEFT_CH
#define MOTO_RIGHT_CH     PWM_RIGHT_CH


extern  Moto_Typedef  Robot_Moto ;


void  Moto_Unlock(void);
void Moto_Set_Speed(uint8_t ucChannel, int16_t sSpeed);


void  Moto_Param_Init(void) ;


void Sport_Stop(void);
void MotoCtrl_Forward(void);
void MotoCtrl_TurnRight(void);
void MotoCtrl_TurnLeft(void);
void MotoCtrl_TurnRightZeroRadius(void);
void MotoCtrl_TurnLeftZeroRadius(void);
void Sport_Back(void);
uint8_t  Wheel_Locded_Check(void);
void Get_Wheel_Dir(float _left_speed, float _right_speed,int8_t *_left_dir, int8_t *_right_dir);
uint8_t Get_Robot_Dir(float _left_pwm, float _right_pwm);

void Speed_Regulation_Process(uint16_t _set_period_l,uint16_t _set_period_r, uint8_t turn_sta);
void Speed_Set_Process(uint16_t set_period,uint16_t real_Period, int16_t *pwm_set);
void Speed_Up_Down(int16_t rate,int16_t set_pwm,int16_t *pwm_value);
void Speed_Inc(int16_t rate,int16_t _setL_pwm,int16_t _setR_pwm,Moto_Typedef *_moto);
void Speed_Del(int16_t rate,int16_t _setL_pwm,int16_t _setR_pwm,Moto_Typedef *_moto);
void Speed_PID_Process(void);
void Chg_Speed_PID_Process(void);
	
#ifdef __cplusplus
}
#endif

#endif /*__BSP_SYSTICK_CFG_H */


/************************ (C) COPYRIGHT YzRobot  *****END OF FILE****/

