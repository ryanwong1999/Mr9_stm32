/*=============================================================================
*  Copyright (C) ".strftime("%Y")." All rights reserved.
*
*  文件名："App_Exti.c"
*  作者：hrx
*  创建时间：2022年4月11日 
*  修改时间：
*  说明：外部中断处理
*
==============================================================================*/

/* Includes ------------------------------------------------------------------*/
#include "includes.h"


/*第一路红外*/
uint8_t  CH1_IrDa_STA = 0;    
uint16_t CH1_IrDa_VAL1 = 0;		// 通道1 上升沿计数值
uint16_t CH1_IrDa_VAL2 = 0;		// 通道1 下降沿计数值
uint8_t  CH1_IrDa_Data = 0;		// 通道1 接收到的数据

/*第三路红外*/
uint8_t  CH3_IrDa_STA = 0;    
uint16_t CH3_IrDa_VAL1 = 0;		// 通道3 上升沿计数值
uint16_t CH3_IrDa_VAL2 = 0;		// 通道3 下降沿计数值
uint8_t  CH3_IrDa_Data = 0;		// 通道3 接收到的数据

uint16_t BaseTimeCount = 0;		// 10us 时间基准计数 

/*=============================================================================
*  函数名 ：IR_Code_Delay10us
*  作   者：hrx
*  创建时间：2022年4月11日 
*  修改时间：
*  输   入： 
*           
*  输   出：
*  说   明：外部中断所需要的计数器
*/
void IR_Code_Delay10us(void)
{
	BaseTimeCount ++;
	if(BaseTimeCount >= 0xffff)
		BaseTimeCount = 0 ;
}


/*=============================================================================
*  函数名 ：EXTI0_IRQHandler
*  作   者：hrx
*  创建时间：2022年4月11日 
*  修改时间：
*  输   入： 
*           
*  输   出：
*  说   明：外部中断, 接收自动充电器的编码
*/
void  EXTI0_IRQHandler(void)
{
	OS_ERR err;	
	uint16_t tmp ;
	#ifdef SYSTEM_SUPPORT_OS	 	
	OSIntEnter();    
  #endif

	//Line6产生中断
	if(EXTI_GetITStatus(EXTI_LINE_IRBL) != RESET)   	
	{
		//没有捕获到引导码	0100 0000
		if((CH1_IrDa_STA & 0x40) != 0x40)            	
		{
			//没有捕获到上升沿	0010 0000
			if((CH1_IrDa_STA & 0x20) != 0x20)          	
			{
				//高电平
				if(GPIO_ReadInputDataBit(IRCODE_PORT, IRCODE_BL_PIN) == SET)
				{
					CH1_IrDa_VAL1 = BaseTimeCount;
					//标记捕获到上升沿	0010 0000	
				  CH1_IrDa_STA |= 0x20;               						
				}
			}
			else
			{
				//低电平 下降沿了
				if(GPIO_ReadInputDataBit(IRCODE_PORT,IRCODE_BL_PIN) == RESET)
				{  
					CH1_IrDa_VAL2 = BaseTimeCount;
					//清除捕获到上升沿标记，接收下一个	1101 1111
				  CH1_IrDa_STA &= 0xDF;               			
					//有溢出
					if(CH1_IrDa_VAL2 < CH1_IrDa_VAL1)
					{
						tmp = 65535 + CH1_IrDa_VAL2 - CH1_IrDa_VAL1;
					}
					else
					{
						tmp = CH1_IrDa_VAL2 - CH1_IrDa_VAL1;
					}
					//450*10us=4.5ms  4ms  4.5ms    引导码
					if(tmp >= 370 && tmp < 550)
					{
						//标记捕获到引导码	0100 0000
						CH1_IrDa_STA |= 0x40;       			
						CH1_IrDa_Data = 0;
					}
				}
			}
		}
		//接收到引导码
		else
		{
			//没有捕获到上升沿	0010 0000
			if((CH1_IrDa_STA & 0x20) != 0x20)         		
			{
				//高电平
				if(GPIO_ReadInputDataBit(IRCODE_PORT,IRCODE_BL_PIN) == SET)
				{
					CH1_IrDa_VAL1 = BaseTimeCount;
					//标记捕获到上升沿
					CH1_IrDa_STA |= 0x20;
				}
			}
			else
			{
				//低电平 下降沿了
				if(GPIO_ReadInputDataBit(IRCODE_PORT,IRCODE_BL_PIN) == RESET)
				{  
					CH1_IrDa_VAL2 = BaseTimeCount;
					//清除捕获到上升沿标记，接收下一个	1101 1111
					CH1_IrDa_STA &= 0xDF;               			
					//有溢出
					if(CH1_IrDa_VAL2 < CH1_IrDa_VAL1) tmp = 65535 + CH1_IrDa_VAL2 - CH1_IrDa_VAL1;
					else tmp = CH1_IrDa_VAL2 - CH1_IrDa_VAL1;
					//56*10us=560us 
					if(tmp >= 20 && tmp < 90)
					{               
						CH1_IrDa_Data |= 0;	
					}
					//169*10us=1690us
					else if(tmp >= 100 && tmp < 210)
					{       	
						CH1_IrDa_Data |= 1;
					}
					//225*10us=2250us 连发  未处理
					else if(tmp >= 220 && tmp < 270)
					{
						
					}
					else if(tmp >= 300)
					{
						//清除状态寄存器，重新接收	
						CH1_IrDa_STA = 0;
					}
					//0001 1111
					if((CH1_IrDa_STA & 0x1F) >= 6)
					{
						CH1_IrDa_STA = 0;
						if(CH1_IrDa_Data == TXDAT_LEFT)
						{
							AutoCharge.CH1_IrDa_Dat[0] = TXDAT_LEFT;	
						}	
						else if(CH1_IrDa_Data == TXDAT_MIDL)
						{
							AutoCharge.CH1_IrDa_Dat[1] = TXDAT_MIDL;	
						}	
						else if(CH1_IrDa_Data == TXDAT_RIGHT)
						{
							AutoCharge.CH1_IrDa_Dat[2] = TXDAT_RIGHT;
						}
						else if(CH1_IrDa_Data == TXDAT_MIDR)
						{
							AutoCharge.CH1_IrDa_Dat[3] = TXDAT_MIDR;	
						}
					}
					else
					{
						CH1_IrDa_STA ++ ;
						CH1_IrDa_Data <<= 1;
					}
				}
			}
		}
		//清除Lin6中断标志位
		EXTI_ClearITPendingBit(EXTI_LINE_IRBL);
	}	
	#ifdef SYSTEM_SUPPORT_OS	 
	OSIntExit();  											 
  #endif
}

/*=============================================================================
*  函数名 ：EXTI1_IRQHandler
*  作   者：hrx
*  创建时间：2022年4月11日 
*  修改时间：
*  输   入： 
*           
*  输   出：
*  说   明：外部中断, 接收自动充电器的编码
*/
void EXTI1_IRQHandler(void)
{
	OS_ERR err;	
  uint16_t tmp;
	#ifdef SYSTEM_SUPPORT_OS	 	
	OSIntEnter();    
  #endif
	
	//Line6产生中断 EXTI_LINE_IRBR :EXTI_Line10
	if(EXTI_GetITStatus(EXTI_LINE_IRBR) != RESET)   	
	{
		//没有捕获到引导码
		if((CH3_IrDa_STA & 0x40) != 0x40)
		{
			//没有捕获到上升沿    	
			if((CH3_IrDa_STA & 0x20) != 0x20)
			{          	
				//高电平
				if(GPIO_ReadInputDataBit(IRCODE_PORT, IRCODE_BR_PIN) == SET)
				{  
					CH3_IrDa_VAL1 = BaseTimeCount;
					//标记捕获到上升沿
				  CH3_IrDa_STA |= 0x20;
				}
			}
			else
			{
				//低电平 下降沿了
				if(GPIO_ReadInputDataBit(IRCODE_PORT, IRCODE_BR_PIN) == RESET)
				{
					CH3_IrDa_VAL2 = BaseTimeCount;
					//清除捕获到上升沿标记，接收下一个
				  CH3_IrDa_STA &= 0xDF;
					//有溢出
					if(CH3_IrDa_VAL2 < CH3_IrDa_VAL1) tmp = 65535 + CH3_IrDa_VAL2 - CH3_IrDa_VAL1;
					else tmp = CH3_IrDa_VAL2 - CH3_IrDa_VAL1;
					
					if(tmp >= 370 && tmp < 550)
					{
						//标记捕获到引导码
						CH3_IrDa_STA |= 0x40 ;            			
						CH3_IrDa_Data = 0;
					}
				}
			}
		}
		//接收到引导码
		else
		{
			//没有捕获到上升沿
			if((CH3_IrDa_STA & 0x20) != 0x20)
			{
				//高电平
				if(GPIO_ReadInputDataBit(IRCODE_PORT, IRCODE_BR_PIN) == SET)
				{
					CH3_IrDa_VAL1 = BaseTimeCount;
					//标记捕获到上升沿
					CH3_IrDa_STA |= 0x20;
				}
			}
			else
			{
				//低电平 下降沿了
				if(GPIO_ReadInputDataBit(IRCODE_PORT,IRCODE_BR_PIN) == RESET)
				{
					CH3_IrDa_VAL2 = BaseTimeCount;
					//清除捕获到上升沿标记，接收下一个
					CH3_IrDa_STA &= 0xDF;               			
					//有溢出
					if(CH3_IrDa_VAL2 < CH3_IrDa_VAL1) tmp = 65535 + CH3_IrDa_VAL2 - CH3_IrDa_VAL1;
					else tmp = CH3_IrDa_VAL2 - CH3_IrDa_VAL1;
					//56*10us=560us
					if(tmp >= 20 && tmp < 100)
					{               
						CH3_IrDa_Data |= 0;
					}
					//169*10us=1690us
					else if(tmp >= 100 && tmp < 220)
					{       	
						CH3_IrDa_Data |= 1;
					}
					//225*10us=2250us 连发  未处理
					else if(tmp >= 230 && tmp < 270)
					{	
					}
					else if(tmp >= 300)
					{
						//清除状态寄存器，重新接收	
						CH3_IrDa_STA = 0;
					}
					if((CH3_IrDa_STA & 0x1F) >= 6)
					{
						//引导头清除，下一次接收
						CH3_IrDa_STA = 0;
						if(CH3_IrDa_Data == TXDAT_LEFT)
						{
							AutoCharge.CH3_IrDa_Dat[0] = TXDAT_LEFT;	
						}
						else if(CH3_IrDa_Data == TXDAT_MIDL)
						{
							AutoCharge.CH3_IrDa_Dat[1] = TXDAT_MIDL;	
						}
						else if(CH3_IrDa_Data == TXDAT_RIGHT)
						{
							AutoCharge.CH3_IrDa_Dat[2] = TXDAT_RIGHT;	
						}
						else if(CH3_IrDa_Data == TXDAT_MIDR)
						{
							AutoCharge.CH3_IrDa_Dat[3] = TXDAT_MIDR;
						}
					}
					else
					{
						CH3_IrDa_STA ++ ;
						CH3_IrDa_Data <<= 1;
					}
				}
			}
		}
		//清除Lin6中断标志位 EXTI_LINE_IRBR :EXTI_Line10
		EXTI_ClearITPendingBit(EXTI_LINE_IRBR);
	}
	#ifdef SYSTEM_SUPPORT_OS	 
	OSIntExit();  											 
  #endif
}

/*=============================================================================
*  函数名 ：EXTI2_IRQHandler
*  作   者：hrx
*  创建时间：2022年4月11日 
*  修改时间：
*  输   入： 
*           
*  输   出：
*  说   明：外部中断,
*/
void EXTI2_IRQHandler(void)
{
	OS_ERR err;	

	#ifdef SYSTEM_SUPPORT_OS	 	
	OSIntEnter();    
  #endif
	
	//Line12产生中断
	if(EXTI_GetITStatus(EXTI_Line2) != RESET)   
	{
		//清除Lin6中断标志位
		EXTI_ClearITPendingBit(EXTI_Line2);       
	}
	
	#ifdef SYSTEM_SUPPORT_OS	 
	OSIntExit();  											 
  #endif
}

/*=============================================================================
*  函数名 ：EXTI4_IRQHandler
*  作   者：hrx
*  创建时间：2022年4月11日 
*  修改时间：
*  输   入： 
*           
*  输   出：
*  说   明：外部中断,
*/
void EXTI4_IRQHandler(void)
{
  OS_ERR err;	
	uint16_t tmp;
  static uint16_t ultra3_val1 = 0;
	static uint16_t ultra3_val2 = 0;
	#ifdef SYSTEM_SUPPORT_OS	 	
	OSIntEnter();    
  #endif
	
	//Line12产生中断
	if(EXTI_GetITStatus(EXTI_Line4) != RESET)   
	{
		//清除Lin6中断标志位
		EXTI_ClearITPendingBit(EXTI_Line4);
	}
	
	#ifdef SYSTEM_SUPPORT_OS	 
	OSIntExit();  											 
  #endif
}

/*=============================================================================
*  函数名 ：EXTI9_5_IRQHandler
*  作   者：hrx
*  创建时间：2022年4月11日 
*  修改时间：
*  输   入： 
*           
*  输   出：
*  说   明：外部中断,获取超声Ultra1，Ultra2，Ultra3的距离
*/
void EXTI9_5_IRQHandler(void)
{
	OS_ERR err;	
	uint16_t tmp;
	
	static uint16_t ultra1_val1 = 0;
	static uint16_t ultra1_val2 = 0;
	static uint16_t ultra2_val1 = 0;
	static uint16_t ultra2_val2 = 0;
	static uint16_t ultra3_val1 = 0;
	static uint16_t ultra3_val2 = 0;
	
	#ifdef SYSTEM_SUPPORT_OS	 	
	OSIntEnter();    
  #endif
	
	//Line12产生中断
	if(EXTI_GetITStatus(EXTI_Line5) != RESET)   
	{
		//已经使能超声波数据捕获
		if((Ultra2.Uswave_Sta & 0x20) == 0x20)
		{
			//数据没有接收完成
			if((Ultra2.Uswave_Sta & 0x80) != 0x80)
			{
				//没有捕获到上升沿
				if((Ultra2.Uswave_Sta & 0x40) != 0x40)
				{
					//高电平
					if(ECHO_READ_2 == SET)
					{
						ultra2_val1 =BaseTimeCount;
						//标记捕获到上升沿
						Ultra2.Uswave_Sta |= 0x40;
					}
				}
				//捕获到上升沿了，判断捕获下降沿
				else
				{
					//低电平 下降沿了
					if(ECHO_READ_2 == RESET)
					{
						ultra2_val2 = BaseTimeCount;
						//计数有溢出
						if(ultra2_val2 < ultra2_val1)
						{
							tmp = 65535 + ultra2_val2 - ultra2_val1;
						}
						else
						{
							tmp = ultra2_val2 - ultra2_val1;
						}
						//障碍物距离  mm
						Ultra2.Distance = tmp*50/29;
						if(Ultra2.Distance > 2000) Ultra2.Distance = 2000;
						//清除使能接收数据
						Ultra2.Uswave_Sta &= 0x0F;
						//数据接收完成
						Ultra2.Uswave_Sta |=0x80;
					  Ultra2.Out_time = 0;
					}
				}
			}
		}
		//清除Lin6中断标志位
		EXTI_ClearITPendingBit(EXTI_Line5);
	}
	
	//Line12产生中断
	if(EXTI_GetITStatus(EXTI_Line7) != RESET)
	{
		//已经使能超声波数据捕获
		if((Ultra1.Uswave_Sta & 0x20) == 0x20)
		{
			//数据没有接收完成
			if((Ultra1.Uswave_Sta & 0x80) != 0x80)
			{
				//没有捕获到上升沿
				if((Ultra1.Uswave_Sta & 0x40) != 0x40)
				{
					//高电平
					if(ECHO_READ_1 == SET)
					{
						ultra1_val1 =BaseTimeCount;
						//标记捕获到上升沿
						Ultra1.Uswave_Sta |= 0x40;	
					}
				}
				//捕获到上升沿了，判断捕获下降沿
				else
				{
					//低电平 下降沿了
					if(ECHO_READ_1 == RESET)
					{
						ultra1_val2 = BaseTimeCount;
						//计数有溢出
						if(ultra1_val2 < ultra1_val1)
						{
							tmp= 65535 + ultra1_val2 - ultra1_val1;
						}
						else
						{
							tmp= ultra1_val2 - ultra1_val1;
						}
						//障碍物距离  mm
						Ultra1.Distance = tmp*50/29;
						if(Ultra1.Distance>2000) Ultra1.Distance = 2000;
						//清除使能接收数据
						Ultra1.Uswave_Sta &= 0x0F;
						//数据接收完成
						Ultra1.Uswave_Sta |=0x80;
					  Ultra1.Out_time = 0;
					}
				}
			}
		}
		//清除Lin6中断标志位
		EXTI_ClearITPendingBit(EXTI_Line7);
	}
	
	//Line12产生中断
	if(EXTI_GetITStatus(EXTI_Line9) != RESET)
	{
	 //已经使能超声波数据捕获
   if((Ultra3.Uswave_Sta & 0x20) == 0x20)
		{
			//数据没有接收完成
			if((Ultra3.Uswave_Sta & 0x80) != 0x80)
			{
				//没有捕获到上升沿
				if((Ultra3.Uswave_Sta & 0x40) != 0x40)
				{
					//高电平
					if(ECHO_READ_3 == SET)
					{
						ultra3_val1 = BaseTimeCount;
						//标记捕获到上升沿	
						Ultra3.Uswave_Sta |= 0x40;
					}
				}
				//捕获到上升沿了，判断捕获下降沿
				else
				{
					//低电平 下降沿了
					if(ECHO_READ_3 == RESET)
					{
						ultra3_val2 = BaseTimeCount;
						//计数有溢出
						if(ultra3_val2 < ultra3_val1) tmp = 65535 + ultra3_val2 - ultra3_val1;
						else tmp = ultra3_val2 - ultra3_val1;
						//障碍物距离  mm
						Ultra3.Distance = tmp*50/29;
						//printf("Ultra3.Distance = %d\r\n",Ultra3.Distance);
						if(Ultra3.Distance > 2000) Ultra3.Distance = 2000;
						//清除使能接收数据
						Ultra3.Uswave_Sta &= 0x0F;
						//数据接收完成
						Ultra3.Uswave_Sta |=0x80;
					  Ultra3.Out_time = 0;
					}
				}
			}
		}
		//清除Lin6中断标志位
		EXTI_ClearITPendingBit(EXTI_Line9);
	}
	#ifdef SYSTEM_SUPPORT_OS	 
	OSIntExit();  											 
  #endif
}


/*=============================================================================
*  函数名 ：EXTI15_10_IRQHandler
*  作   者：hrx
*  创建时间：2022年4月11日 
*  修改时间：
*  输   入： 
*           
*  输   出：
*  说   明：外部中断,获取超声Ultra4的距离
*/
void EXTI15_10_IRQHandler(void)
{
	OS_ERR err;	
	uint16_t tmp;
	
	static uint16_t ultra4_val1 = 0;
	static uint16_t ultra4_val2 = 0;
	static uint16_t ultra5_val1 = 0;
	static uint16_t ultra5_val2 = 0;
	static uint16_t ultra6_val1 = 0;
	static uint16_t ultra6_val2 = 0;
	
	#ifdef SYSTEM_SUPPORT_OS	 	
	OSIntEnter();    
  #endif
	//Line12产生中断
  if(EXTI_GetITStatus(EXTI_Line11) != RESET)
	{
		//已经使能超声波数据捕获
		if((Ultra4.Uswave_Sta &0x20) == 0x20)
		{
			//数据没有接收完成
			if((Ultra4.Uswave_Sta &0x80) != 0x80)
			{
				//没有捕获到上升沿
				if((Ultra4.Uswave_Sta &0x40) != 0x40)
				{
					//高电平
					if(ECHO_READ_4 == SET)
					{
						ultra4_val1 = BaseTimeCount;
						//标记捕获到上升沿	
						Ultra4.Uswave_Sta |= 0x40;
					}
				}
				//捕获到上升沿了，判断捕获下降沿
				else
				{
					//低电平 下降沿了
					if(ECHO_READ_4 == RESET)
					{
						ultra4_val2 = BaseTimeCount;
						//计数有溢出
						if(ultra4_val2 < ultra4_val1) tmp = 65535 + ultra4_val2 - ultra4_val1;
						else tmp = ultra4_val2 - ultra4_val1;
						
						//障碍物距离  mm
						Ultra4.Distance = tmp*50/29;
						if(Ultra4.Distance>2000) Ultra4.Distance = 2000;
						//清除使能接收数据
						Ultra4.Uswave_Sta &= 0x0F;
						//数据接收完成
						Ultra4.Uswave_Sta |=0x80;
					  Ultra4.Out_time = 0;
					}
				}
			}
		}
		//清除Lin6中断标志位
		EXTI_ClearITPendingBit(EXTI_Line11);
	}

	#ifdef SYSTEM_SUPPORT_OS	 
	OSIntExit();  											 
  #endif
}

/* CopyRight : Copyright(c) 1998 - 2015 YZRobot. All Rights Reserved */





