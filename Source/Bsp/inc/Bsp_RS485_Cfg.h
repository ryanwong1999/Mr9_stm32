/*=============================================================================
*  Copyright (C) ".2016 " All rights reserved.
*
*  文件名："Bsp_USART_Cfg.c"
*  作者：zz  (zhangzhen123good@163.com)
*  创建时间：2016年07月25日 
*  修改时间：
*  说明：
*
==============================================================================*/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __BSP_RS485_CFG_H
#define __BSP_RS485_CFG_H

#ifdef __cplusplus
extern "C" {
#endif 
	 
/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
	 

#define USART_RS485 	UART4

/* RS485_CS*/     
#define RS485_TX_EN_PORT_CLK			RCC_AHB1Periph_GPIOD
#define RS485_TX_EN_PORT					GPIOD
#define RS485_TX_EN_PIN						GPIO_Pin_3

#define RS485_TX_ENABLE						GPIO_ResetBits(RS485_TX_EN_PORT, RS485_TX_EN_PIN);
#define RS485_RX_ENABLE						GPIO_SetBits(RS485_TX_EN_PORT, RS485_TX_EN_PIN);	 
#define RS485_TX_DISABLE					RS485_RX_ENABLE
	 
void RS485_Cfg_Init(uint32_t lBaudrate);
void RS485_SendOneByte(USART_TypeDef* pUSARTx, uint8_t cDat);
void RS485_SendMultibyte(USART_TypeDef* pUSARTx,uint8_t *pBuf, uint16_t lLength);
void RS485_SendString(USART_TypeDef* pUSARTx, uint8_t *pStr);
  
#ifdef __cplusplus
}
#endif

#endif /*__BSP_SYSTICK_CFG_H */


/************************ (C) COPYRIGHT YzRobot  *****END OF FILE****/

