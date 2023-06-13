/*=============================================================================
*  Copyright (C) ".2016 " All rights reserved.
*
*  �ļ�����"Bsp_USART_Cfg.c"
*  ���ߣ�zz  (zhangzhen123good@163.com)
*  ����ʱ�䣺2016��07��25�� 
*  �޸�ʱ�䣺
*  ˵����
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

