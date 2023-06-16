/*=============================================================================
*  Copyright (C) ".strftime("%Y")." All rights reserved.
*
*  ÎÄ¼şÃû£º"Bsp_CAP_Cfg.c"
*  ×÷Õß£ºhrx
*  ´´½¨Ê±¼ä£º2022Äê3ÔÂ5ÈÕ 
*  ĞŞ¸ÄÊ±¼ä£º
*  ËµÃ÷£ºÊäÈë²¶»ñ³õÊ¼»¯
*
==============================================================================*/

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include "includes.h"

#define TIM_PERIOD 						65535

#define ENCODER_TIM_PSC  			0				/* ¼ÆÊıÆ÷·ÖÆµ */
#define ENCODER_TIM_PERIOD  	65535		/* ¼ÆÊıÆ÷×î´óÖµ */
#define CNT_INIT 							1000		/* ¼ÆÊıÆ÷³õÖµ */

/*=============================================================================
*  º¯ÊıÃû £ºTIM_ICP_Cfg_Init
*  ×÷   Õß£ºhrx
*  ´´½¨Ê±¼ä£º2022Äê3ÔÂ5ÈÕ 
*  ĞŞ¸ÄÊ±¼ä£º
*  Êä   Èë£ºiArr  -> ×°ÔØÖµ
*           iPsc  ->·ÖÆµÏµÊıº
*  Êä   ³ö£º
*  Ëµ   Ã÷£º¶¨Ê±Æ÷²¶»ñ³õÊ¼»¯ÅäÖÃ
*/
void TIM_ICP_Cfg_Init(uint16_t iArr, uint16_t iPsc)
{	
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_ICInitTypeDef ICP_ICInitStructure;
	
	RCC_AHB1PeriphClockCmd(CAP_PORT_CLK, ENABLE); 	        				// Ê¹ÄÜPORTÊ±ÖÓ	
	RCC_APB2PeriphClockCmd(CAP_TIM_CLK, ENABLE);										// TIMÊ±ÖÓÊ¹ÄÜ    
	
	GPIO_InitStructure.GPIO_Pin 	= CAP_LEFT_PIN| CAP_RIGHT_PIN | CAP2_LEFT_PIN | CAP2_RIGHT_PIN;	// GPIO
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AF;            				// ¸´ÓÃ¹¦ÄÜ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	    				// ËÙ¶È100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;          				// ÍÆÍì¸´ÓÃÊä³ö
	GPIO_InitStructure.GPIO_PuPd 	= GPIO_PuPd_DOWN;          				// ÏÂÀ­
	GPIO_Init(CAP_PORT, &GPIO_InitStructure);              					// ³õÊ¼»¯PORT
	
	GPIO_PinAFConfig(CAP_PORT, GPIO_PinSource6, GPIO_AF_TIM8);  		// PB0¸´ÓÃÎ»¶¨Ê±Æ÷3
	GPIO_PinAFConfig(CAP_PORT, GPIO_PinSource7, GPIO_AF_TIM8);  		// PB0¸´ÓÃÎ»¶¨Ê±Æ÷3
	GPIO_PinAFConfig(CAP_PORT, GPIO_PinSource8, GPIO_AF_TIM8);  		// PB0¸´ÓÃÎ»¶¨Ê±Æ÷3
	GPIO_PinAFConfig(CAP_PORT, GPIO_PinSource9, GPIO_AF_TIM8);  		// PB0¸´ÓÃÎ»¶¨Ê±Æ÷3
	
	TIM_TimeBaseStructure.TIM_Period = iArr-1; 	
	TIM_TimeBaseStructure.TIM_Prescaler = iPsc-1;		                // ¸ß¼¶¿ØÖÆ¶¨Ê±Æ÷Ê±ÖÓÔ´TIMxCLK = HCLK=180MHz 
	                                                                // Éè¶¨¶¨Ê±Æ÷ÆµÂÊÎª=TIMxCLK/(TIM_Prescaler+1)=100KHz
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;		        // ¶ÔÍâ²¿Ê±ÖÓ½øĞĞ²ÉÑùµÄÊ±ÖÓ·ÖÆµ,ÕâÀïÃ»ÓĞÓÃµ½ */
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;	      // ¼ÆÊı·½Ê½
	TIM_TimeBaseInit(CAP_TIM, &TIM_TimeBaseStructure);	            // ³õÊ¼»¯¶¨Ê±Æ÷TIMx, x[1,8]	
	/* ³õÊ¼»¯TIM5ÊäÈë²¶»ñ²ÎÊı */
	ICP_ICInitStructure.TIM_Channel 		= TIM_Channel_1;            // CC1S=01 	Ñ¡ÔñÊäÈë¶Ë IC1Ó³Éäµ½TI1ÉÏ
	ICP_ICInitStructure.TIM_ICPolarity 	= TIM_ICPolarity_Rising;	  // ÉÏÉıÑØ²¶»ñ
	ICP_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; // Ó³Éäµ½TI1ÉÏ
	ICP_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	          // ÅäÖÃÊäÈë·ÖÆµ,²»·ÖÆµ 
	ICP_ICInitStructure.TIM_ICFilter 		= 0x00;                     // IC1F=0000 ÅäÖÃÊäÈëÂË²¨Æ÷ ²»ÂË²¨
	TIM_ICInit(CAP_TIM, &ICP_ICInitStructure);
	
	ICP_ICInitStructure.TIM_Channel 		= TIM_Channel_2;            // CC1S=01 	Ñ¡ÔñÊäÈë¶Ë IC1Ó³Éäµ½TI1ÉÏ
	ICP_ICInitStructure.TIM_ICPolarity 	= TIM_ICPolarity_Rising;	  // ÉÏÉıÑØ²¶»ñ
	ICP_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; // Ó³Éäµ½TI1ÉÏ
	ICP_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	          // ÅäÖÃÊäÈë·ÖÆµ,²»·ÖÆµ 
	ICP_ICInitStructure.TIM_ICFilter 		= 0x00;                     // IC1F=0000 ÅäÖÃÊäÈëÂË²¨Æ÷ ²»ÂË²¨
	TIM_ICInit(CAP_TIM, &ICP_ICInitStructure);	
	/* ³õÊ¼»¯TIM5ÊäÈë²¶»ñ²ÎÊı */
	ICP_ICInitStructure.TIM_Channel 		= TIM_Channel_3;            // CC1S=01 	Ñ¡ÔñÊäÈë¶Ë IC1Ó³Éäµ½TI1ÉÏ
	ICP_ICInitStructure.TIM_ICPolarity 	= TIM_ICPolarity_Rising;	  // ÉÏÉıÑØ²¶»ñ
	ICP_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; // Ó³Éäµ½TI1ÉÏ
	ICP_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	          // ÅäÖÃÊäÈë·ÖÆµ,²»·ÖÆµ 
	ICP_ICInitStructure.TIM_ICFilter 		= 0x00;                     // IC1F=0000 ÅäÖÃÊäÈëÂË²¨Æ÷ ²»ÂË²¨
	TIM_ICInit(CAP_TIM, &ICP_ICInitStructure);
	
	ICP_ICInitStructure.TIM_Channel 		= TIM_Channel_4;            // CC1S=01 	Ñ¡ÔñÊäÈë¶Ë IC1Ó³Éäµ½TI1ÉÏ
	ICP_ICInitStructure.TIM_ICPolarity 	= TIM_ICPolarity_Rising;	  // ÉÏÉıÑØ²¶»ñ
	ICP_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; // Ó³Éäµ½TI1ÉÏ
	ICP_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	          // ÅäÖÃÊäÈë·ÖÆµ,²»·ÖÆµ 
	ICP_ICInitStructure.TIM_ICFilter 		= 0x00;                     // IC1F=0000 ÅäÖÃÊäÈëÂË²¨Æ÷ ²»ÂË²¨
	TIM_ICInit(CAP_TIM, &ICP_ICInitStructure);	 
	
	TIM_ITConfig(CAP_TIM, TIM_IT_CC1, ENABLE);											// ÔÊĞí¸üĞÂÖĞ¶Ï ,ÔÊĞíCC1IE²¶»ñÖĞ¶Ï	
	TIM_ITConfig(CAP_TIM, TIM_IT_CC2, ENABLE);											// ÔÊĞí¸üĞÂÖĞ¶Ï ,ÔÊĞíCC1IE²¶»ñÖĞ¶Ï	
	TIM_ITConfig(CAP_TIM, TIM_IT_CC3, ENABLE);											// ÔÊĞí¸üĞÂÖĞ¶Ï ,ÔÊĞíCC1IE²¶»ñÖĞ¶Ï	
	TIM_ITConfig(CAP_TIM, TIM_IT_CC4, ENABLE);											// ÔÊĞí¸üĞÂÖĞ¶Ï ,ÔÊĞíCC1IE²¶»ñÖĞ¶Ï	
	
	TIM_Cmd(CAP_TIM, DISABLE); 	                           					// ½ûÖ¹¶¨Ê±Æ÷5
	/* NVIC ÅäÖÃ, Í³Ò»·ÅÖÃÓÚmain.c Í³Ò»´¦Àí */
}

void TIM8_Configuration(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStruct;
    TIM_ICInitTypeDef TIM_ICInitStruct;

    /* GPIO³õÊ¼»¯ */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

    GPIO_InitStruct.GPIO_Pin 		= GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStruct.GPIO_Mode 	= GPIO_Mode_AF;
    GPIO_InitStruct.GPIO_Speed 	= GPIO_Speed_100MHz;
    GPIO_InitStruct.GPIO_OType 	= GPIO_OType_PP;
    GPIO_InitStruct.GPIO_PuPd 	= GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOC, &GPIO_InitStruct);

    GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_TIM8);
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource7, GPIO_AF_TIM8);
    /* Ê±»ù³õÊ¼»¯ */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);
    TIM_DeInit(TIM8);
    TIM_TimeBaseStructInit(&TIM_TimeBaseStruct);
    TIM_TimeBaseStruct.TIM_Prescaler			= ENCODER_TIM_PSC;
    TIM_TimeBaseStruct.TIM_Period 				= ENCODER_TIM_PERIOD;
    TIM_TimeBaseStruct.TIM_ClockDivision 	= TIM_CKD_DIV1;
    TIM_TimeBaseStruct.TIM_CounterMode 		= TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM8, &TIM_TimeBaseStruct);
    /* ±àÂëÆ÷Ä£Ê½ÅäÖÃ£ºÍ¬Ê±²¶»ñÍ¨µÀ1ÓëÍ¨µÀ2(¼´4±¶Æµ)£¬¼«ĞÔ¾ùÎªRising */
    TIM_EncoderInterfaceConfig(TIM8, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
    TIM_ICStructInit(&TIM_ICInitStruct);
    TIM_ICInitStruct.TIM_ICFilter = 0;
    TIM_ICInit(TIM8, &TIM_ICInitStruct);
    TIM_SetCounter(TIM8, CNT_INIT);
    TIM_ClearFlag(TIM8, TIM_IT_Update);
    TIM_ITConfig(TIM8, TIM_IT_Update, ENABLE);
    TIM_Cmd(TIM8, ENABLE);
}


int encoderNum = CNT_INIT;
int encoderOld = CNT_INIT;
int hight = 0, hight_read = 0, hight_mm = 0, reset_cnt = 0;
/* ¶ÁÈ¡¶¨Ê±Æ÷¼ÆÊıÖµ */
static int read_encoder(void)
{
	encoderNum = TIM_GetCounter(TIM8);	
	/* ²»¼ÌĞøÉı½µÁË */
	if(encoderNum == encoderOld)
	{
		if(Lift_Moto.Set_Height == 0)
		{
			reset_cnt++;
			if(reset_cnt >= 100)
			{
				encoderNum = CNT_INIT;
				encoderOld = CNT_INIT;
				TIM_SetCounter(TIM8, CNT_INIT);		/* CNTÉè³õÖµ */	
				reset_cnt = 0;
			}
		}		
		if(Lift_Moto.Set_Height == MAX_HEIGHT_3)
		{
			reset_cnt++;
			if(reset_cnt >= 100)
			{
//				encoderNum = 4750;
//				encoderOld = 4750;
//				TIM_SetCounter(TIM8, 4750);		/* CNTÉè³õÖµ */	
				encoderNum = 4260;
				encoderOld = 4260;
				TIM_SetCounter(TIM8, 4260);		/* CNTÉè³õÖµ */	
				Lift_Moto.Lift_OK_flag = true;
				reset_cnt = 0;
			}
		}
	}
	/* »¹ÔÚ¼ÌĞøÉı½µ */
	else
	{
		reset_cnt = 0;
		/* Âö³åÓĞ´óÌø¶¯ */
		if(abs(encoderNum - encoderOld) > 20) 
		{
			encoderNum = encoderOld;
			TIM_SetCounter(TIM8, encoderOld);
		}
		/* 700mmĞĞ³ÌÉı½µ¸ËÓĞÊ±ºòÉÏÉıµÄÊ±ºòÂö³å»á»ØÌø */
		else if(Lift_Moto.Height < Lift_Moto.Set_Height)
		{
			if(encoderNum - encoderOld < 0)
			{
//				printf("Lift_Moto.Height: %d Lift_Moto.Set_Height: %d\r\n",Lift_Moto.Height, Lift_Moto.Set_Height);
//				printf("encoderNum: %d encoderOld: %d\r\n",encoderNum, encoderOld);
				encoderNum = encoderOld + 3;
				TIM_SetCounter(TIM8, encoderOld + 3);
			}
			else
				encoderOld = encoderNum;
		}
		else 
			encoderOld = encoderNum;
		
		if(encoderNum < CNT_INIT) 
			TIM_SetCounter(TIM8, CNT_INIT);		/* CNTÉè³õÖµ */	
	}
//	printf("encoderNum: %d\r\n", encoderNum);
	return encoderNum;
}

/* ¼ÆËã¸ß¶È£¨±»ÁíÒ»¸ö¶¨Ê±Æ÷Ã¿100msµ÷ÓÃ1´Î£© */ 
int GetLiftHeight(void)
{
	/*¶ÁÈ¡±àÂëÆ÷µÄÖµ£¬Õı¸º´ú±íĞı×ª·½Ïò*/
	hight_read = read_encoder();
//	hight = (hight_read - 1000) * 0.0213;		/* (80/3750) = 0.0213 */
	hight = (hight_read - 1000) * 0.0215;		/* (70/3260) = 0.0218 */
	hight_mm = ((hight_read - 1000) * 0.0213) * 10;
	printf("hight: %d       hight_mm: %d\r\n", hight, hight_mm);
	return hight;
}


