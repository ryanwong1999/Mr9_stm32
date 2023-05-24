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
#include "Bsp_CAP_Cfg.h"

#define ENCODER_RESOLUTION 1    /*±àÂëÆ÷Ò»È¦µÄÎïÀíÂö³åÊı*/
#define ENCODER_MULTIPLE 4      /*±àÂëÆ÷±¶Æµ£¬Í¨¹ı¶¨Ê±Æ÷µÄ±àÂëÆ÷Ä£Ê½ÉèÖÃ*/
#define MOTOR_REDUCTION_RATIO 1 /*µç»úµÄ¼õËÙ±È*/
/*µç»ú×ªÒ»È¦×ÜµÄÂö³åÊı(¶¨Ê±Æ÷ÄÜ¶Áµ½µÄÂö³åÊı) = ±àÂëÆ÷ÎïÀíÂö³åÊı*±àÂëÆ÷±¶Æµ*µç»ú¼õËÙ±È */
#define TOTAL_RESOLUTION ( ENCODER_RESOLUTION*ENCODER_MULTIPLE*MOTOR_REDUCTION_RATIO ) 

#define TIM_PERIOD 65535

#define ENCODER_TIM_PSC  0          /*¼ÆÊıÆ÷·ÖÆµ*/
#define ENCODER_TIM_PERIOD  65535   /*¼ÆÊıÆ÷×î´óÖµ*/
#define CNT_INIT 0                  /*¼ÆÊıÆ÷³õÖµ*/

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
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_ICInitTypeDef   ICP_ICInitStructure;
	
	RCC_AHB1PeriphClockCmd( CAP_PORT_CLK, ENABLE); 	        // Ê¹ÄÜPORTÊ±ÖÓ	
	RCC_APB2PeriphClockCmd( CAP_TIM_CLK, ENABLE);  	                // TIMÊ±ÖÓÊ¹ÄÜ    
	
	GPIO_InitStructure.GPIO_Pin = CAP_LEFT_PIN| CAP_RIGHT_PIN | CAP2_LEFT_PIN | CAP2_RIGHT_PIN;       // GPIO
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;            // ¸´ÓÃ¹¦ÄÜ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	    // ËÙ¶È100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;          // ÍÆÍì¸´ÓÃÊä³ö
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;          // ÏÂÀ­
	GPIO_Init( CAP_PORT, &GPIO_InitStructure);              // ³õÊ¼»¯PORT
	
	GPIO_PinAFConfig( CAP_PORT, GPIO_PinSource6,GPIO_AF_TIM8);  // PB0¸´ÓÃÎ»¶¨Ê±Æ÷3
	GPIO_PinAFConfig( CAP_PORT, GPIO_PinSource7,GPIO_AF_TIM8);  // PB0¸´ÓÃÎ»¶¨Ê±Æ÷3
	GPIO_PinAFConfig( CAP_PORT, GPIO_PinSource8,GPIO_AF_TIM8);  // PB0¸´ÓÃÎ»¶¨Ê±Æ÷3
	GPIO_PinAFConfig( CAP_PORT, GPIO_PinSource9,GPIO_AF_TIM8);  // PB0¸´ÓÃÎ»¶¨Ê±Æ÷3
	
	
	TIM_TimeBaseStructure.TIM_Period = iArr-1; 	
	TIM_TimeBaseStructure.TIM_Prescaler = iPsc-1;		                // ¸ß¼¶¿ØÖÆ¶¨Ê±Æ÷Ê±ÖÓÔ´TIMxCLK = HCLK=180MHz 
	                                                                // Éè¶¨¶¨Ê±Æ÷ÆµÂÊÎª=TIMxCLK/(TIM_Prescaler+1)=100KHz
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;		        // ¶ÔÍâ²¿Ê±ÖÓ½øĞĞ²ÉÑùµÄÊ±ÖÓ·ÖÆµ,ÕâÀïÃ»ÓĞÓÃµ½ */
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;	      // ¼ÆÊı·½Ê½
	TIM_TimeBaseInit( CAP_TIM, &TIM_TimeBaseStructure);	            // ³õÊ¼»¯¶¨Ê±Æ÷TIMx, x[1,8]	
	
	//³õÊ¼»¯TIM5ÊäÈë²¶»ñ²ÎÊı
	ICP_ICInitStructure.TIM_Channel = TIM_Channel_1;                // CC1S=01 	Ñ¡ÔñÊäÈë¶Ë IC1Ó³Éäµ½TI1ÉÏ
	ICP_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	  // ÉÏÉıÑØ²¶»ñ
	ICP_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; // Ó³Éäµ½TI1ÉÏ
	ICP_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	          // ÅäÖÃÊäÈë·ÖÆµ,²»·ÖÆµ 
	ICP_ICInitStructure.TIM_ICFilter = 0x00;                        // IC1F=0000 ÅäÖÃÊäÈëÂË²¨Æ÷ ²»ÂË²¨
	TIM_ICInit( CAP_TIM, &ICP_ICInitStructure);
	
	ICP_ICInitStructure.TIM_Channel = TIM_Channel_2;                // CC1S=01 	Ñ¡ÔñÊäÈë¶Ë IC1Ó³Éäµ½TI1ÉÏ
	ICP_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	  // ÉÏÉıÑØ²¶»ñ
	ICP_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; // Ó³Éäµ½TI1ÉÏ
	ICP_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	          // ÅäÖÃÊäÈë·ÖÆµ,²»·ÖÆµ 
	ICP_ICInitStructure.TIM_ICFilter = 0x00;                        // IC1F=0000 ÅäÖÃÊäÈëÂË²¨Æ÷ ²»ÂË²¨
	TIM_ICInit( CAP_TIM, &ICP_ICInitStructure);	 

	//³õÊ¼»¯TIM5ÊäÈë²¶»ñ²ÎÊı
	ICP_ICInitStructure.TIM_Channel = TIM_Channel_3;                // CC1S=01 	Ñ¡ÔñÊäÈë¶Ë IC1Ó³Éäµ½TI1ÉÏ
	ICP_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	  // ÉÏÉıÑØ²¶»ñ
	ICP_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; // Ó³Éäµ½TI1ÉÏ
	ICP_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	          // ÅäÖÃÊäÈë·ÖÆµ,²»·ÖÆµ 
	ICP_ICInitStructure.TIM_ICFilter = 0x00;                        // IC1F=0000 ÅäÖÃÊäÈëÂË²¨Æ÷ ²»ÂË²¨
	TIM_ICInit( CAP_TIM, &ICP_ICInitStructure);
	
	ICP_ICInitStructure.TIM_Channel = TIM_Channel_4;                // CC1S=01 	Ñ¡ÔñÊäÈë¶Ë IC1Ó³Éäµ½TI1ÉÏ
	ICP_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	  // ÉÏÉıÑØ²¶»ñ
	ICP_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; // Ó³Éäµ½TI1ÉÏ
	ICP_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	          // ÅäÖÃÊäÈë·ÖÆµ,²»·ÖÆµ 
	ICP_ICInitStructure.TIM_ICFilter = 0x00;                        // IC1F=0000 ÅäÖÃÊäÈëÂË²¨Æ÷ ²»ÂË²¨
	TIM_ICInit( CAP_TIM, &ICP_ICInitStructure);	 
	
	TIM_ITConfig( CAP_TIM,  TIM_IT_CC1,ENABLE);//ÔÊĞí¸üĞÂÖĞ¶Ï ,ÔÊĞíCC1IE²¶»ñÖĞ¶Ï	
	TIM_ITConfig( CAP_TIM,  TIM_IT_CC2,ENABLE);//ÔÊĞí¸üĞÂÖĞ¶Ï ,ÔÊĞíCC1IE²¶»ñÖĞ¶Ï	
	TIM_ITConfig( CAP_TIM,  TIM_IT_CC3,ENABLE);//ÔÊĞí¸üĞÂÖĞ¶Ï ,ÔÊĞíCC1IE²¶»ñÖĞ¶Ï	
	TIM_ITConfig( CAP_TIM,  TIM_IT_CC4,ENABLE);//ÔÊĞí¸üĞÂÖĞ¶Ï ,ÔÊĞíCC1IE²¶»ñÖĞ¶Ï	
	
	//TIM_Cmd(CAP_TIM,ENABLE ); 	                           //Ê¹ÄÜ¶¨Ê±Æ÷5
	TIM_Cmd(CAP_TIM,DISABLE ); 	                           //½ûÖ¹¶¨Ê±Æ÷5
	//NVIC ÅäÖÃ, Í³Ò»·ÅÖÃÓÚmain.c Í³Ò»´¦Àí
}

//void TIM8_Configuration(void) 
//{
//	GPIO_InitTypeDef GPIO_InitStructure;
//	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
//	TIM_ICInitTypeDef   ICP_ICInitStructure;
//	
//  // Enable TIM8 clock
//  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);
//  // Enable GPIOC clock
//  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
//	
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
//  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
//  GPIO_Init(GPIOC, &GPIO_InitStructure);

//	GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_TIM8);
//	GPIO_PinAFConfig(GPIOC, GPIO_PinSource7, GPIO_AF_TIM8);
//	
//  TIM_TimeBaseStructure.TIM_Period = TIM_PERIOD;
//  TIM_TimeBaseStructure.TIM_Prescaler = 83; // APB2 clock (84 MHz) / prescaler = 1 MHz
//	                                                                // Éè¶¨¶¨Ê±Æ÷ÆµÂÊÎª=TIMxCLK/(TIM_Prescaler+1)=100KHz
//	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;		        // ¶ÔÍâ²¿Ê±ÖÓ½øĞĞ²ÉÑùµÄÊ±ÖÓ·ÖÆµ,ÕâÀïÃ»ÓĞÓÃµ½ */
//	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;	      // ¼ÆÊı·½Ê½
//	TIM_TimeBaseInit(TIM8, &TIM_TimeBaseStructure);	            // ³õÊ¼»¯¶¨Ê±Æ÷TIMx, x[1,8]	
//	
//	//³õÊ¼»¯TIM5ÊäÈë²¶»ñ²ÎÊı
//	ICP_ICInitStructure.TIM_Channel = TIM_Channel_1;                // CC1S=01 	Ñ¡ÔñÊäÈë¶Ë IC1Ó³Éäµ½TI1ÉÏ
//	ICP_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	  // ÉÏÉıÑØ²¶»ñ
//	ICP_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; // Ó³Éäµ½TI1ÉÏ
//	ICP_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	          // ÅäÖÃÊäÈë·ÖÆµ,²»·ÖÆµ 
//	ICP_ICInitStructure.TIM_ICFilter = 0x00;                        // IC1F=0000 ÅäÖÃÊäÈëÂË²¨Æ÷ ²»ÂË²¨
//	TIM_ICInit(TIM8, &ICP_ICInitStructure);
//	
//	ICP_ICInitStructure.TIM_Channel = TIM_Channel_2;                // CC1S=01 	Ñ¡ÔñÊäÈë¶Ë IC1Ó³Éäµ½TI1ÉÏ
//	ICP_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	  // ÉÏÉıÑØ²¶»ñ
//	ICP_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; // Ó³Éäµ½TI1ÉÏ
//	ICP_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	          // ÅäÖÃÊäÈë·ÖÆµ,²»·ÖÆµ 
//	ICP_ICInitStructure.TIM_ICFilter = 0x00;                        // IC1F=0000 ÅäÖÃÊäÈëÂË²¨Æ÷ ²»ÂË²¨
//	TIM_ICInit(TIM8, &ICP_ICInitStructure);	 
//	
//	TIM_ITConfig(TIM8, TIM_IT_CC1, ENABLE);//ÔÊĞí¸üĞÂÖĞ¶Ï ,ÔÊĞíCC1IE²¶»ñÖĞ¶Ï	
//	TIM_ITConfig(TIM8, TIM_IT_CC2, ENABLE);//ÔÊĞí¸üĞÂÖĞ¶Ï ,ÔÊĞíCC1IE²¶»ñÖĞ¶Ï	
//	
//	//TIM_Cmd(CAP_TIM, ENABLE ); 	                           //Ê¹ÄÜ¶¨Ê±Æ÷5
//	TIM_Cmd(CAP_TIM, DISABLE ); 	                           //½ûÖ¹¶¨Ê±Æ÷5
//	//NVIC ÅäÖÃ, Í³Ò»·ÅÖÃÓÚmain.c Í³Ò»´¦Àí
//}

void TIM8_Configuration(void) 
{
    GPIO_InitTypeDef GPIO_InitStruct;            /*GPIO*/
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStruct; /*Ê±»ù*/
    TIM_ICInitTypeDef TIM_ICInitStruct;          /*ÊäÈëÍ¨µÀ*/

    /*GPIO³õÊ¼»¯*/    
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE); /*Ê¹ÄÜGPIOÊ±ÖÓ AHB1*/                    
    GPIO_StructInit(&GPIO_InitStruct);        
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7; 
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;        /*¸´ÓÃ¹¦ÄÜ*/
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;  /*ËÙ¶È100MHz*/
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;   
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;        
    GPIO_Init(GPIOB, &GPIO_InitStruct); 
    
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_TIM8); 
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource7, GPIO_AF_TIM8);
	
	  /*Ê±»ù³õÊ¼»¯*/
    RCC_APB1PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);   /*Ê¹ÄÜ¶¨Ê±Æ÷Ê±ÖÓ APB1*/
    TIM_DeInit(TIM4);  
    TIM_TimeBaseStructInit(&TIM_TimeBaseStruct);    
    TIM_TimeBaseStruct.TIM_Prescaler = ENCODER_TIM_PSC;       /*Ô¤·ÖÆµ */        
    TIM_TimeBaseStruct.TIM_Period = ENCODER_TIM_PERIOD;       /*ÖÜÆÚ(ÖØ×°ÔØÖµ)*/
    TIM_TimeBaseStruct.TIM_ClockDivision = TIM_CKD_DIV1;      
    TIM_TimeBaseStruct.TIM_CounterMode = TIM_CounterMode_Up;  /*Á¬ĞøÏòÉÏ¼ÆÊıÄ£Ê½*/  
    TIM_TimeBaseInit(TIM8, &TIM_TimeBaseStruct); 

    /*±àÂëÆ÷Ä£Ê½ÅäÖÃ£ºÍ¬Ê±²¶»ñÍ¨µÀ1ÓëÍ¨µÀ2(¼´4±¶Æµ)£¬¼«ĞÔ¾ùÎªRising*/
    TIM_EncoderInterfaceConfig(TIM8, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising); 
    TIM_ICStructInit(&TIM_ICInitStruct);        
    TIM_ICInitStruct.TIM_ICFilter = 0;   /*ÊäÈëÍ¨µÀµÄÂË²¨²ÎÊı*/
    TIM_ICInit(TIM8, &TIM_ICInitStruct); /*ÊäÈëÍ¨µÀ³õÊ¼»¯*/
    TIM_SetCounter(TIM8, CNT_INIT);      /*CNTÉè³õÖµ*/
    TIM_ClearFlag(TIM8, TIM_IT_Update);   /*ÖĞ¶Ï±êÖ¾Çå0*/
    TIM_ITConfig(TIM8, TIM_IT_Update, ENABLE); /*ÖĞ¶ÏÊ¹ÄÜ*/
    TIM_Cmd(TIM8, ENABLE);                /*Ê¹ÄÜCR¼Ä´æÆ÷*/
}

// ¶ÁÈ¡¶¨Ê±Æ÷¼ÆÊıÖµ
static int read_encoder(void)
{
    int encoderNum = 0;
    encoderNum = (int)((int16_t)(TIM8->CNT)); /*CNTÎªuint32, ×ªÎªint16*/
    TIM_SetCounter(TIM8, CNT_INIT);/*CNTÉè³õÖµ*/

    return encoderNum;
}

//¼ÆËãµç»ú×ªËÙ£¨±»ÁíÒ»¸ö¶¨Ê±Æ÷Ã¿100msµ÷ÓÃ1´Î£© 
void calc_motor_rotate_speed()
{
    int encoderNum = 0;
    float rotateSpeed = 0;
    
    /*¶ÁÈ¡±àÂëÆ÷µÄÖµ£¬Õı¸º´ú±íĞı×ª·½Ïò*/
    encoderNum = read_encoder();
    /* ×ªËÙ(1ÃëÖÓ×ª¶àÉÙÈ¦)=µ¥Î»Ê±¼äÄÚµÄ¼ÆÊıÖµ/×Ü·Ö±æÂÊ*Ê±¼äÏµÊı */
    rotateSpeed = (float)encoderNum/TOTAL_RESOLUTION*10;
    
    printf("encoder: %d\t speed:%.2f rps\r\n",encoderNum,rotateSpeed);
}


