/*=============================================================================
*  Copyright (C) ".strftime("%Y")." All rights reserved.
*
*  �ļ�����"App_Cap.c"
*  ���ߣ�hrx
*  ����ʱ�䣺2022��4��11�� 
*  �޸�ʱ�䣺
*  ˵������ʱ�������жϺ�����������������壬�����⹺��������������õ��ú���
*
==============================================================================*/

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include <stdbool.h>
#include "includes.h"

int position = 0;
uint8_t SPEED_CAP_STA  = 0x00;

/*=============================================================================
*  ������ ��Moto_Sample_Delay10us
*  ��   �ߣ�hrx
*  ����ʱ�䣺2022��4��11�� 
*  �޸�ʱ�䣺
*  ��   �룺
*           
*  ��   ����
*  ˵   �����������ʱ�����
*/
void Moto_Sample_Delay10us(void)
{
	Robot_Moto.Left_Tim10us ++;
	Robot_Moto.Right_Tim10us ++;
	Robot_Moto.Tim10us ++;
}

/*=============================================================================
*  ������ ��TIM8_CC_IRQHandler
*  ��   �ߣ�hrx
*  ����ʱ�䣺2022��4��11�� 
*  �޸�ʱ�䣺
*  ��   �룺
*           
*  ��   ����
*  ˵   ������ʱ�������жϺ���
*/
void TIM8_CC_IRQHandler(void) 
{
//	position = TIM_GetCounter(TIM8);
  if(TIM_GetITStatus(TIM8, TIM_IT_Update) != RESET) 
	{
    position++;
		printf("position: %d\r\n", position);
    TIM_ClearITPendingBit(TIM8, TIM_IT_Update);
  }
//	if(TIM_GetITStatus(TIM8, TIM_IT_CC2) != RESET) 
//	{
//    int position = TIM_GetCounter(TIM8);
//		printf("position: %d\r\n", position);
//    TIM_ClearITPendingBit(TIM8, TIM_IT_CC2);
//  }
}

//void TIM8_CC_IRQHandler(void)
//{
//static uint16_t Left_Period_Old = 0x0000;
//static uint16_t Left_Period_New = 0x0000;

//static uint16_t Right_Period_Old = 0x0000;
//static uint16_t Right_Period_New = 0x0000;
//	
//static uint8_t left_pulse_flag = 0;
//static uint8_t right_pulse_flag = 0;
//	
//uint16_t tmp =0;
//	
//#if SYSTEM_SUPPORT_OS  //ʹ��UCOS����ϵͳ
//	OSIntEnter();    
//#endif
//	
//	
//	
//	// ��
//	if(TIM_GetITStatus( CAP_TIM, TIM_IT_CC_LEFT) != RESET)  // CH3 ���������¼�
//	{			

//		Robot_Moto.Left_Pulse_Cnt ++;
//		left_pulse_flag = 1;
//    Robot_Moto.Left_Locked_Pulse ++;// ��ת���
//			

//		// ��������
//		if((SPEED_CAP_STA&0x10) !=0x10) 
//		{
//			SPEED_CAP_STA |= 0x10;                    // bit4 = 1   ,��һ�η��������־λ
//			Left_Period_Old = TIM_GetCapture2(CAP_TIM);  // ��¼��һ�β���ļ���ֵ					
//		}else
//		{
//			Left_Period_New=TIM_GetCapture2(CAP_TIM);              // ��¼�����ʱ��
//			if(Left_Period_New < Left_Period_Old) 
//			{
//				tmp = 0xFFFF ;
//			}else  tmp = 0;		
//			
//			Robot_Moto.Left_Period = (Left_Period_New + tmp) - Left_Period_Old ;
//			Left_Period_Old = Left_Period_New;					//��ֵ������ֵ
//			SPEED_CAP_STA |= 0x80;   // bit3 = 1
//			Robot_Moto.Left_Tim10us = 0;
//    }		
//		
//		TIM_ClearITPendingBit( CAP_TIM, TIM_IT_CC_LEFT);       // ���жϱ�־					
//	}
//	
//	// ��
//	if (TIM_GetITStatus(CAP_TIM, TIM_IT_CC_RIGHT) != RESET)   // CH4 ���������¼�
//	{	
//		 Robot_Moto.Right_Pulse_Cnt ++;
//		right_pulse_flag = 1;
//		Robot_Moto.Right_Locked_Pulse ++;  				 // �Զ�����ת���
//		// ��������
//		if((SPEED_CAP_STA&0x01) != 0x01)               // ��һ�β���
//		{
//			SPEED_CAP_STA |= 0x01 ;                      // ��ǵ�һ�β���������
//			Right_Period_Old = TIM_GetCapture1(CAP_TIM);    // ��¼��һ��������ʱ��
//		}else
//		{
//			Right_Period_New = TIM_GetCapture1(CAP_TIM);    // ��¼�ڶ���������ʱ�� 
//			if(Right_Period_New < Right_Period_Old)      // �����������ʼֵ����ĩβֵ��˵�������������
//			{
//				tmp = 0xFFFF ;
//			}else  tmp = 0;
//			Robot_Moto.Right_Period = (Right_Period_New + tmp) - Right_Period_Old ;
//			Right_Period_Old = Right_Period_New;					//�µĸ��ɵ�
//			SPEED_CAP_STA |= 0x08;    // bit3 = 1
//      Robot_Moto.Right_Tim10us = 0;
//		}		
//		TIM_ClearITPendingBit( CAP_TIM, TIM_IT_CC_RIGHT);       // ���жϱ�־		
//	}
//	
//		// ��C2
//	if (TIM_GetITStatus(CAP_TIM, TIM_IT_CC4) != RESET)   // CH4 ���������¼�
//	{	
//		if(left_pulse_flag == 1){
//			left_pulse_flag = 0;
//		if(LEFT_READ_C1 != 0){
//			Robot_Moto.Left_Real_Dir = -1;
//		}else{
//			Robot_Moto.Left_Real_Dir = 1;
//		}
//		
//		if(Robot_Moto.Left_Real_Dir == 1)
//		{
//			Robot_Moto.Left_Pulse_Val++ ;
//		}else if(Robot_Moto.Left_Real_Dir == -1)
//		{
//			Robot_Moto.Left_Pulse_Val-- ;
//		}
//	}
//		
//		TIM_ClearITPendingBit( CAP_TIM, TIM_IT_CC4);       // ���жϱ�־		
//	}	
//	// ��C2
//	if (TIM_GetITStatus(CAP_TIM, TIM_IT_CC3) != RESET)   // CH4 ���������¼�
//	{	
//		if(right_pulse_flag == 1){
//			right_pulse_flag = 0;
//		if(RIGHT_READ_C1 != 0){
//			Robot_Moto.Right_Real_Dir = 1;
//		}else{
//			Robot_Moto.Right_Real_Dir = -1;
//		}
//		if(Robot_Moto.Right_Real_Dir == 1)              // �����б�
//		{
//			Robot_Moto.Right_Pulse_Val++ ;
//		}else if(Robot_Moto.Right_Real_Dir == -1)   
//		{
//			Robot_Moto.Right_Pulse_Val-- ;
//		}	
//	}
//		TIM_ClearITPendingBit( CAP_TIM, TIM_IT_CC3);       // ���жϱ�־		
//	}	
//	
//	#ifdef SYSTEM_SUPPORT_OS	 
//	OSIntExit();  											 
//  #endif	
//}



