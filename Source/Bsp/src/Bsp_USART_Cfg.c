/*=============================================================================
*  Copyright (C) "2016." All rights reserved.
*
*  文件名："Bsp_USART_Cfg.c"
*  作者：hrx
*  创建时间：2021年12月10日 
*  修改时间：
*  说明：串口初始化
*
==============================================================================*/

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include "Bsp_USART_Cfg.h"
#include "Bsp_GPIO_Cfg.h"
#include "delay.h"
#include "includes.h"


/*=============================================================================
*  函数名 ：USART1_Cfg_Init
*  作   者：
*  创建时间：2022年3月5日 
*  修改时间：
*  输   入：lBaudrate  -> 波特率
*  输   出：
*  说   明：USART1初始化
*/
static void USART1_Cfg_Init(uint32_t lBaudrate)
{
	/* GPIO端口设置 */
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); 			// 使能GPIOA时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);			// 使能USART1时钟
	/* 串口1对应引脚复用映射 */
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1); 	// GPIOA9复用为USART1
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1);	// GPIOA10复用为USART1
	/* USART1端口配置 */
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_9 | GPIO_Pin_10; // GPIOA9与GPIOA10
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AF;            	// 复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	      	// 速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;          	// 推挽复用输出
	GPIO_InitStructure.GPIO_PuPd 	= GPIO_PuPd_UP;            	// 上拉
	GPIO_Init(GPIOA,&GPIO_InitStructure);                   	// 初始化PA9，PA10
	//*USART1 初始化设置 */
	USART_InitStructure.USART_BaudRate 						= lBaudrate;             					// 波特率设置
	USART_InitStructure.USART_WordLength 					= USART_WordLength_8b;						// 字长为8位数据格式
	USART_InitStructure.USART_StopBits 						= USART_StopBits_1;     					// 一个停止位
	USART_InitStructure.USART_Parity 							= USART_Parity_No;        				// 无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	// 无硬件数据流控制
	USART_InitStructure.USART_Mode 								= USART_Mode_Rx | USART_Mode_Tx;	// 收发模式
	USART_Init(USART1, &USART_InitStructure);				// 初始化串口1
	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);	/* 使能接收中断 */
	/*
		USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
		注意: 不要在此处打开发送中断
		发送中断使能在SendUart()函数打开
	*/
	USART_Cmd(USART1, ENABLE);											/* 使能串口 */
	/* CPU的小缺陷：串口配置好，如果直接Send，则第1个字节发送不出去
		如下语句解决第1个字节无法正确发送出去的问题 */
	USART_ClearFlag(USART1, USART_FLAG_TC);					/* 清发送完成标志，Transmission Complete flag */
	/* USART1 NVIC 统一配置 */
}


/*=============================================================================
*  函数名 ：USART2_Cfg_Init
*  作   者：
*  创建时间：2022年3月5日 
*  修改时间：
*  输   入：lBaudrate  -> 波特率
*  输   出：
*  说   明：USART2初始化
*/
static void USART2_Cfg_Init(uint32_t lBaudrate)
{
	/* GPIO端口设置 */
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); 		// 使能GPIOA时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	/* 串口1对应引脚复用映射 */
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2); // GPIOA9复用为USART1
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2); // GPIOA10复用为USART1
	/* USART1端口配置 */
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_2 | GPIO_Pin_3; 	// GPIOA9与GPIOA10
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AF;            	// 复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	      	// 速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;          	// 推挽复用输出
	GPIO_InitStructure.GPIO_PuPd 	= GPIO_PuPd_UP;            	// 上拉
	GPIO_Init(GPIOA, &GPIO_InitStructure);                   	// 初始化PA9，PA10
	/* USART1 初始化设置 */
	USART_InitStructure.USART_BaudRate 						= lBaudrate;                 			// 波特率设置
	USART_InitStructure.USART_WordLength 					= USART_WordLength_8b;     				// 字长为8位数据格式
	USART_InitStructure.USART_StopBits 						= USART_StopBits_1;          			// 一个停止位
	USART_InitStructure.USART_Parity 							= USART_Parity_No;             		// 无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	// 无硬件数据流控制
	USART_InitStructure.USART_Mode 								= USART_Mode_Rx | USART_Mode_Tx;	// 收发模式
	USART_Init(USART2, &USART_InitStructure);				// 初始化串口1
	
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);	/* 使能接收中断 */
	/*
		USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
		注意: 不要在此处打开发送中断
		发送中断使能在SendUart()函数打开
	*/
	USART_Cmd(USART2, ENABLE);											/* 使能串口 */
	/* CPU的小缺陷：串口配置好，如果直接Send，则第1个字节发送不出去
		如下语句解决第1个字节无法正确发送出去的问题 */
	USART_ClearFlag(USART2, USART_FLAG_TC);					/* 清发送完成标志，Transmission Complete flag */
}


/*=============================================================================
*  函数名 ：USART3_Cfg_Init
*  作   者：
*  创建时间：2022年3月5日 
*  修改时间：
*  输   入：lBaudrate  -> 波特率
*  输   出：
*  说   明：USART3初始化
*/
static void USART3_Cfg_Init(uint32_t lBaudrate)
{
	/* GPIO端口设置 */
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);				// 使能GPIOA时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);			// 使能USART1时钟
	/* 串口1对应引脚复用映射 */
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_USART3);	// GPIOA9复用为USART1
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_USART3);	// GPIOA10复用为USART1
	/* USART1端口配置 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;		// GPIOA9与GPIOA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;            		// 复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	      		// 速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;          		// 推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;            		// 上拉
	GPIO_Init(GPIOB,&GPIO_InitStructure);                   		// 初始化PA9，PA10
	/* USART1 初始化设置 */
	USART_InitStructure.USART_BaudRate 						= lBaudrate;                 			// 波特率设置
	USART_InitStructure.USART_WordLength 					= USART_WordLength_8b;     				// 字长为8位数据格式
	USART_InitStructure.USART_StopBits 						= USART_StopBits_1;          			// 一个停止位
	USART_InitStructure.USART_Parity 							= USART_Parity_No;             		// 无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	// 无硬件数据流控制
	USART_InitStructure.USART_Mode 								= USART_Mode_Rx | USART_Mode_Tx;	// 收发模式
	USART_Init(USART3, &USART_InitStructure);				// 初始化串口1
	
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);	/* 使能接收中断 */
	/*
		USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
		注意: 不要在此处打开发送中断
		发送中断使能在SendUart()函数打开
	*/
	USART_Cmd(USART3, ENABLE);											/* 使能串口 */
	/* CPU的小缺陷：串口配置好，如果直接Send，则第1个字节发送不出去
		如下语句解决第1个字节无法正确发送出去的问题 */
	USART_ClearFlag(USART3, USART_FLAG_TC);     		/* 清发送完成标志，Transmission Complete flag */
}


/*=============================================================================
*  函数名 ：USART4_Cfg_Init
*  作   者：
*  创建时间：2022年3月5日 
*  修改时间：
*  输   入：lBaudrate  -> 波特率
*  输   出：
*  说   明：USART4初始化
*/
static void UART4_Cfg_Init(uint32_t lBaudrate)
{
	// GPIO端口设置
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE); 			// 使能GPIOA时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);				// 使能USART1时钟
	// 串口1对应引脚复用映射
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource10, GPIO_AF_UART4);		// GPIOA9复用为USART1
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource11, GPIO_AF_UART4);		// GPIOA10复用为USART1
	// USART1端口配置
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_10 | GPIO_Pin_11;	// GPIOA9与GPIOA10
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AF;            		// 复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	      		// 速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;          		// 推挽复用输出
	GPIO_InitStructure.GPIO_PuPd 	= GPIO_PuPd_UP;            		// 上拉
	GPIO_Init(GPIOC,&GPIO_InitStructure);                   		// 初始化PA9，PA10
	// USART1 初始化设置
	USART_InitStructure.USART_BaudRate 						= lBaudrate;                 			// 波特率设置
	USART_InitStructure.USART_WordLength 					= USART_WordLength_8b;     				// 字长为8位数据格式
	USART_InitStructure.USART_StopBits 						= USART_StopBits_1;          			// 一个停止位
	USART_InitStructure.USART_Parity 							= USART_Parity_No;             		// 无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	// 无硬件数据流控制
	USART_InitStructure.USART_Mode 								= USART_Mode_Rx | USART_Mode_Tx;	// 收发模式
	USART_Init(UART4, &USART_InitStructure);		// 初始化串口1
	
	USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);		/* 使能接收中断 */
	/*
		USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
		注意: 不要在此处打开发送中断
		发送中断使能在SendUart()函数打开
	*/
	USART_Cmd(UART4, ENABLE);		/* 使能串口 */
	/* CPU的小缺陷：串口配置好，如果直接Send，则第1个字节发送不出去
		如下语句解决第1个字节无法正确发送出去的问题 */
	USART_ClearFlag(UART4, USART_FLAG_TC);		/* 清发送完成标志，Transmission Complete flag */
}


/*=============================================================================
*  函数名 ：USART5_Cfg_Init
*  作   者：
*  创建时间：2022年3月5日 
*  修改时间：
*  输   入：lBaudrate  -> 波特率
*  输   出：
*  说   明：USART5初始化
*/
static void UART5_Cfg_Init(uint32_t lBaudrate)
{
	/* GPIO端口设置 */
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE); 		// 使能GPIOA时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE); 		// 使能GPIOA时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE);			// 使能USART1时钟
	/* 串口1对应引脚复用映射 */
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource12, GPIO_AF_UART5);	// GPIOA9复用为USART1
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource2, GPIO_AF_UART5);	// GPIOA10复用为USART1
	/* USART1端口配置 */
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_12; 							// GPIOA9与GPIOA10
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AF;           	// 复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	      	// 速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;          	// 推挽复用输出
	GPIO_InitStructure.GPIO_PuPd 	= GPIO_PuPd_UP;           	// 上拉
	GPIO_Init(GPIOC,&GPIO_InitStructure);                   	// 初始化PA9，PA10
	/* USART1端口配置 */
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_2; 							// GPIOA9与GPIOA10
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AF;            	// 复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	      	// 速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;          	// 推挽复用输出
	GPIO_InitStructure.GPIO_PuPd 	= GPIO_PuPd_UP;            	// 上拉
	GPIO_Init(GPIOD,&GPIO_InitStructure);                   	// 初始化PA9，PA10
	/* USART1 初始化设置 */
	USART_InitStructure.USART_BaudRate 						= lBaudrate;                 			// 波特率设置
	USART_InitStructure.USART_WordLength 					= USART_WordLength_8b;     				// 字长为8位数据格式
	USART_InitStructure.USART_StopBits 						= USART_StopBits_1;          			// 一个停止位
	USART_InitStructure.USART_Parity 							= USART_Parity_No;             		// 无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	// 无硬件数据流控制
	USART_InitStructure.USART_Mode 								= USART_Mode_Rx | USART_Mode_Tx;	// 收发模式
	USART_Init(UART5, &USART_InitStructure);				// 初始化串口1
	
	USART_ITConfig(UART5, USART_IT_RXNE, ENABLE);		/* 使能接收中断 */
	/*
		USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
		注意: 不要在此处打开发送中断
		发送中断使能在SendUart()函数打开
	*/
	USART_Cmd(UART5, ENABLE);												/* 使能串口 */
	/* 
		CPU的小缺陷：串口配置好，如果直接Send，则第1个字节发送不出去
		如下语句解决第1个字节无法正确发送出去的问题 
	*/
	USART_ClearFlag(UART5, USART_FLAG_TC);     			/* 清发送完成标志，Transmission Complete flag */
}


/*=============================================================================
*  函数名 ：USART8_Cfg_Init
*  作   者：
*  创建时间：2022年3月5日 
*  修改时间：
*  输   入：lBaudrate  -> 波特率
*  输   出：
*  说   明：USART8初始化
*/
void USART8_Cfg_Init(uint32_t lBaudrate)
{
	/* GPIO端口设置 */
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE); 		// 使能GPIOE时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART8, ENABLE);			// 使能USART8时钟
	/* 串口1对应引脚复用映射 */
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource0, GPIO_AF_UART8);	// GPIOE0复用为USART8
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource1, GPIO_AF_UART8); 	// GPIOE1复用为USART8
	/* USART1端口配置 */
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_0 | GPIO_Pin_1; 	// GPIOE0与GPIOE1
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AF;            	// 复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	      	// 速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;          	// 推挽复用输出
	GPIO_InitStructure.GPIO_PuPd 	= GPIO_PuPd_UP;            	// 上拉
	GPIO_Init(GPIOE,&GPIO_InitStructure);                   	// 初始化PE0，PE1
	/* USART1 初始化设置 */
	USART_InitStructure.USART_BaudRate 						= lBaudrate;                 			// 波特率设置
	USART_InitStructure.USART_WordLength 					= USART_WordLength_8b;     				// 字长为8位数据格式
	USART_InitStructure.USART_StopBits 						= USART_StopBits_1;          			// 一个停止位
	USART_InitStructure.USART_Parity 							= USART_Parity_No;             		// 无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	// 无硬件数据流控制
	USART_InitStructure.USART_Mode 								= USART_Mode_Rx | USART_Mode_Tx;	// 收发模式
	USART_Init(UART8, &USART_InitStructure);				// 初始化串口1
	
	USART_ITConfig(UART8, USART_IT_RXNE, ENABLE);		/* 使能接收中断 */
	/*
		USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
		注意: 不要在此处打开发送中断
		发送中断使能在SendUart()函数打开
	*/
	USART_Cmd(UART8, ENABLE);												/* 使能串口 */
	/* CPU的小缺陷：串口配置好，如果直接Send，则第1个字节发送不出去
		如下语句解决第1个字节无法正确发送出去的问题 */
	USART_ClearFlag(UART8, USART_FLAG_TC);					/* 清发送完成标志，Transmission Complete flag */
}


/*=============================================================================
*  函数名 ：USARTx_Cfg_Init
*  作   者：
*  创建时间：2022年3月5日
*  修改时间：
*  输   入：pUSARTx   ->串口设置
            lBaudrate  -> 波特率
*  输   出：
*  说   明：USART8初始化
*/
void USARTx_Cfg_Init(USART_TypeDef* pUSARTx, uint32_t iBaudrate)
{
	if(pUSARTx == USART1)
	{
		USART1_Cfg_Init(iBaudrate);
	}	
	else if(pUSARTx == USART2)
	{
		USART2_Cfg_Init(iBaudrate);
	}
	else if(pUSARTx == USART3)
	{
		USART3_Cfg_Init(iBaudrate);
	}
	else if(pUSARTx == UART4)
	{
		UART4_Cfg_Init(iBaudrate);
	}
	else if(pUSARTx == UART5)
	{
		UART5_Cfg_Init(iBaudrate);
	}	
	else if(pUSARTx == UART8)
	{
		USART8_Cfg_Init(iBaudrate);
	}
}



/*=============================================================================
*  函数名 ：USARTx_SendOneByte
*  作   者：
*  创建时间：2022年3月5日 
*  修改时间：
*  输   入：
*  输   出：
*  说   明：串口发送一个字节函数
*/
void USARTx_SendOneByte(USART_TypeDef* pUSARTx, uint8_t cDat)
{
	USART_GetFlagStatus(pUSARTx,USART_FLAG_TC);
	USART_SendData(pUSARTx, (uint8_t) cDat);
	/* Loop until the end of transmission */
	while(USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);	
}


/*=============================================================================
*  函数名 ：USART_SendMultibyte
*  作   者：
*  创建时间：2022年3月5日 
*  修改时间：
*  输   入：
*  输   出：
*  说   明：串口发送多个字节函数
*/
void USARTx_SendMultibyte(USART_TypeDef* pUSARTx, uint8_t *pBuf, uint32_t lLength)
{
	uint8_t i;
	for(i = 0; i < lLength; i++)
	{
		USARTx_SendOneByte(pUSARTx, *pBuf);
		pBuf++;                           // 指针地址加1
	}
}


/*=============================================================================
*  函数名 ：USARTx_SendString
*  作   者：
*  创建时间：2022年3月5日 
*  修改时间：
*  输   入：
*  输   出：
*  说   明：串口发送字符串
*/
void USARTx_SendString(USART_TypeDef* pUSARTx, uint8_t *pStr)
{
	uint8_t i;
  do 
  {
		USARTx_SendOneByte(pUSARTx, *(pStr + i));
		i++;
  } while(*(pStr + i)!='\0');
  /* 等待发送完成 */
  while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TC)==RESET){}	
}




