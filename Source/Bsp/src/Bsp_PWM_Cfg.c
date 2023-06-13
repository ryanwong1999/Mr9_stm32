/*=============================================================================
*  Copyright (C) ".strftime("%Y")." All rights reserved.
*
*  ÎÄ¼þÃû£º"Bsp_PWM_Cfg.c"
*  ×÷Õß£ºhrx
*  ´´½¨Ê±¼ä£º2022Äê3ÔÂ5ÈÕ 
*  ÐÞ¸ÄÊ±¼ä£º
*  ËµÃ÷£ºPWM¿ØÖÆ³õÊ¼»¯
*
==============================================================================*/

/* Includes ------------------------------------------------------------------*/
#include "Bsp_PWM_Cfg.h"
#include "sys.h"  
#include "includes.h"


/*=============================================================================
*  º¯ÊýÃû £ºPWM_Wheel_Cfg_Init
*  ×÷   Õß£ºhrx
*  ´´½¨Ê±¼ä£º2022Äê3ÔÂ5ÈÕ 
*  ÐÞ¸ÄÊ±¼ä£º
*  Êä   Èë£ºiArr  -> ×°ÔØÖµ
*           iPsc  ->·ÖÆµÏµÊýº
*  Êä   ³ö£º
*  Ëµ   Ã÷£ºµç»úÇý¶¯¿ØÖÆPWM GPIO³õÊ¼»¯
*/
void PWM_Wheel_Cfg_Init(uint16_t iArr, uint16_t iPsc)
{
	/* PWMÐÅºÅµçÆ½Ìø±äµÄÖµ */   
 	uint16_t CCR1_Val = 0;   // PWM_CH1  Value              
 	uint16_t CCR2_Val = 0;   // PWM_CH2  Value  	
	
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;	
	/* 1: ÅäÖÃGPIO  */
	RCC_AHB1PeriphClockCmd(PWM_WHEEL_GPIO_CLK, ENABLE); 	
	
	GPIO_InitStructure.GPIO_Pin 	=  PWM_LEFT_PIN | PWM_RIGHT_PIN;
  GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AF;		         	// ¹¦ÄÜ¸´ÓÃ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;           	// ÍÆÍì¸´ÓÃÊä³ö
	GPIO_InitStructure.GPIO_PuPd 	= GPIO_PuPd_UP;							// ÉÏÀ­
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_Init(PWM_WHEEL_PORT, &GPIO_InitStructure);		
	
	GPIO_PinAFConfig(PWM_WHEEL_PORT, GPIO_PinSource12, GPIO_AF_TIM4);		// TIM4_CH1
	GPIO_PinAFConfig(PWM_WHEEL_PORT, GPIO_PinSource13, GPIO_AF_TIM4);		// TIM4_CH1

	RCC_APB1PeriphClockCmd(PWM_WHEEL_TIM_CLK, ENABLE);  	             	// TIM5Ê±ÖÓÊ¹ÄÜ      

	TIM_TimeBaseStructure.TIM_Period 				= iArr - 1;                 // ×Ô¶¯ÖØ×°ÔØÖµ
	TIM_TimeBaseStructure.TIM_Prescaler 		= iPsc - 1;                 // ¶¨Ê±Æ÷·ÖÆµ
	TIM_TimeBaseStructure.TIM_CounterMode		= TIM_CounterMode_Up;       // ÏòÉÏ¼ÆÊýÄ£Ê½
	TIM_TimeBaseStructure.TIM_ClockDivision	= TIM_CKD_DIV1; 
	TIM_TimeBaseInit(PWM_WHEEL_TIM, &TIM_TimeBaseStructure);            // ³õÊ¼»¯¶¨Ê±Æ÷
	/* ³õÊ¼»¯TIM CH1 PWMÄ£Ê½ */ 
	TIM_OCInitStructure.TIM_OCMode 				= TIM_OCMode_PWM1;            // Ñ¡Ôñ¶¨Ê±Æ÷Ä£Ê½:TIMÂö³å¿í¶Èµ÷ÖÆÄ£Ê½2
 	TIM_OCInitStructure.TIM_OutputState 	= TIM_OutputState_Enable;     // ±È½ÏÊä³öÊ¹ÄÜ
	TIM_OCInitStructure.TIM_OutputNState 	= TIM_OutputNState_Disable;  	// ¹Ø±Õ»¥²¹Í¨µÀ
	TIM_OCInitStructure.TIM_OCPolarity 		= TIM_OCPolarity_High;        // Êä³ö¼«ÐÔ:TIMÊä³ö±È½Ï¼«ÐÔµÍ
//	TIM_OCInitStructure.TIM_OCNPolarity 	= TIM_OCPolarity_Low;
	TIM_OCInitStructure.TIM_Pulse 				= CCR1_Val;                   // ³õÖµ 0
	TIM_OC1Init(PWM_WHEEL_TIM, &TIM_OCInitStructure);                   // ¸ù¾ÝTÖ¸¶¨µÄ²ÎÊý³õÊ¼»¯ÍâÉè
	TIM_OC1PreloadConfig(PWM_WHEEL_TIM, TIM_OCPreload_Enable);          // Ê¹ÄÜTIM8ÔÚCCR1ÉÏµÄÔ¤×°ÔØ¼Ä´æÆ÷
	
	TIM_OCInitStructure.TIM_Pulse 				= CCR2_Val;                   // ³õÖµ 0
	TIM_OC2Init(PWM_WHEEL_TIM, &TIM_OCInitStructure);                   // ¸ù¾ÝTÖ¸¶¨µÄ²ÎÊý³õÊ¼»¯ÍâÉè
	TIM_OC2PreloadConfig(PWM_WHEEL_TIM, TIM_OCPreload_Enable);          // Ê¹ÄÜTIM8ÔÚCCR2ÉÏµÄÔ¤×°ÔØ¼Ä´æÆ÷	

  TIM_ARRPreloadConfig(PWM_WHEEL_TIM, ENABLE);   // Ê¹ÄÜTIM ÖØÔØ¼Ä´æÆ÷ARR
	
//	TIM_ClearFlag(PWM_TIMx, TIM_FLAG_Update);
	TIM_ITConfig(PWM_WHEEL_TIM, TIM_IT_Update, ENABLE);

  TIM_Cmd(PWM_WHEEL_TIM, ENABLE);								// Ê¹ÄÜ¶¨Ê±Æ÷	
	/* ¶ÔÓÚTIM1 TIM8 ±ØÐëÌí¼Ó */
	TIM_CtrlPWMOutputs(PWM_WHEEL_TIM, ENABLE);		// PWM Êä³öÊ¹ÄÜ
	/* Ä¬ÈÏÊä³ö0Õ¼¿Õ±È */
	PWM_Output_Set(PWM_LEFT_CH, 0);
	PWM_Output_Set(PWM_RIGHT_CH, 0);
}


/*=============================================================================
*  º¯ÊýÃû £ºPWM_Wheel_Cfg_Init
*  ×÷   Õß£ºhrx
*  ´´½¨Ê±¼ä£º2022Äê3ÔÂ5ÈÕ 
*  ÐÞ¸ÄÊ±¼ä£º
*  Êä   Èë£ºucChx  ->Í¨µÀ
*           uiPWM_Val  ->PWMÖµ
*  Êä   ³ö£º
*  Ëµ   Ã÷£ºÉèÖÃÍ¨µÀNµÄPWMÊä³ö
*/
void PWM_Output_Set(uint8_t ucChx, uint16_t uiPWM_Val)
{	
	switch(ucChx)
	{
		case PWM_LEFT_CH:   // ×ó²à
			TIM_SetCompare2(PWM_WHEEL_TIM, uiPWM_Val);
			//TIM_OC2PreloadConfig(PWM_TIMx, TIM_OCPreload_Enable);
			break;
				
		case PWM_RIGHT_CH:  // ÓÒ²à
			TIM_SetCompare1(PWM_WHEEL_TIM, uiPWM_Val);
			//TIM_OC1PreloadConfig(PWM_TIMx, TIM_OCPreload_Enable);
			break;		
		/*
		case PWM_CH3:   // ×ó²à
			TIM_SetCompare3(PWM_WHEEL_TIM, uiPWM_Val);
		  //TIM_OC3PreloadConfig(PWM_TIMx, TIM_OCPreload_Enable);
			break;		
		case PWM_CH4:   // ×ó²à
			TIM_SetCompare4(PWM_WHEEL_TIM, uiPWM_Val);
		  //TIM_OC4PreloadConfig(PWM_TIMx, TIM_OCPreload_Enable);
			break;	
		*/
		default:  
			break;
	}	
}


/*=============================================================================
*  º¯ÊýÃû £ºPWM_Head_Cfg_Init
*  ×÷   Õß£ºhrx
*  ´´½¨Ê±¼ä£º2022Äê3ÔÂ5ÈÕ 
*  ÐÞ¸ÄÊ±¼ä£º
*  Êä   Èë£ºiArr  -> ×°ÔØÖµ
*           iPsc  ->·ÖÆµÏµÊý
*  Êä   ³ö£º
*  Ëµ   Ã÷£ºÔÆÌ¨¶æ»ú¿ØÖÆPWM ³õÊ¼»¯
*/
void PWM_Head_Cfg_Init(uint16_t iArr, uint16_t iPsc)
{
	/* PWMÐÅºÅµçÆ½Ìø±äµÄÖµ */   
 	uint16_t CCR1_Val = 0;   // PWM_CH1  Value              
 	uint16_t CCR2_Val = 0;   // PWM_CH2  Value  	
	
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;	
	/* 1: ÅäÖÃGPIO  */
	RCC_AHB1PeriphClockCmd(PWM_HEAD_PORT_CLK, ENABLE); 	
	
	GPIO_InitStructure.GPIO_Pin 	= PWM_PITCH_PIN | PWM_LEVEL_PIN;
  GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AF;		         					// ¹¦ÄÜ¸´ÓÃ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;           					// ÍÆÍì¸´ÓÃÊä³ö
	GPIO_InitStructure.GPIO_PuPd 	= GPIO_PuPd_UP;             				// ÉÏÀ­
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_Init(PWM_HEAD_PORT, &GPIO_InitStructure);		
	
	GPIO_PinAFConfig(PWM_HEAD_PORT, GPIO_PinSource8, GPIO_AF_TIM4);		// TIM4_CH3
	GPIO_PinAFConfig(PWM_HEAD_PORT, GPIO_PinSource9, GPIO_AF_TIM4);		// TIM4_CH4
	/* 2: ÅäÖÃTIM  (TIM1/TIM8 <==> APB2, TIM2/TIM3/TIM4 ... <==> APB1 ) */
	RCC_APB1PeriphClockCmd(PWM_HEAD_TIM_CLK, ENABLE);  	              // TIM5Ê±ÖÓÊ¹ÄÜ      

	TIM_TimeBaseStructure.TIM_Period 				= iArr - 1;               // ×Ô¶¯ÖØ×°ÔØÖµ
	TIM_TimeBaseStructure.TIM_Prescaler 		= iPsc - 1;               // ¶¨Ê±Æ÷·ÖÆµ
	TIM_TimeBaseStructure.TIM_CounterMode		= TIM_CounterMode_Up;     // ÏòÉÏ¼ÆÊýÄ£Ê½
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 
	TIM_TimeBaseInit(PWM_HEAD_TIM, &TIM_TimeBaseStructure);           // ³õÊ¼»¯¶¨Ê±Æ÷
	
	/* ³õÊ¼»¯TIM CH1 PWMÄ£Ê½ */	 
	TIM_OCInitStructure.TIM_OCMode 				= TIM_OCMode_PWM1;          // Ñ¡Ôñ¶¨Ê±Æ÷Ä£Ê½:TIMÂö³å¿í¶Èµ÷ÖÆÄ£Ê½2
 	TIM_OCInitStructure.TIM_OutputState 	= TIM_OutputState_Enable;		// ±È½ÏÊä³öÊ¹ÄÜ
	TIM_OCInitStructure.TIM_OutputNState 	= TIM_OutputNState_Disable; // ¹Ø±Õ»¥²¹Í¨µÀ
	TIM_OCInitStructure.TIM_OCPolarity 		= TIM_OCPolarity_High;      // Êä³ö¼«ÐÔ:TIMÊä³ö±È½Ï¼«ÐÔµÍ
//	TIM_OCInitStructure.TIM_OCNPolarity 	= TIM_OCPolarity_Low;
		
	TIM_OCInitStructure.TIM_Pulse					= CCR1_Val;                 // ³õÖµ 0
	TIM_OC3Init(PWM_HEAD_TIM, &TIM_OCInitStructure);                  // ¸ù¾ÝTÖ¸¶¨µÄ²ÎÊý³õÊ¼»¯ÍâÉè
	TIM_OC3PreloadConfig(PWM_HEAD_TIM, TIM_OCPreload_Enable);         // Ê¹ÄÜTIM8ÔÚCCR1ÉÏµÄÔ¤×°ÔØ¼Ä´æÆ÷
	
	TIM_OCInitStructure.TIM_Pulse 				= CCR2_Val;                 // ³õÖµ 0
	TIM_OC4Init(PWM_HEAD_TIM, &TIM_OCInitStructure);                  // ¸ù¾ÝTÖ¸¶¨µÄ²ÎÊý³õÊ¼»¯ÍâÉè
	TIM_OC4PreloadConfig(PWM_HEAD_TIM, TIM_OCPreload_Enable);         // Ê¹ÄÜTIM8ÔÚCCR2ÉÏµÄÔ¤×°ÔØ¼Ä´æÆ÷	
 
  TIM_ARRPreloadConfig(PWM_HEAD_TIM, ENABLE);   										// Ê¹ÄÜTIM ÖØÔØ¼Ä´æÆ÷ARR
	
//	TIM_ClearFlag(PWM_TIMx, TIM_FLAG_Update);
	TIM_ITConfig(PWM_HEAD_TIM, TIM_IT_Update, ENABLE);

  TIM_Cmd(PWM_HEAD_TIM, ENABLE);                 					  // Ê¹ÄÜ¶¨Ê±Æ÷	
	/* ¶ÔÓÚTIM1 TIM8 ±ØÐëÌí¼Ó */
	TIM_CtrlPWMOutputs(PWM_HEAD_TIM, ENABLE);	                // PWM Êä³öÊ¹ÄÜ
}


/*=============================================================================
*  º¯ÊýÃû £ºHead_PWM_Set
*  ×÷   Õß£ºhrx
*  ´´½¨Ê±¼ä£º2022Äê3ÔÂ5ÈÕ 
*  ÐÞ¸ÄÊ±¼ä£º
*  Êä   Èë£ºucChx  ->Í¨µÀ
*           uiPWM_Val  ->PWMÖµ
*  Êä   ³ö£º
*  Ëµ   Ã÷£ºÉèÖÃÔÆÌ¨µÄPWMÊä³ö
*/
void Head_PWM_Set(uint8_t _usChannel, uint16_t _usPWM_Val)
{
	switch(_usChannel)
	{
		case PWM_PITCH_CH:
			TIM_SetCompare3(PWM_HEAD_TIM, _usPWM_Val);
			break;
		
		case PWM_LEVEL_CH:
			TIM_SetCompare4(PWM_HEAD_TIM, _usPWM_Val);
			break;
		default:
			break;
	}
}


/*=============================================================================
*  º¯ÊýÃû £ºSetHeadPitchPosition
*  ×÷   Õß£ºhrx
*  ´´½¨Ê±¼ä£º2022Äê3ÔÂ5ÈÕ 
*  ÐÞ¸ÄÊ±¼ä£º
*  Êä   Èë£ºpos  ->½Ç¶È
*           offset  ->Æ«²îÖµ
*  Êä   ³ö£º
*  Ëµ   Ã÷£ºÉèÖÃÔÆÌ¨¸©Ñö½Ç
*/
void SetHeadPitchPosition(float pos, float offset)
{
	uint16_t tmp ;
	Head_PWM_Set(PWM_PITCH_CH, (uint16_t)(500+((pos+offset)*2000/270)));  	// ¶ÔÓ¦270¶È¶æ»ú
}


/*=============================================================================
*  º¯ÊýÃû £ºSetHeadLevelPosition
*  ×÷   Õß£ºhrx
*  ´´½¨Ê±¼ä£º2022Äê3ÔÂ5ÈÕ 
*  ÐÞ¸ÄÊ±¼ä£º
*  Êä   Èë£ºpos  ->½Ç¶È
*           offset  ->Æ«²îÖµ
*  Êä   ³ö£º
*  Ëµ   Ã÷£ºÉèÖÃÔÆÌ¨Ë®Æ½×ª¶¯½Ç¶È
*/
void SetHeadLevelPosition(float pos, float offset)
{
	uint16_t tmp ;
	Head_PWM_Set(PWM_LEVEL_CH, (500+(uint16_t)((180-pos+offset)*2000/270)));	// ¶ÔÓ¦270¶È¶æ»ú
}


