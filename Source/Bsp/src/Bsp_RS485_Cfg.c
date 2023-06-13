/*=============================================================================
*  Copyright (C) ".strftime("%Y")." All rights reserved.
*
*  文件名："Bsp_RS485_Cfg.c"
*  作者：hrx
*  创建时间：2022年3月5日 
*  修改时间：
*  说明：RS485通信初始化
*
==============================================================================*/

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include "Bsp_RS485_Cfg.h"
#include "delay.h"
//#include "Bsp_SysTick_Cfg.h"


/*=============================================================================
*  函数名 ：RS485_Cfg_Init
*  作   者：hrx
*  创建时间：2022年3月5日 
*  修改时间：
*  输   入：lBaudrate  -> 波特率
*          
*  输   出：
*  说   明：RS485 IO初始化
*/
void RS485_Cfg_Init(uint32_t lBaudrate)
{
	/* GPIO端口设置 */
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RS485_TX_EN_PORT_CLK, ENABLE); 

  GPIO_InitStructure.GPIO_Pin 	=  RS485_TX_EN_PIN;
  GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_OUT;		    			// 输出
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;        			// 推挽输出
	GPIO_InitStructure.GPIO_PuPd 	= GPIO_PuPd_NOPULL;						// 不上拉 ,也不下拉
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	 
  GPIO_Init(RS485_TX_EN_PORT, &GPIO_InitStructure);	

	RS485_RX_ENABLE;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE); 			// 使能GPIOA时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);				// 使能USART1时钟
	/* USART1端口配置 */
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_10 | GPIO_Pin_11;	// GPIOA9与GPIOA10
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AF;         			// 复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	    			// 速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;        			// 推挽复用输出
	GPIO_InitStructure.GPIO_PuPd 	= GPIO_PuPd_UP;         			// 上拉
	GPIO_Init(GPIOC,&GPIO_InitStructure);                 			// 初始化PA9，PA10
	/* 串口1对应引脚复用映射 */
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource10, GPIO_AF_UART4);		// GPIOA9复用为USART1
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource11, GPIO_AF_UART4);		// GPIOA10复用为USART1
	/* USART1 初始化设置 */
	USART_InitStructure.USART_BaudRate 						= lBaudrate;                 			// 波特率设置
	USART_InitStructure.USART_WordLength 					= USART_WordLength_8b;     				// 字长为8位数据格式
	USART_InitStructure.USART_StopBits 						= USART_StopBits_1;          			// 一个停止位
	USART_InitStructure.USART_Parity 							= USART_Parity_No;             		// 无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	// 无硬件数据流控制
	USART_InitStructure.USART_Mode 								= USART_Mode_Rx | USART_Mode_Tx;	// 收发模式
	USART_Init(USART_RS485, &USART_InitStructure);                       						// 初始化串口1
	
	USART_ITConfig(USART_RS485, USART_IT_RXNE, ENABLE);		/* 使能接收中断 */
	/*
		USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
		注意: 不要在此处打开发送中断
		发送中断使能在SendUart()函数打开
	*/
	USART_Cmd(USART_RS485, ENABLE);												/* 使能串口 */
	/* CPU的小缺陷：串口配置好，如果直接Send，则第1个字节发送不出去
		如下语句解决第1个字节无法正确发送出去的问题 */
	USART_ClearFlag(USART_RS485, USART_FLAG_TC);     			/* 清发送完成标志，Transmission Complete flag */
}


/*=============================================================================
*  函数名 ：RS485_SendOneByte
*  作   者：hrx
*  创建时间：2022年3月5日 
*  修改时间：
*  输   入：pUSARTx  -> 串口选择
            cDat   -> 发送的数据
*          
*  输   出：
*  说   明：串口发送一个字节
*/
void RS485_SendOneByte(USART_TypeDef* pUSARTx, uint8_t cDat)
{
	/* 发送使能 */
	// RS485_TX_ENABLE;
	USART_GetFlagStatus(pUSARTx, USART_FLAG_TC);
	USART_SendData(pUSARTx, (uint8_t) cDat);
	/* Loop until the end of transmission */
	while(USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);	
	// RS485_SendOneByte(pUSARTx, cDat);
	/* 关闭发送使能，等待接收 */
	// RS485_RX_ENABLE;
}


/*=============================================================================
*  函数名 ：RS485_SendString
*  作   者：hrx
*  创建时间：2022年3月5日 
*  修改时间：
*  输   入：pUSARTx  -> 串口选择
            cDat   -> 发送的字符串
*          
*  输   出：
*  说   明：串口发送字符串
*/
void RS485_SendString(USART_TypeDef* pUSARTx, uint8_t *pStr)
{
	uint8_t i;
	RS485_TX_ENABLE;		// 485发送使能
	delay_us(100);
  do 
  {
		RS485_SendOneByte(pUSARTx, *(pStr + i));
		i++;
  } 
	while(*(pStr + i)!='\0');
  /* 等待发送完成 */
  while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TC)==RESET){}	
	delay_us(100);
	RS485_RX_ENABLE;		// 485发送禁止，进入接收模式	
}

/*=============================================================================
*  函数名 ：RS485_SendMultibyte
*  作   者：hrx
*  创建时间：2022年3月5日 
*  修改时间：
*  输   入：pUSARTx  -> 串口选择
            pBuf   -> 发送的字符数组
           lLength -> 数组长度
*          
*  输   出：
*  说   明：发送固定长度的字符数组
*/
void RS485_SendMultibyte(USART_TypeDef* pUSARTx,uint8_t *pBuf, uint16_t lLength)
{
	uint16_t i;

	RS485_TX_ENABLE;		// 485发送使能
	delay_ms(1);
	USART_ClearFlag(pUSARTx, USART_FLAG_TC);   // 清发送外城标志，Transmission Complete flag 

	for(i=0;i<lLength;i++)
	{
		RS485_SendOneByte(pUSARTx, pBuf[i] );
	}
  while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TC) == RESET);	// 注意此句与RS232不同
	RS485_RX_ENABLE; 		// 485发送禁止，进入接收模式	
}


