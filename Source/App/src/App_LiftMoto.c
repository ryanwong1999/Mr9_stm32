/*=============================================================================
*  Copyright (C) ".strftime("%Y")." All rights reserved.
*
*  文件名："App_LiftMoto.c"
*  作者：hrx
*  创建时间：2022年4月11日 
*  修改时间：
*  说明：升降控制处理
*
==============================================================================*/


/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include <stdbool.h>

#include "includes.h"

uint16_t Lift_Cnt = 0;

LiftMoto_Type Lift_Moto;

/*=============================================================================
*  函数名 ：Get_Lift_time
*  作   者：hrx
*  创建时间：2022年4月11日 
*  修改时间：
*  输   入：
*           
*  输   出：
*  说   明：获取升降时间
*/
void Get_Lift_time(uint8_t _cmd)
{
	switch(_cmd)
	{
		/* moto up */
		case LIFT_UP:
		  if((Lift_Moto.Limit_Switch_Flag & 0x01) != 0 || (Lift_Moto.OverCurrent_Flag & 0x01) != 0)
			{
				Lift_Moto.Lift_Tim = MAX_LIFT_TIM;
			}
			else
			{
				if(Lift_Moto.Lift_Tim < MAX_LIFT_TIM)
					Lift_Moto.Lift_Tim++;
			}
			break;
			
		/* moto down */
		case LIFT_DOWN:
			if((Lift_Moto.Limit_Switch_Flag & 0x02) != 0 || (Lift_Moto.OverCurrent_Flag & 0x02) != 0)
			{
				Lift_Moto.Lift_Tim = 0;
			}
			else
			{
				if( Lift_Moto.Lift_Tim > 0)
					Lift_Moto.Lift_Tim--;
			}
			break;
			 
		case LIFT_STOP:
			if((Lift_Moto.Limit_Switch_Flag & 0x01) != 0)
			{
				Lift_Moto.Lift_Tim = MAX_LIFT_TIM;
			}
			else if((Lift_Moto.Limit_Switch_Flag & 0x02) != 0)
			{
				Lift_Moto.Lift_Tim = 0;
			}
			else
			{
				Lift_Moto.Lift_Tim = Lift_Moto.Lift_Tim;
		  }
			break;
			 
		default:
			break;
	}
}

/*=============================================================================
*  函数名 ：LiftMoto_Set
*  作   者：hrx
*  创建时间：2022年4月11日 
*  修改时间：
*  输   入：
*           
*  输   出：
*  说   明：升降控制
*/
void LiftMoto_Set(uint8_t _cmd)
{
	static uint8_t down_limit_sta = 0;
	switch(_cmd)
	{
		/* moto up */
		case LIFT_UP:
			Lift_Moto.Init_OK_Flag = 0;
			if((Lift_Moto.Limit_Switch_Flag & 0x01) != 0 || (Lift_Moto.OverCurrent_Flag & 0x01) != 0)
			{
				LIFTMOTO_DISABLE;
			}
			else if(Lift_Moto.Lift_Tim >= MAX_LIFT_TIM)
			{
				LIFTMOTO_DISABLE;
			}
			else
			{
				LIFTMOTO_UP;	
				LIFTMOTO_ENABLE;
			}
			break;
			
		/* moto down */
		case LIFT_DOWN:
			if(Lift_Moto.Init_OK_Flag == 1)
			{
				LIFTMOTO_DISABLE;
			}
			else if((Lift_Moto.Limit_Switch_Flag & 0x02) != 0 || (Lift_Moto.OverCurrent_Flag & 0x02) != 0 )
			{
				LIFTMOTO_DISABLE;
				delay_ms(50);
				LIFTMOTO_UP;
				LIFTMOTO_ENABLE;
				delay_ms(300);
				LIFTMOTO_DISABLE;
				Lift_Moto.Init_OK_Flag = 1;
			}
			else
			{
				LIFTMOTO_DOWN;
				LIFTMOTO_ENABLE;
			}
			break;
			
		case LIFT_STOP:  
			LIFTMOTO_DISABLE;
		  Lift_Moto.Set_Height = 0xffff;
			break;
		
		default:
			break;
	}
}

/*=============================================================================
*  函数名 ：Get_LimitSwitch
*  作   者：hrx
*  创建时间：2022年4月11日 
*  修改时间：
*  输   入：
*           
*  输   出：
*  说   明：获取限位开关
*/
uint8_t Get_LimitSwitch(void)
{
	uint8_t limit_flag = 0;

	if(LIMIT_UP_READ == 0 && Lift_Moto.Height > 60) limit_flag |= 0x01;
	else limit_flag &= 0xfe;
	
	if(LIMIT_DOWN_READ == 0 && Lift_Moto.Height < 30) limit_flag |= 0x02;
	else limit_flag &=0xfd;
	
	return limit_flag;
}

/*=============================================================================
*  函数名 ：Get_LimitSwitch
*  作   者：hrx
*  创建时间：2022年4月11日 
*  修改时间：
*  输   入：
*           
*  输   出：
*  说   明：获取过流状态
*/
uint8_t Get_OverCurFlag(uint8_t _cmd, uint16_t _cur)
{
	static uint8_t overcur_cnt = 0;
	static uint8_t overcur_flag = 0;
	
	switch(_cmd)
	{
		/* moto up */
		case LIFT_UP:
			overcur_flag &= 0xfd;
			if(_cur > 1000)
			{
				overcur_cnt++;
				if(overcur_cnt >= 20)
				{
					overcur_cnt = 0;
					overcur_flag |= 0x01;
				}
			}
			else
			{
			overcur_cnt = 0;
			}
			break;
			
		/* moto down */
		case LIFT_DOWN:
			overcur_flag &= 0xfe;
			if(_cur > 1000)
			{
				overcur_cnt++;
				if(overcur_cnt >= 10)
				{
					overcur_cnt = 0;
					overcur_flag |= 0x02;
				}
			}
			else
			{
				overcur_cnt = 0;
			}
			break;
			
		case LIFT_STOP:
			break;
		
		default:
			break;
	}	
	
	return overcur_flag;
}

/*=============================================================================
*  函数名 ：Get_LiftMoto_Height
*  作   者：hrx
*  创建时间：2022年4月11日 
*  修改时间：
*  输   入：
*           
*  输   出：
*  说   明：计算升降高度
*/
uint16_t Get_LiftMoto_Height(void)
{
	return (Lift_Moto.Lift_Tim * MAX_HEIGHT)/MAX_LIFT_TIM;
}

/*=============================================================================
*  函数名 ：LiftMoto_GoTO_SetHeiht
*  作   者：hrx
*  创建时间：2022年4月11日 
*  修改时间：
*  输   入：
*           
*  输   出：
*  说   明：升降到指定的高度
*/
void LiftMoto_GoTO_SetHeiht(uint16_t *_set_height)
{
	#ifdef LiftMoto_1
	if(*_set_height == MAX_HEIGHT)
	{
		/* 上限位 */
		//if((Lift_Moto.Limit_Switch_Flag & 0x01) != 0 || (Lift_Moto.OverCurrent_Flag & 0x01) != 0){
		if(Lift_Moto.Limit_Switch_Flag == 0x01)
		{
			Lift_Moto.Lift_OK_flag = true;
		}
		else
		{
			Lift_Moto.Cmd = LIFT_UP;
		}
	}
	else if(*_set_height == 0)
	{
		/* 下限位 */
		//if((Lift_Moto.Limit_Switch_Flag & 0x02) != 0 || (Lift_Moto.OverCurrent_Flag & 0x02) != 0 )
		if(Lift_Moto.Limit_Switch_Flag == 0x02)
		{
			Lift_Moto.Lift_OK_flag = true;
		}else{
			Lift_Moto.Cmd = LIFT_DOWN;
		}
	}
	else if(Lift_Moto.Height < *_set_height)
	{	
		if(Lift_Moto.Height >= *_set_height)
		{
			Lift_Moto.Cmd = LIFT_STOP;
			Lift_Moto.Lift_OK_flag = true;
		}
		else
		{
			Lift_Moto.Cmd = LIFT_UP;
			Lift_Moto.Lift_OK_flag = false;
		}
	}
	else
	{
		if(Lift_Moto.Height <= *_set_height)
		{
			Lift_Moto.Cmd = LIFT_STOP;
			Lift_Moto.Lift_OK_flag = true;
		}
		else
		{
			Lift_Moto.Cmd = LIFT_DOWN;
			Lift_Moto.Lift_OK_flag = false;
		}
	}	
	#elif LiftMoto_2
	if(*_set_height == MAX_HEIGHT_3)
	{
		/* 上限位 */
		if(Lift_Moto.Height >= MAX_HEIGHT_3 - 1) 
			Lift_Moto.Lift_OK_flag = true;
		else 
			Lift_Moto.Cmd = LIFT_UP;
	}
	else if(*_set_height == 0)
	{
		/* 下限位 */
		if(Lift_Moto.Height <= 1) 
			Lift_Moto.Lift_OK_flag = true;
		else 
			Lift_Moto.Cmd = LIFT_DOWN;
	}
	else if(Lift_Moto.Height < *_set_height)
	{	
		if(Lift_Moto.Height >= *_set_height)
		{
			Lift_Moto.Cmd = LIFT_STOP;
			Lift_Moto.Lift_OK_flag = true;
		}
		else
		{
			Lift_Moto.Cmd = LIFT_UP;
			Lift_Moto.Lift_OK_flag = false;
		}
	}
	else
	{
		if(Lift_Moto.Height <= *_set_height)
		{
			Lift_Moto.Cmd = LIFT_STOP;
			Lift_Moto.Lift_OK_flag = true;
		}
		else
		{
			Lift_Moto.Cmd = LIFT_DOWN;
			Lift_Moto.Lift_OK_flag = false;
		}
	}	
	#endif
}

/*=============================================================================
*  函数名 ：LiftMoto_Process
*  作   者：hrx
*  创建时间：2022年4月11日 
*  修改时间：
*  输   入：
*           
*  输   出：
*  说   明：升降处理函数
*/
void LiftMoto_Process(void)
{
	#ifdef LiftMoto_1
	Lift_Moto.Limit_Switch_Flag = Get_LimitSwitch();
	Lift_Moto.OverCurrent_Flag = Get_OverCurFlag(Lift_Moto.Cmd, ADC_Filter_Value[4]);
	Lift_Moto.Height = Get_LiftMoto_Height();
	if(Lift_Moto.Set_Height != 0xffff)
	{
		LiftMoto_GoTO_SetHeiht(&Lift_Moto.Set_Height);
	}
	else
	{
		Lift_Moto.Lift_OK_flag = false;
	}
	LiftMoto_Set(Lift_Moto.Cmd);
	#elif LiftMoto_2
	Lift_Moto.Height = GetLiftHeight();
//	printf("Lift_Moto.Lift_OK_flag: %d\r\n", Lift_Moto.Lift_OK_flag);
//	printf("Lift_Moto.Set_Height: %d\r\n", Lift_Moto.Set_Height);
	if(Lift_Moto.Set_Height != 0xffff)
	{
		LiftMoto_GoTO_SetHeiht(&Lift_Moto.Set_Height);
	}
	else
	{
		Lift_Moto.Lift_OK_flag = false;
	}
	LiftMoto_Set(Lift_Moto.Cmd);
	#endif
}

/*=============================================================================
*  函数名 ：LiftMoto_Init
*  作   者：hrx
*  创建时间：2022年4月11日 
*  修改时间：
*  输   入：
*           
*  输   出：
*  说   明：开机初始化升降机
*/
void LiftMoto_Init(void)
{
	static uint8_t cnt = 0;

	if(LIMIT_DOWN_READ == 0)
	{
		LIFTMOTO_DISABLE;
		delay_ms(50);
		LIFTMOTO_UP;
		LIFTMOTO_ENABLE;
		delay_ms(300);
		LIFTMOTO_DISABLE;	
		Lift_Moto.Cmd = LIFT_STOP;
		LiftMoto_Set(Lift_Moto.Cmd);
		Lift_Moto.Set_Height = 0xffff;
		Lift_Moto.Lift_Tim = 0;
		Lift_Moto.Height = 0;

		Lift_Moto.Init_OK_Flag = 1;
		printf("limit bit lift OK\r\n");
	}
	/* 2.8A */
	else if(ADC_Filter_Value[4] > 500)
	{
		cnt++;
		if(cnt > 10)
		{
			cnt = 0;
			LIFTMOTO_DISABLE;
			delay_ms(50);
			LIFTMOTO_UP;
			LIFTMOTO_ENABLE;            
			delay_ms(1000);
			LIFTMOTO_DISABLE;	
			Lift_Moto.Cmd = LIFT_STOP;
			LiftMoto_Set(Lift_Moto.Cmd);
			//printf("qqqqqqqq\r\n");
			Lift_Moto.Set_Height = 0xffff;
			Lift_Moto.Lift_Tim = 0;
			Lift_Moto.Height = 0;	
			Lift_Moto.Init_OK_Flag = 1;
			printf("over cur lift OK\r\n");
		}
	}
	else
	{
		Lift_Moto.Lift_Tim = MAX_LIFT_TIM/2;
		Lift_Moto.Height = MAX_HEIGHT/2;
		Lift_Moto.Cmd = LIFT_DOWN;
		cnt = 0;
	}
	LiftMoto_Set(Lift_Moto.Cmd);
	delay_ms(10);
}
