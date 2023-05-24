/*=============================================================================
*  Copyright (C) ".strftime("%Y")." All rights reserved.
*
*  �ļ�����"Bsp_CAP_Cfg.c"
*  ���ߣ�hrx
*  ����ʱ�䣺2022��3��5�� 
*  �޸�ʱ�䣺
*  ˵�������벶���ʼ��
*
==============================================================================*/

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include "Bsp_CAP_Cfg.h"

#define ENCODER_RESOLUTION 1    /*������һȦ������������*/
#define ENCODER_MULTIPLE 4      /*��������Ƶ��ͨ����ʱ���ı�����ģʽ����*/
#define MOTOR_REDUCTION_RATIO 1 /*����ļ��ٱ�*/
/*���תһȦ�ܵ�������(��ʱ���ܶ�����������) = ����������������*��������Ƶ*������ٱ� */
#define TOTAL_RESOLUTION ( ENCODER_RESOLUTION*ENCODER_MULTIPLE*MOTOR_REDUCTION_RATIO ) 

#define TIM_PERIOD 65535

#define ENCODER_TIM_PSC  0          /*��������Ƶ*/
#define ENCODER_TIM_PERIOD  65535   /*���������ֵ*/
#define CNT_INIT 1000                  /*��������ֵ*/

/*=============================================================================
*  ������ ��TIM_ICP_Cfg_Init
*  ��   �ߣ�hrx
*  ����ʱ�䣺2022��3��5�� 
*  �޸�ʱ�䣺
*  ��   �룺iArr  -> װ��ֵ
*           iPsc  ->��Ƶϵ���
*  ��   ����
*  ˵   ������ʱ�������ʼ������
*/
void TIM_ICP_Cfg_Init(uint16_t iArr, uint16_t iPsc)
{	
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_ICInitTypeDef   ICP_ICInitStructure;
	
	RCC_AHB1PeriphClockCmd( CAP_PORT_CLK, ENABLE); 	        // ʹ��PORTʱ��	
	RCC_APB2PeriphClockCmd( CAP_TIM_CLK, ENABLE);  	                // TIMʱ��ʹ��    
	
	GPIO_InitStructure.GPIO_Pin = CAP_LEFT_PIN| CAP_RIGHT_PIN | CAP2_LEFT_PIN | CAP2_RIGHT_PIN;       // GPIO
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;            // ���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	    // �ٶ�100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;          // ���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;          // ����
	GPIO_Init( CAP_PORT, &GPIO_InitStructure);              // ��ʼ��PORT
	
	GPIO_PinAFConfig( CAP_PORT, GPIO_PinSource6,GPIO_AF_TIM8);  // PB0����λ��ʱ��3
	GPIO_PinAFConfig( CAP_PORT, GPIO_PinSource7,GPIO_AF_TIM8);  // PB0����λ��ʱ��3
	GPIO_PinAFConfig( CAP_PORT, GPIO_PinSource8,GPIO_AF_TIM8);  // PB0����λ��ʱ��3
	GPIO_PinAFConfig( CAP_PORT, GPIO_PinSource9,GPIO_AF_TIM8);  // PB0����λ��ʱ��3
	
	
	TIM_TimeBaseStructure.TIM_Period = iArr-1; 	
	TIM_TimeBaseStructure.TIM_Prescaler = iPsc-1;		                // �߼����ƶ�ʱ��ʱ��ԴTIMxCLK = HCLK=180MHz 
	                                                                // �趨��ʱ��Ƶ��Ϊ=TIMxCLK/(TIM_Prescaler+1)=100KHz
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;		        // ���ⲿʱ�ӽ��в�����ʱ�ӷ�Ƶ,����û���õ� */
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;	      // ������ʽ
	TIM_TimeBaseInit( CAP_TIM, &TIM_TimeBaseStructure);	            // ��ʼ����ʱ��TIMx, x[1,8]	
	
	//��ʼ��TIM5���벶�����
	ICP_ICInitStructure.TIM_Channel = TIM_Channel_1;                // CC1S=01 	ѡ������� IC1ӳ�䵽TI1��
	ICP_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	  // �����ز���
	ICP_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; // ӳ�䵽TI1��
	ICP_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	          // ���������Ƶ,����Ƶ 
	ICP_ICInitStructure.TIM_ICFilter = 0x00;                        // IC1F=0000 ���������˲��� ���˲�
	TIM_ICInit( CAP_TIM, &ICP_ICInitStructure);
	
	ICP_ICInitStructure.TIM_Channel = TIM_Channel_2;                // CC1S=01 	ѡ������� IC1ӳ�䵽TI1��
	ICP_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	  // �����ز���
	ICP_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; // ӳ�䵽TI1��
	ICP_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	          // ���������Ƶ,����Ƶ 
	ICP_ICInitStructure.TIM_ICFilter = 0x00;                        // IC1F=0000 ���������˲��� ���˲�
	TIM_ICInit( CAP_TIM, &ICP_ICInitStructure);	 

	//��ʼ��TIM5���벶�����
	ICP_ICInitStructure.TIM_Channel = TIM_Channel_3;                // CC1S=01 	ѡ������� IC1ӳ�䵽TI1��
	ICP_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	  // �����ز���
	ICP_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; // ӳ�䵽TI1��
	ICP_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	          // ���������Ƶ,����Ƶ 
	ICP_ICInitStructure.TIM_ICFilter = 0x00;                        // IC1F=0000 ���������˲��� ���˲�
	TIM_ICInit( CAP_TIM, &ICP_ICInitStructure);
	
	ICP_ICInitStructure.TIM_Channel = TIM_Channel_4;                // CC1S=01 	ѡ������� IC1ӳ�䵽TI1��
	ICP_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	  // �����ز���
	ICP_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; // ӳ�䵽TI1��
	ICP_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	          // ���������Ƶ,����Ƶ 
	ICP_ICInitStructure.TIM_ICFilter = 0x00;                        // IC1F=0000 ���������˲��� ���˲�
	TIM_ICInit( CAP_TIM, &ICP_ICInitStructure);	 
	
	TIM_ITConfig( CAP_TIM,  TIM_IT_CC1,ENABLE);//��������ж� ,����CC1IE�����ж�	
	TIM_ITConfig( CAP_TIM,  TIM_IT_CC2,ENABLE);//��������ж� ,����CC1IE�����ж�	
	TIM_ITConfig( CAP_TIM,  TIM_IT_CC3,ENABLE);//��������ж� ,����CC1IE�����ж�	
	TIM_ITConfig( CAP_TIM,  TIM_IT_CC4,ENABLE);//��������ж� ,����CC1IE�����ж�	
	
	//TIM_Cmd(CAP_TIM,ENABLE ); 	                           //ʹ�ܶ�ʱ��5
	TIM_Cmd(CAP_TIM,DISABLE ); 	                           //��ֹ��ʱ��5
	//NVIC ����, ͳһ������main.c ͳһ����
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
//	                                                                // �趨��ʱ��Ƶ��Ϊ=TIMxCLK/(TIM_Prescaler+1)=100KHz
//	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;		        // ���ⲿʱ�ӽ��в�����ʱ�ӷ�Ƶ,����û���õ� */
//	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;	      // ������ʽ
//	TIM_TimeBaseInit(TIM8, &TIM_TimeBaseStructure);	            // ��ʼ����ʱ��TIMx, x[1,8]	
//	
//	//��ʼ��TIM5���벶�����
//	ICP_ICInitStructure.TIM_Channel = TIM_Channel_1;                // CC1S=01 	ѡ������� IC1ӳ�䵽TI1��
//	ICP_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	  // �����ز���
//	ICP_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; // ӳ�䵽TI1��
//	ICP_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	          // ���������Ƶ,����Ƶ 
//	ICP_ICInitStructure.TIM_ICFilter = 0x00;                        // IC1F=0000 ���������˲��� ���˲�
//	TIM_ICInit(TIM8, &ICP_ICInitStructure);
//	
//	ICP_ICInitStructure.TIM_Channel = TIM_Channel_2;                // CC1S=01 	ѡ������� IC1ӳ�䵽TI1��
//	ICP_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	  // �����ز���
//	ICP_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; // ӳ�䵽TI1��
//	ICP_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	          // ���������Ƶ,����Ƶ 
//	ICP_ICInitStructure.TIM_ICFilter = 0x00;                        // IC1F=0000 ���������˲��� ���˲�
//	TIM_ICInit(TIM8, &ICP_ICInitStructure);	 
//	
//	TIM_ITConfig(TIM8, TIM_IT_CC1, ENABLE);//��������ж� ,����CC1IE�����ж�	
//	TIM_ITConfig(TIM8, TIM_IT_CC2, ENABLE);//��������ж� ,����CC1IE�����ж�	
//	
//	//TIM_Cmd(CAP_TIM, ENABLE ); 	                           //ʹ�ܶ�ʱ��5
//	TIM_Cmd(CAP_TIM, DISABLE ); 	                           //��ֹ��ʱ��5
//	//NVIC ����, ͳһ������main.c ͳһ����
//}

void TIM8_Configuration(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStruct;
    TIM_ICInitTypeDef TIM_ICInitStruct;

    /* GPIO��ʼ�� */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOC, &GPIO_InitStruct);

    GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_TIM8);
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource7, GPIO_AF_TIM8);

    /* ʱ����ʼ�� */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);
    TIM_DeInit(TIM8);
    TIM_TimeBaseStructInit(&TIM_TimeBaseStruct);
    TIM_TimeBaseStruct.TIM_Prescaler = ENCODER_TIM_PSC;
    TIM_TimeBaseStruct.TIM_Period = ENCODER_TIM_PERIOD;
    TIM_TimeBaseStruct.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStruct.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM8, &TIM_TimeBaseStruct);

    /* ������ģʽ���ã�ͬʱ����ͨ��1��ͨ��2(��4��Ƶ)�����Ծ�ΪRising */
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
// ��ȡ��ʱ������ֵ
static int read_encoder(void)
{
    encoderNum = TIM_GetCounter(TIM8);
//    TIM_SetCounter(TIM8, CNT_INIT);/*CNT���ֵ*/
		printf("encoderNum: %d\r\n",encoderNum);
    return encoderNum;
}

//����߶ȣ�����һ����ʱ��ÿ100ms����1�Σ� 
void GetLiftHeight(void)
{
    /*��ȡ��������ֵ������������ת����*/
    encoderNum1 = read_encoder();
}


