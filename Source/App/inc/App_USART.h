/*****************************************************************
* CopyRight : Copyright(c) 1998 - 2015 YZRobot. All Rights Reserved
* FileName  : Bsp_USARTCfg.h
* Author    : hrx
* Date      : 2015年8月21日
*****************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __APP_USART_H
#define __APP_USART_H

#ifdef __cplusplus
extern "C" {
#endif 
  
/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include "stdbool.h"


//#define SUPPORT_IR 				1
//#define SUPPORT_RFID 			1
#define SUPPORT_ESP8266 	1
	 
#define RX_LEN_MAX  			24
#define TX_LEN_MAX  			24
	 
#define USART_PC  USART3
	 
typedef struct 
{
	uint8_t Rx_Sta;    					// 接收状态
  uint8_t Rx_Len;     				// 接收数据长度
	uint16_t Comm_TimeOut;
	uint8_t Disconnect_flag;
	bool Usart_Rx_OK;    				// 接收完成
	uint8_t Rx_Buf[RX_LEN_MAX];	// 接收数组
}Usart_TypeDef;

extern Usart_TypeDef UsartToPC;
extern Usart_TypeDef UsartToEnviro;
extern Usart_TypeDef UsartToVoice;
extern Usart_TypeDef UsartToTest;
extern Usart_TypeDef UsartToDrv;

#ifdef __cplusplus
}
#endif
  
#endif


/* CopyRight : Copyright(c) 1998 - 2015 YZRobot. All Rights Reserved */


