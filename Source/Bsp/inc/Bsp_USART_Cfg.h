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
#ifndef __BSP_USART_CFG_H
#define __BSP_USART_CFG_H

#ifdef __cplusplus
extern "C" {
#endif 

	 
/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
	 

void USART8_Cfg_Init(uint32_t lBaudrate);	 
void USARTx_Cfg_Init(USART_TypeDef* pUSARTx, uint32_t iBaudrate);
void USARTx_SendOneByte(USART_TypeDef* pUSARTx, uint8_t cDat);
void USARTx_SendMultibyte(USART_TypeDef* pUSARTx, uint8_t *pBuf, uint32_t lLength);	 
void USARTx_SendString(USART_TypeDef* pUSARTx, uint8_t *pStr);	 

#ifdef __cplusplus
}
#endif

#endif /*__BSP_SYSTICK_CFG_H */


/************************ (C) COPYRIGHT YzRobot  *****END OF FILE****/

