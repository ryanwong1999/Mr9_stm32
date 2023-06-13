/*=============================================================================
*  Copyright (C) ".2016 " All rights reserved.
*
*  文件名："App_LiftMoto.h"
*  作者：
*  创建时间：2018年07月05日 
*  修改时间：
*  说明：
*
==============================================================================*/


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __APP_ENVIRONMENTAL_H
#define __APP_ENVIRONMENTAL_H

#ifdef __cplusplus
 extern "C" {
#endif 

/* Includes ------------------------------------------------------------------*/
#include "stdbool.h"
#include "stm32f4xx.h"
#include "App_USART.h"


#define USART_ENVIRON UART5
#define USART_VOICE   USART2 

typedef struct
{	
	uint16_t CO2;
	uint16_t VOC;
	uint16_t Tempera;
	uint16_t Humi;
	uint16_t CH2O;
	uint16_t PM2_5;
	uint16_t PM10;
	uint16_t PM1_0;
	uint16_t state;
	uint16_t voice;
}Environmental_Typedef;

extern Environmental_Typedef Environ;

void Send_Environ_Query(void);
void Send_Voice_Query(void);
uint8_t CRC_Sum(uint8_t *buf,uint8_t num);
unsigned int CRC_Compute(unsigned char *arr_buff, unsigned char len);
void Environmental_Process(void);
void Voice_Process(void);
void Send_TempHumMess(uint8_t index,uint8_t addr, Environmental_Typedef *_environ);
void Send_EnvironMess(uint8_t index,uint8_t addr, Environmental_Typedef *_environ);
unsigned int CRC_Compute(unsigned char *arr_buff, unsigned char len);

#ifdef __cplusplus
}
#endif

#endif 
	 
