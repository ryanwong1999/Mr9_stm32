/*=============================================================================
*  Copyright (C) ".strftime("%Y")." All rights reserved.
*
*  Œƒº˛√˚£∫"Bsp_CAP_Cfg.c"
*  ◊˜’ﬂ£∫hrx
*  ¥¥Ω® ±º‰£∫2022ƒÍ3‘¬5»’ 
*  –ﬁ∏ƒ ±º‰£∫
*  Àµ√˜£∫ ‰»Î≤∂ªÒ≥ı ºªØ
*
==============================================================================*/

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include "Bsp_CAP_Cfg.h"

#define ENCODER_RESOLUTION 1    /*±‡¬Î∆˜“ª»¶µƒŒÔ¿Ì¬ˆ≥Â ˝*/
#define ENCODER_MULTIPLE 4      /*±‡¬Î∆˜±∂∆µ£¨Õ®π˝∂® ±∆˜µƒ±‡¬Î∆˜ƒ£ Ω…Ë÷√*/
#define MOTOR_REDUCTION_RATIO 1 /*µÁª˙µƒºıÀŸ±»*/
/*µÁª˙◊™“ª»¶◊‹µƒ¬ˆ≥Â ˝(∂® ±∆˜ƒ‹∂¡µΩµƒ¬ˆ≥Â ˝) = ±‡¬Î∆˜ŒÔ¿Ì¬ˆ≥Â ˝*±‡¬Î∆˜±∂∆µ*µÁª˙ºıÀŸ±» */
#define TOTAL_RESOLUTION ( ENCODER_RESOLUTION*ENCODER_MULTIPLE*MOTOR_REDUCTION_RATIO ) 

#define TIM_PERIOD 65535

#define ENCODER_TIM_PSC  0          /*º∆ ˝∆˜∑÷∆µ*/
#define ENCODER_TIM_PERIOD  65535   /*º∆ ˝∆˜◊Ó¥Û÷µ*/
#define CNT_INIT 1000                  /*º∆ ˝∆˜≥ı÷µ*/

/*=============================================================================
*  ∫Ø ˝√˚ £∫TIM_ICP_Cfg_Init
*  ◊˜   ’ﬂ£∫hrx
*  ¥¥Ω® ±º‰£∫2022ƒÍ3‘¬5»’ 
*  –ﬁ∏ƒ ±º‰£∫
*   ‰   »Î£∫iArr  -> ◊∞‘ÿ÷µ
*           iPsc  ->∑÷∆µœµ ˝∫
*   ‰   ≥ˆ£∫
*  Àµ   √˜£∫∂® ±∆˜≤∂ªÒ≥ı ºªØ≈‰÷√
*/
void TIM_ICP_Cfg_Init(uint16_t iArr, uint16_t iPsc)
{	
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_ICInitTypeDef   ICP_ICInitStructure;
	
	RCC_AHB1PeriphClockCmd( CAP_PORT_CLK, ENABLE); 	        //  πƒ‹PORT ±÷”	
	RCC_APB2PeriphClockCmd( CAP_TIM_CLK, ENABLE);  	                // TIM ±÷” πƒ‹    
	
	GPIO_InitStructure.GPIO_Pin = CAP_LEFT_PIN| CAP_RIGHT_PIN | CAP2_LEFT_PIN | CAP2_RIGHT_PIN;       // GPIO
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;            // ∏¥”√π¶ƒ‹
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	    // ÀŸ∂»100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;          // Õ∆ÕÏ∏¥”√ ‰≥ˆ
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;          // œ¬¿≠
	GPIO_Init( CAP_PORT, &GPIO_InitStructure);              // ≥ı ºªØPORT
	
	GPIO_PinAFConfig( CAP_PORT, GPIO_PinSource6,GPIO_AF_TIM8);  // PB0∏¥”√Œª∂® ±∆˜3
	GPIO_PinAFConfig( CAP_PORT, GPIO_PinSource7,GPIO_AF_TIM8);  // PB0∏¥”√Œª∂® ±∆˜3
	GPIO_PinAFConfig( CAP_PORT, GPIO_PinSource8,GPIO_AF_TIM8);  // PB0∏¥”√Œª∂® ±∆˜3
	GPIO_PinAFConfig( CAP_PORT, GPIO_PinSource9,GPIO_AF_TIM8);  // PB0∏¥”√Œª∂® ±∆˜3
	
	
	TIM_TimeBaseStructure.TIM_Period = iArr-1; 	
	TIM_TimeBaseStructure.TIM_Prescaler = iPsc-1;		                // ∏ﬂº∂øÿ÷∆∂® ±∆˜ ±÷”‘¥TIMxCLK = HCLK=180MHz 
	                                                                // …Ë∂®∂® ±∆˜∆µ¬ Œ™=TIMxCLK/(TIM_Prescaler+1)=100KHz
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;		        // ∂‘Õ‚≤ø ±÷”Ω¯––≤…—˘µƒ ±÷”∑÷∆µ,’‚¿Ô√ª”–”√µΩ */
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;	      // º∆ ˝∑Ω Ω
	TIM_TimeBaseInit( CAP_TIM, &TIM_TimeBaseStructure);	            // ≥ı ºªØ∂® ±∆˜TIMx, x[1,8]	
	
	//≥ı ºªØTIM5 ‰»Î≤∂ªÒ≤Œ ˝
	ICP_ICInitStructure.TIM_Channel = TIM_Channel_1;                // CC1S=01 	—°‘Ò ‰»Î∂À IC1”≥…‰µΩTI1…œ
	ICP_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	  // …œ…˝—ÿ≤∂ªÒ
	ICP_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; // ”≥…‰µΩTI1…œ
	ICP_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	          // ≈‰÷√ ‰»Î∑÷∆µ,≤ª∑÷∆µ 
	ICP_ICInitStructure.TIM_ICFilter = 0x00;                        // IC1F=0000 ≈‰÷√ ‰»Î¬À≤®∆˜ ≤ª¬À≤®
	TIM_ICInit( CAP_TIM, &ICP_ICInitStructure);
	
	ICP_ICInitStructure.TIM_Channel = TIM_Channel_2;                // CC1S=01 	—°‘Ò ‰»Î∂À IC1”≥…‰µΩTI1…œ
	ICP_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	  // …œ…˝—ÿ≤∂ªÒ
	ICP_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; // ”≥…‰µΩTI1…œ
	ICP_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	          // ≈‰÷√ ‰»Î∑÷∆µ,≤ª∑÷∆µ 
	ICP_ICInitStructure.TIM_ICFilter = 0x00;                        // IC1F=0000 ≈‰÷√ ‰»Î¬À≤®∆˜ ≤ª¬À≤®
	TIM_ICInit( CAP_TIM, &ICP_ICInitStructure);	 

	//≥ı ºªØTIM5 ‰»Î≤∂ªÒ≤Œ ˝
	ICP_ICInitStructure.TIM_Channel = TIM_Channel_3;                // CC1S=01 	—°‘Ò ‰»Î∂À IC1”≥…‰µΩTI1…œ
	ICP_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	  // …œ…˝—ÿ≤∂ªÒ
	ICP_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; // ”≥…‰µΩTI1…œ
	ICP_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	          // ≈‰÷√ ‰»Î∑÷∆µ,≤ª∑÷∆µ 
	ICP_ICInitStructure.TIM_ICFilter = 0x00;                        // IC1F=0000 ≈‰÷√ ‰»Î¬À≤®∆˜ ≤ª¬À≤®
	TIM_ICInit( CAP_TIM, &ICP_ICInitStructure);
	
	ICP_ICInitStructure.TIM_Channel = TIM_Channel_4;                // CC1S=01 	—°‘Ò ‰»Î∂À IC1”≥…‰µΩTI1…œ
	ICP_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	  // …œ…˝—ÿ≤∂ªÒ
	ICP_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; // ”≥…‰µΩTI1…œ
	ICP_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	          // ≈‰÷√ ‰»Î∑÷∆µ,≤ª∑÷∆µ 
	ICP_ICInitStructure.TIM_ICFilter = 0x00;                        // IC1F=0000 ≈‰÷√ ‰»Î¬À≤®∆˜ ≤ª¬À≤®
	TIM_ICInit( CAP_TIM, &ICP_ICInitStructure);	 
	
	TIM_ITConfig( CAP_TIM,  TIM_IT_CC1,ENABLE);//‘ –Ì∏¸–¬÷–∂œ ,‘ –ÌCC1IE≤∂ªÒ÷–∂œ	
	TIM_ITConfig( CAP_TIM,  TIM_IT_CC2,ENABLE);//‘ –Ì∏¸–¬÷–∂œ ,‘ –ÌCC1IE≤∂ªÒ÷–∂œ	
	TIM_ITConfig( CAP_TIM,  TIM_IT_CC3,ENABLE);//‘ –Ì∏¸–¬÷–∂œ ,‘ –ÌCC1IE≤∂ªÒ÷–∂œ	
	TIM_ITConfig( CAP_TIM,  TIM_IT_CC4,ENABLE);//‘ –Ì∏¸–¬÷–∂œ ,‘ –ÌCC1IE≤∂ªÒ÷–∂œ	
	
	//TIM_Cmd(CAP_TIM,ENABLE ); 	                           // πƒ‹∂® ±∆˜5
	TIM_Cmd(CAP_TIM,DISABLE ); 	                           //Ω˚÷π∂® ±∆˜5
	//NVIC ≈‰÷√, Õ≥“ª∑≈÷√”⁄main.c Õ≥“ª¥¶¿Ì
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
//	                                                                // …Ë∂®∂® ±∆˜∆µ¬ Œ™=TIMxCLK/(TIM_Prescaler+1)=100KHz
//	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;		        // ∂‘Õ‚≤ø ±÷”Ω¯––≤…—˘µƒ ±÷”∑÷∆µ,’‚¿Ô√ª”–”√µΩ */
//	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;	      // º∆ ˝∑Ω Ω
//	TIM_TimeBaseInit(TIM8, &TIM_TimeBaseStructure);	            // ≥ı ºªØ∂® ±∆˜TIMx, x[1,8]	
//	
//	//≥ı ºªØTIM5 ‰»Î≤∂ªÒ≤Œ ˝
//	ICP_ICInitStructure.TIM_Channel = TIM_Channel_1;                // CC1S=01 	—°‘Ò ‰»Î∂À IC1”≥…‰µΩTI1…œ
//	ICP_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	  // …œ…˝—ÿ≤∂ªÒ
//	ICP_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; // ”≥…‰µΩTI1…œ
//	ICP_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	          // ≈‰÷√ ‰»Î∑÷∆µ,≤ª∑÷∆µ 
//	ICP_ICInitStructure.TIM_ICFilter = 0x00;                        // IC1F=0000 ≈‰÷√ ‰»Î¬À≤®∆˜ ≤ª¬À≤®
//	TIM_ICInit(TIM8, &ICP_ICInitStructure);
//	
//	ICP_ICInitStructure.TIM_Channel = TIM_Channel_2;                // CC1S=01 	—°‘Ò ‰»Î∂À IC1”≥…‰µΩTI1…œ
//	ICP_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	  // …œ…˝—ÿ≤∂ªÒ
//	ICP_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; // ”≥…‰µΩTI1…œ
//	ICP_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	          // ≈‰÷√ ‰»Î∑÷∆µ,≤ª∑÷∆µ 
//	ICP_ICInitStructure.TIM_ICFilter = 0x00;                        // IC1F=0000 ≈‰÷√ ‰»Î¬À≤®∆˜ ≤ª¬À≤®
//	TIM_ICInit(TIM8, &ICP_ICInitStructure);	 
//	
//	TIM_ITConfig(TIM8, TIM_IT_CC1, ENABLE);//‘ –Ì∏¸–¬÷–∂œ ,‘ –ÌCC1IE≤∂ªÒ÷–∂œ	
//	TIM_ITConfig(TIM8, TIM_IT_CC2, ENABLE);//‘ –Ì∏¸–¬÷–∂œ ,‘ –ÌCC1IE≤∂ªÒ÷–∂œ	
//	
//	//TIM_Cmd(CAP_TIM, ENABLE ); 	                           // πƒ‹∂® ±∆˜5
//	TIM_Cmd(CAP_TIM, DISABLE ); 	                           //Ω˚÷π∂® ±∆˜5
//	//NVIC ≈‰÷√, Õ≥“ª∑≈÷√”⁄main.c Õ≥“ª¥¶¿Ì
//}

void TIM8_Configuration(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStruct;
    TIM_ICInitTypeDef TIM_ICInitStruct;

    /* GPIO≥ı ºªØ */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOC, &GPIO_InitStruct);

    GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_TIM8);
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource7, GPIO_AF_TIM8);

    /*  ±ª˘≥ı ºªØ */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);
    TIM_DeInit(TIM8);
    TIM_TimeBaseStructInit(&TIM_TimeBaseStruct);
    TIM_TimeBaseStruct.TIM_Prescaler = ENCODER_TIM_PSC;
    TIM_TimeBaseStruct.TIM_Period = ENCODER_TIM_PERIOD;
    TIM_TimeBaseStruct.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStruct.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM8, &TIM_TimeBaseStruct);

    /* ±‡¬Î∆˜ƒ£ Ω≈‰÷√£∫Õ¨ ±≤∂ªÒÕ®µ¿1”ÎÕ®µ¿2(º¥4±∂∆µ)£¨º´–‘æ˘Œ™Rising */
    TIM_EncoderInterfaceConfig(TIM8, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
    TIM_ICStructInit(&TIM_ICInitStruct);
    TIM_ICInitStruct.TIM_ICFilter = 0;
    TIM_ICInit(TIM8, &TIM_ICInitStruct);
    TIM_SetCounter(TIM8, CNT_INIT);
    TIM_ClearFlag(TIM8, TIM_IT_Update);
    TIM_ITConfig(TIM8, TIM_IT_Update, ENABLE);
    TIM_Cmd(TIM8, ENABLE);
}

int encoderNum = 0;
int encoderNum1 = 0;
float hight = 0;
// ∂¡»°∂® ±∆˜º∆ ˝÷µ
static int read_encoder(void)
{
    encoderNum = TIM_GetCounter(TIM8);
//    TIM_SetCounter(TIM8, CNT_INIT);/*CNT…Ë≥ı÷µ*/
		printf("encoderNum: %d\r\n",encoderNum);
    return encoderNum;
}

//º∆À„∏ﬂ∂»£®±ª¡Ì“ª∏ˆ∂® ±∆˜√ø100msµ˜”√1¥Œ£© 
void GetLiftHeight(void)
{
    /*∂¡»°±‡¬Î∆˜µƒ÷µ£¨’˝∏∫¥˙±Ì–˝◊™∑ΩœÚ*/
    encoderNum1 = read_encoder();
}


