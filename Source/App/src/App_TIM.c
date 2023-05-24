/*=============================================================================
*  Copyright (C) "2016." All rights reserved.
*
*  �ļ�����"Bsp_TIM2_Cfg.c"
*  ���ߣ�hrx
*  ����ʱ�䣺2016��07��25�� 
*  �޸�ʱ�䣺
*  ˵������ʱ���жϴ���
*


==============================================================================*/
/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include <stdbool.h>
#include "App_TIM.h"

#include "includes.h"

/*=============================================================================
*  ������ ��TIM2_IRQHandler
*  ��   �ߣ�hrx
*  ����ʱ�䣺2022��4��11�� 
*  �޸�ʱ�䣺
*  ��   �룺
*           
*  ��   ����
*  ˵   ����TIM2�жϴ���������1ms��ʱ
*/
void TIM2_IRQHandler(void)
{
	static uint8_t last_ultra_en = 0;
	static int8_t last_lever_offset = 0;
	static int8_t last_pitch_offset = 0;
	static uint16_t tim1ms_cnt = 0;
	static uint16_t lift_cnt = 0;
	static uint16_t errom_cnt = 0;
	#if SYSTEM_SUPPORT_OS  //ʹ��UCOS����ϵͳ
		OSIntEnter();    
	#endif
	
	if((TIM_GetITStatus( TIM2, TIM_IT_Update) == SET ))
	{
		TIM_ClearITPendingBit(TIM2 , TIM_IT_Update);  
		
		Charge_Tim1ms();
		//Robot_Moto.Locked_Flag = Wheel_Locded_Check();
	
		UsartToPC.Comm_TimeOut++;
		UsartToDrv.Comm_TimeOut++;
	  Robot_Sys.Speed_Timeout_cnt++;
		Robot_Sys.Odom_Timeout_cnt++;
		errom_cnt++;
		if(errom_cnt > 1000)		//1��
		{
			errom_cnt = 0;

			if(last_ultra_en != Robot_Sys.Ultra_Disable_Flag){
				AT24CXX_WriteOneByte(8,Robot_Sys.Ultra_Disable_Flag);
				last_ultra_en = AT24CXX_ReadOneByte(8);
			}
			if(last_lever_offset != Head_Status.Level_Offset)
			{
				AT24CXX_WriteOneByte(6,Head_Status.Level_Offset);   //��ǰ����
				last_lever_offset = AT24CXX_ReadOneByte(6);
			}
			if(last_pitch_offset != Head_Status.Pitch_Offset)
			{
				AT24CXX_WriteOneByte(7,Head_Status.Pitch_Offset);
				last_pitch_offset = AT24CXX_ReadOneByte(7);
			}
		}
	 if((UsartToEnviro.Rx_Sta & 0x01) != 0){
		  UsartToEnviro.Comm_TimeOut ++;
		  if( UsartToEnviro.Comm_TimeOut > 10){
				UsartToEnviro.Comm_TimeOut = 0;
			  UsartToEnviro.Usart_Rx_OK = true;
		    UsartToEnviro.Rx_Sta = 0;
			}
		} 
		tim1ms_cnt++;
		if(tim1ms_cnt >= 1000)
		{
			tim1ms_cnt = 0;
		}
   #ifdef LIFTMOTO_TASK_OS
		lift_cnt ++;
		if(lift_cnt >= 10){
		  lift_cnt = 0;
			Get_Lift_time(Lift_Moto.Cmd);
		}
		#endif
		
		if(Robot_Sys.mBeepStatus.BeepMode !=0)
		{
			switch(Robot_Sys.mBeepStatus.BeepMode)
			{
				case 0x01:
					BEEP_ON;
					if(Robot_Sys.mBeepStatus.BeepTime++ > 100)
					{				
						BEEP_OFF;
						Robot_Sys.mBeepStatus.BeepMode = 0;
						Robot_Sys.mBeepStatus.BeepTime = 0;
					}
					break;
					
				case 0x02:
					BEEP_ON;
					if((Robot_Sys.mBeepStatus.BeepTime++ > 100) && (Robot_Sys.mBeepStatus.BeepTime++ < 200))
					{
						BEEP_OFF;
					}
					else if((Robot_Sys.mBeepStatus.BeepTime++ > 200) && (Robot_Sys.mBeepStatus.BeepTime++ < 300))
					{
						BEEP_ON;
					}
					else if(Robot_Sys.mBeepStatus.BeepTime > 300)
					{
						BEEP_OFF;
						Robot_Sys.mBeepStatus.BeepMode = 0;
						Robot_Sys.mBeepStatus.BeepTime = 0;						
					}
					break;
					
				case 0x03:
					BEEP_ON;
					if(Robot_Sys.mBeepStatus.BeepTime++ > 400)
					{				
						BEEP_OFF;
						Robot_Sys.mBeepStatus.BeepMode = 0;
						Robot_Sys.mBeepStatus.BeepTime = 0;
					}
					break;
			}
		}
		else	
		{
			Robot_Sys.mBeepStatus.BeepTime = 0;
		}
		
		
	#if SYSTEM_SUPPORT_OS  //ʹ��UCOS����ϵͳ
	OSIntExit();    
  #endif

	}
}
	
/*=============================================================================
*  ������ ��TIM1_UP_TIM10_IRQHandler
*  ��   �ߣ�hrx
*  ����ʱ�䣺2022��4��11�� 
*  �޸�ʱ�䣺
*  ��   �룺
*           
*  ��   ����
*  ˵   ����TIM1�жϴ���������10us��ʱ
*/
void TIM1_UP_TIM10_IRQHandler (void)
{
	static uint16_t tim10us = 0;
	static uint16_t tim500ms = 0;
	
	#if SYSTEM_SUPPORT_OS  //ʹ��UCOS����ϵͳ
		OSIntEnter();    
	#endif
	
	if( (TIM_GetITStatus(TIM1, TIM_IT_Update) == SET))
	{
		TIM_ClearITPendingBit(TIM1, TIM_IT_Update);  
	  IR_Code_Delay10us();
		Moto_Sample_Delay10us();
  }

	#if SYSTEM_SUPPORT_OS  //ʹ��UCOS����ϵͳ
		OSIntExit();    
  #endif
}

/*=============================================================================
*  ������ ��TIM7_IRQHandler
*  ��   �ߣ�hrx
*  ����ʱ�䣺2022��4��11�� 
*  �޸�ʱ�䣺
*  ��   �룺
*           
*  ��   ����
*  ˵   ����TIM7�жϴ���������10ms��ʱ
*/
void TIM7_IRQHandler(void)
{ 	
	OS_ERR err;
	#if SYSTEM_SUPPORT_OS  //ʹ��UCOS����ϵͳ
		OSIntEnter();    
	#endif
	if(TIM_GetITStatus(TIM7, TIM_IT_Update) != RESET)//�Ǹ����ж�
	{	 			   
		UsartToDrv.Usart_Rx_OK = true;
    #ifdef SYSTEM_SUPPORT_OS	 
			OSSemPost(&UsartToDrv_SEM, OS_OPT_POST_1, &err);//????? 
    #endif	
   
		TIM_ClearITPendingBit(TIM7, TIM_IT_Update);  //���TIM7�����жϱ�־    
		TIM_Cmd(TIM7, DISABLE);  //�ر�TIM7 
		
		#if SYSTEM_SUPPORT_OS  //ʹ��UCOS����ϵͳ
			OSIntExit();    
		#endif
	}	    
}