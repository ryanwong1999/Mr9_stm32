/*=============================================================================
*  Copyright (C) ".strftime("%Y")." All rights reserved.
*
*  文件名："App_Pms.c"
*  作者：hrx
*  创建时间：2022年4月11日 
*  修改时间：
*  说明：超声传感器处理
*
==============================================================================*/

/* Includes ------------------------------------------------------------------*/
#include "includes.h"

Ultra_tpyedef Ultra1;
Ultra_tpyedef Ultra2;
Ultra_tpyedef Ultra3;
Ultra_tpyedef Ultra4;


/*=============================================================================
*  函数名 ：Ultra_StartUp
*  作   者：hrx
*  创建时间：2022年4月11日 
*  修改时间：
*  输   入：
*           
*  输   出：
*  说   明：超声启动信号
*/
void Ultra_StartUp(void)
{
	static uint8_t startup_flag = 0;
	uint16_t i;

	if(startup_flag == 0)
	{
		//if((Robot_Sys.Ultra_Disable_Flag & 0x01) == 0){
			TRIG1_ON;
			Ultra1.Uswave_Sta = 0x20;
		//}
		//if((Robot_Sys.Ultra_Disable_Flag & 0x10) == 0){
			TRIG3_ON;
			Ultra3.Uswave_Sta = 0x20;
		//}
		//if((Robot_Sys.Ultra_Disable_Flag & 0x02) == 0){
			TRIG2_OFF;
			Ultra2.Uswave_Sta = 0x80;
		//}
		//if((Robot_Sys.Ultra_Disable_Flag & 0x20) == 0){
			TRIG4_OFF;
			Ultra4.Uswave_Sta = 0x80;
		//}
	}
	else
	{
		//if((Robot_Sys.Ultra_Disable_Flag & 0x01) == 0){
			TRIG1_OFF;
			Ultra1.Uswave_Sta = 0x80;
		//}
		//if((Robot_Sys.Ultra_Disable_Flag & 0x10) == 0){
			TRIG3_OFF;
			Ultra3.Uswave_Sta = 0x80;
		//}
		//if((Robot_Sys.Ultra_Disable_Flag & 0x02) == 0){
			TRIG2_ON;
			Ultra2.Uswave_Sta = 0x20;
		//} 		
		//if((Robot_Sys.Ultra_Disable_Flag & 0x20) == 0){
			TRIG4_ON;
			Ultra4.Uswave_Sta = 0x20;
		//}
	}
	
	for(i = 0; i<3000; i++) { ; }
	
	TRIG1_OFF;
	TRIG2_OFF;
	TRIG3_OFF;
	TRIG4_OFF;

	if(startup_flag == 0) startup_flag = 1;
	else startup_flag = 0;
		
  Ultra1.Out_time = 0;
  Ultra2.Out_time = 0;
	Ultra3.Out_time = 0;	
	Ultra4.Out_time = 0;
}


/*=============================================================================
*  函数名 ：Ultra_OverTime_Process
*  作   者：hrx
*  创建时间：2022年4月11日 
*  修改时间：
*  输   入：
*           
*  输   出：
*  说   明：超声超时处理
*/
void Ultra_OverTime_Process(void)
{
	if((Ultra1.Uswave_Sta&0x80) == 0) Ultra1.Distance = ULTRA_DEFAULT_DIS;
	if((Ultra2.Uswave_Sta&0x80) == 0) Ultra2.Distance = ULTRA_DEFAULT_DIS;
	if((Ultra3.Uswave_Sta&0x80) == 0) Ultra3.Distance = ULTRA_DEFAULT_DIS;
	if((Ultra4.Uswave_Sta&0x80) == 0) Ultra4.Distance = ULTRA_DEFAULT_DIS;
}


/*=============================================================================
*  函数名 ：Ultra_Param_Init
*  作   者：hrx
*  创建时间：2022年4月11日 
*  修改时间：
*  输   入：
*           
*  输   出：
*  说   明：超声距离初始化
*/
void Ultra_Param_Init(void)
{
	Ultra1.Distance = ULTRA_DEFAULT_DIS;
	Ultra2.Distance = ULTRA_DEFAULT_DIS;
	Ultra3.Distance = ULTRA_DEFAULT_DIS;
	Ultra4.Distance = ULTRA_DEFAULT_DIS;
}


/*=============================================================================
*  函数名 ：Ultra_Process
*  作   者：hrx
*  创建时间：2022年4月11日 
*  修改时间：
*  输   入：
*           
*  输   出：
*  说   明：超声处理
*/
void Ultra_Process(void)
{
	static uint16_t stop_dis;
	static uint8_t front_cnt = 0;
	static uint8_t front_reset_cnt1 = 0;
	static uint8_t front_reset_cnt2 = 0;
	static uint8_t back_cnt = 0;
	static uint8_t back_reset_cnt1 = 0;
	static uint8_t back_reset_cnt2 = 0;
	stop_dis = ULTRA_STOP_DIS;
  
	if((Robot_Sys.Ultra_Disable_Flag & 0x01) == 0)
	{
		if(Ultra1.Distance < stop_dis)
		{
			Robot_Sys.Ultra_sta |= 0x01;      	// front obstacle
			front_reset_cnt1 = 0;
		}
		else
		{
			front_reset_cnt1++;
			if(front_reset_cnt1 > 20)
			{
				front_reset_cnt1 = 0;
				Robot_Sys.Ultra_sta &= 0xfe;      // front obstacle
			}
		}
	}
	else
	{
		front_reset_cnt1 = 0;
		Robot_Sys.Ultra_sta &= 0xfe;      		// front obstacle
	}
				
	if((Robot_Sys.Ultra_Disable_Flag & 0x02) == 0)
	{
		if(Ultra2.Distance < stop_dis)
		{
			front_reset_cnt2 = 0;
			Robot_Sys.Ultra_sta |= 0x02;      	// front obstacle
		}
		else
		{
			front_reset_cnt2++;
			if(front_reset_cnt2 > 20)
			{
				front_reset_cnt2 = 0;
				Robot_Sys.Ultra_sta &= 0xfd;      // front obstacle
			}
		}				
	}
	else
	{
		front_reset_cnt2 = 0;
		Robot_Sys.Ultra_sta &= 0xfd;      		// front obstacle
	}


	if((Robot_Sys.Ultra_Disable_Flag & 0x10) == 0)
	{
		if(Ultra3.Distance < stop_dis)
		{
			back_reset_cnt1 = 0;
			Robot_Sys.Ultra_sta |= 0x10;       	// back obstacle
		}
		else
		{
			back_reset_cnt1++;
			if(back_reset_cnt1 > 20)
			{
				back_reset_cnt1 = 0;
				Robot_Sys.Ultra_sta &= 0xef;      // front obstacle
			}
		}	
	}
	else
	{
		back_reset_cnt1 = 0;
		Robot_Sys.Ultra_sta &= 0xef;      		// front obstacle
	}
					
					
	if((Robot_Sys.Ultra_Disable_Flag & 0x20) == 0)
	{
		if(Ultra4.Distance < stop_dis)
		{
			back_reset_cnt2 = 0;
			Robot_Sys.Ultra_sta |= 0x20;       // back obstacle
		}
		else
		{
			back_reset_cnt2++;
			if(back_reset_cnt2 > 20)
			{
				back_reset_cnt2 = 0;
				Robot_Sys.Ultra_sta &= 0xdf;		// front obstacle
			}
		}	
	}
	else
	{
		back_reset_cnt2 = 0;
		Robot_Sys.Ultra_sta &= 0xdf;      	// front obstacle
	}
}


/*=============================================================================
*  函数名 ：Get_Crash_Status
*  作   者：hrx
*  创建时间：2022年4月11日 
*  修改时间：
*  输   入：
*           
*  输   出：
*  说   明：获取碰撞状态
*/
uint8_t Get_Crash_Status(void)
{
	static uint16_t crash_front_cnt = 0;
	static uint16_t crash_back_cnt = 0;
	static uint8_t crash_sta = 0;
	
	if(CRASH_FRONT_READ == 0)
	{
		crash_sta |= 0x01;			// 0000 0001
		crash_front_cnt = 0;
	}
	else
	{
		crash_front_cnt++;
		if(crash_front_cnt > 1)
		{
			crash_front_cnt = 0;
			crash_sta &= 0xfe;		// 1111 1110
		}
	}
	
	if(CRASH_BACK_READ == 0)
	{
		crash_back_cnt = 0;
		crash_sta |= 0x02;			// 0000 0010
	}
	else
	{
		crash_back_cnt++;
		if(crash_back_cnt > 1)
		{
			crash_back_cnt = 0;
			crash_sta &= 0xfd;		// 1111 1101
		}
	}
	return crash_sta;
}
