/*=============================================================================
*  Copyright (C) ".strftime("%Y")." All rights reserved.
*
*  文件名："App_USART.c"
*  作者：hrx
*  创建时间：2022年4月11日 
*  修改时间：
*  说明：串口处理
*
==============================================================================*/

/* Includes ------------------------------------------------------------------*/

#include "includes.h"


Usart_TypeDef  UsartToPC;
Usart_TypeDef	 UsartToEnviro;
Usart_TypeDef  UsartToVoice;
Usart_TypeDef  UsartToTest;
Usart_TypeDef  UsartToDrv;

/*=============================================================================
*  函数名 ：USART1_IRQHandler
*  作   者：hrx
*  创建时间：2022年4月11日 
*  修改时间：
*  输   入：
*           
*  输   出：
*  说   明：串口1 中断函数，用于测试
      接收命令: 55  AA  11  0e  10  00  00  00  00  00  00  00  0D  0A
                 head  adr len cmd voltage current cap tmp sta   ends    
*/

void USART1_IRQHandler(void)  
{
	uint8_t dat;
	OS_ERR err;	
	#ifdef SYSTEM_SUPPORT_OS	 	
	OSIntEnter();    
  #endif
	// 串口数据接收处理
  if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)    //若接收数据寄存器满
  {  	
		USART_ClearITPendingBit(USART1,USART_IT_RXNE);       
		dat = USART_ReceiveData(USART1);           // 先把UART4串口数据取出来暂存
	  //USARTx_SendOneByte(USART1,dat);
		if(UsartToTest.Usart_Rx_OK == false){
			if((UsartToTest.Rx_Sta & 0x02) != 0x02 )    //收到帧头
			{
				if((UsartToTest.Rx_Sta & 0x01) != 0x01)
				{
					if(dat == 0x55)
					{
						UsartToTest.Rx_Len = 0;
						UsartToTest.Rx_Buf[UsartToTest.Rx_Len++] = dat;
						UsartToTest.Rx_Sta |= 0x01;
					}
				}
				else 
				{
					if(dat == 0xAA)
					{
						UsartToTest.Rx_Buf[UsartToTest.Rx_Len++] = dat;
						UsartToTest.Rx_Sta |= 0x02;
					}
					else if(dat == 0x55)
					{
						UsartToTest.Rx_Len = 0;;
						UsartToTest.Rx_Buf[UsartToTest.Rx_Len++] = dat;
						UsartToTest.Rx_Sta |= 0x01;
					}
					else 
					{
						UsartToTest.Rx_Len = 0;
						UsartToTest.Rx_Sta = 0;
						UsartToTest.Usart_Rx_OK = false;
					}
				}
			}
			else 
			{
				UsartToTest.Rx_Buf[UsartToTest.Rx_Len++] = dat;
				if((UsartToTest.Rx_Sta & 0x04) !=  0x04)
				{
					if(UsartToTest.Rx_Len == UsartToTest.Rx_Buf[3] - 1)
					{
						if(dat == 0x0D)
						{
							UsartToTest.Rx_Sta |= 0x04;
						}
						else 
						{
							UsartToTest.Rx_Len = 0;
							UsartToTest.Rx_Sta = 0;
							UsartToTest.Usart_Rx_OK = false;
						}
					}
				}
				else
				{
					if(dat == 0x0A)
					{
						UsartToTest.Rx_Buf[UsartToTest.Rx_Len] = dat;

						UsartToTest.Rx_Len = 0;
						UsartToTest.Rx_Sta = 0;
						UsartToTest.Usart_Rx_OK = true ;         //接收完成
						#ifdef SYSTEM_SUPPORT_OS	 	
						OSSemPost(&UsartToTest_SEM,OS_OPT_POST_1,&err);//发送信号量 
						#endif
					}
					else 
					{
						UsartToTest.Rx_Len = 0;
						UsartToTest.Rx_Sta = 0;
						UsartToTest.Usart_Rx_OK = false;
					}
				}
			}
		}   
	}
	
	#ifdef SYSTEM_SUPPORT_OS	 
	OSIntExit();  											 
  #endif   
}


/*=============================================================================
*  函数名 ：USART2_IRQHandler
*  作   者：hrx
*  创建时间：2022年4月11日 
*  修改时间：
*  输   入：
*           
*  输   出：
*  说   明：串口1 中断函数，噪声传感器获取   
*/

void USART2_IRQHandler(void)                	//串口1中断服务程序
{
	//u8 Res;
	uint8_t dat;
	OS_ERR err;	
	
#if SYSTEM_SUPPORT_OS  //使用UCOS操作系统
	OSIntEnter();    
#endif
	
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
	{
		USART_ClearITPendingBit(USART2,USART_IT_RXNE);
		
		dat =USART_ReceiveData(USART2);//(USART1->DR);	//读取接收到的数据
		//USARTx_SendOneByte(USART1, dat);
		
		if(UsartToVoice.Usart_Rx_OK == false){
			UsartToVoice.Rx_Buf[UsartToVoice.Rx_Len++] = dat;
			if(UsartToVoice.Rx_Len >= 7){
				UsartToVoice.Usart_Rx_OK = true;
				
			#ifdef SYSTEM_SUPPORT_OS
				OSSemPost(&UsartEnviron_SEM,OS_OPT_POST_1,&err);//发送信号量 
			#endif
			}
		}
  } 
	
 #if SYSTEM_SUPPORT_OS  
	OSIntExit();    	//退出中断
#endif
	
} 

/*=============================================================================
*  函数名 ：USART3_IRQHandler
*  作   者：hrx
*  创建时间：2022年4月11日 
*  修改时间：
*  输   入：
*           
*  输   出：
*  说   明：串口1 中断函数，工控机通信
*/
void USART3_IRQHandler(void)  
{
	OS_ERR err;	
	static unsigned char bTemp;
	static unsigned char dat;
	static unsigned char res;
	static unsigned char rfid_flag = 0;
	uint8_t checksum = 0;
	static uint16_t distance = 0;

	#ifdef SYSTEM_SUPPORT_OS	 	
	OSIntEnter();    
  #endif
	// 串口数据接收处理
  if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)    //若接收数据寄存器满
  {  	
		USART_ClearITPendingBit(USART3,USART_IT_RXNE);
		
		dat =USART_ReceiveData(USART3);//(USART1->DR);	//读取接收到的数据
	 // USARTx_SendOneByte(USART1, dat);
		if(UsartToPC.Usart_Rx_OK == false){
			if((UsartToPC.Rx_Sta & 0x02) != 0x02)    //收到帧头
			{
				if((UsartToPC.Rx_Sta&0x01) != 0x01)
				{
					if(dat == 0x55)
					{
						UsartToPC.Rx_Len = 0;
						UsartToPC.Rx_Buf[UsartToPC.Rx_Len++] = dat;
						UsartToPC.Rx_Sta |= 0x01;
					}
				}
				else 
				{
					if(dat == 0xAA)
					{
						UsartToPC.Rx_Buf[UsartToPC.Rx_Len++] = dat;
						UsartToPC.Rx_Sta |= 0x02;
					}
					else if(dat == 0x55)
					{
						UsartToPC.Rx_Len = 0;;
						UsartToPC.Rx_Buf[UsartToPC.Rx_Len++] = dat;
						UsartToPC.Rx_Sta |= 0x01;
					}
					else 
					{
						UsartToPC.Rx_Len = 0;
						UsartToPC.Rx_Sta = 0;
						UsartToPC.Usart_Rx_OK = false;
					}
				}
			}
			else 
			{
				UsartToPC.Rx_Buf[UsartToPC.Rx_Len++] = dat;
				if((UsartToPC.Rx_Sta & 0x04) !=  0x04)
				{
					if(UsartToPC.Rx_Len == UsartToPC.Rx_Buf[3] - 1)
					{
						if(dat == 0x0D)
						{
							//Usart3_Rx_Struct.Rx_Buf[Usart3_Rx_Struct.Rx_Len] = dat;
							UsartToPC.Rx_Sta |= 0x04;
						}
						else 
						{
							UsartToPC.Rx_Len = 0;
							UsartToPC.Rx_Sta = 0;
							UsartToPC.Usart_Rx_OK = false;
						}
					}
				}
				else
				{
					if(dat == 0x0A)
					{
						UsartToPC.Rx_Buf[UsartToPC.Rx_Len] = dat;

						UsartToPC.Rx_Len = 0;
						UsartToPC.Rx_Sta = 0;
						UsartToPC.Usart_Rx_OK = true ;         //接收完成
						
						//USARTx_SendMultibyte(USART1,UsartToPC.Rx_Buf, 19);
					 #ifdef SYSTEM_SUPPORT_OS	 	
							OSSemPost(&UsartRxFromPC_SEM,OS_OPT_POST_1,&err);//发送信号量 
					 #endif
					}
					else 
					{
						UsartToPC.Rx_Len = 0;
						UsartToPC.Rx_Sta = 0;
						UsartToPC.Usart_Rx_OK = false;
					}
				}
			}
		} 
	}

	#ifdef SYSTEM_SUPPORT_OS	 
	OSIntExit();  											 
  #endif
}

/*=============================================================================
*  函数名 ：UART4_IRQHandler
*  作   者：hrx
*  创建时间：2022年4月11日 
*  修改时间：
*  输   入：
*           
*  输   出：
*  说   明：串口1 中断函数，电机驱动器通信
*/
void UART4_IRQHandler(void)   
{
	OS_ERR err;	
	static uint8_t dat;
	#ifdef SYSTEM_SUPPORT_OS	 	
	OSIntEnter();                                     
  #endif
	
	if(USART_GetITStatus(UART4, USART_IT_RXNE) != RESET)    //若接收数据寄存器满
  {  	
		USART_ClearITPendingBit(UART4,USART_IT_RXNE);       
		dat = USART_ReceiveData(UART4);           // 先把UART4串口数据取出来暂存
	  //USARTx_SendOneByte(USART1, dat);
		if(UsartToDrv.Usart_Rx_OK == false){
			UsartToDrv.Rx_Buf[UsartToDrv.Rx_Len++] = dat;
			TIM_SetCounter(TIM7,0);
			TIM_Cmd(TIM7,ENABLE);
		}
	}
	
	#ifdef SYSTEM_SUPPORT_OS	 	
	OSIntExit();    
  #endif
}

/*=============================================================================
*  函数名 ：UART5_IRQHandler
*  作   者：hrx
*  创建时间：2022年4月11日 
*  修改时间：
*  输   入：
*           
*  输   出：
*  说   明：串口 中断函数，获取环境传感器参数
*/
void UART5_IRQHandler(void)    
{
	OS_ERR err;	
	static uint8_t dat;

	#ifdef SYSTEM_SUPPORT_OS	 	
	OSIntEnter();                                     
  #endif
	
	if(USART_GetITStatus(UART5, USART_IT_RXNE) != RESET)    //?????????
  {  	
		USART_ClearITPendingBit(UART5,USART_IT_RXNE);       
		dat = USART_ReceiveData(UART5);           // ??UART4?????????
	
   // USARTx_SendOneByte(USART1, dat);
		UsartToEnviro.Comm_TimeOut = 0;
		if(UsartToEnviro.Usart_Rx_OK == false){

			if((UsartToEnviro.Rx_Sta & 0x01) == 0){
				if(dat == 0x01){              //head
					UsartToEnviro.Rx_Len = 0;
					UsartToEnviro.Rx_Sta = 0;
					UsartToEnviro.Rx_Sta |= 0x01;		
					UsartToEnviro.Rx_Buf[UsartToEnviro.Rx_Len++] = dat;
				}else{
					UsartToEnviro.Rx_Sta = 0;
					UsartToEnviro.Rx_Len = 0;
				}
			}else{
				if((UsartToEnviro.Rx_Sta & 0x02) == 0){
					if(dat == 0x03){

				   	UsartToEnviro.Rx_Sta |= 0x02;
					 
					 	UsartToEnviro.Rx_Buf[UsartToEnviro.Rx_Len++] = dat;
					
					}else{
						UsartToEnviro.Rx_Sta = 0;
						UsartToEnviro.Rx_Len = 0;	
					}
				}else{
				UsartToEnviro.Rx_Buf[UsartToEnviro.Rx_Len++] = dat;
					if(UsartToEnviro.Rx_Len >= UsartToEnviro.Rx_Buf[2] + 5){
						UsartToEnviro.Usart_Rx_OK = true;
						#ifdef SYSTEM_SUPPORT_OS					
						OSSemPost(&UsartEnviron_SEM,OS_OPT_POST_1,&err);//发送信号量 
						#endif
					}
				}
			}		
		}
	
	}
	
	#ifdef SYSTEM_SUPPORT_OS	 	
	OSIntExit();    
  #endif
}

