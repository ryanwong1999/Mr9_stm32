
/*=============================================================================
*  Copyright (C) ".strftime("%Y")." All rights reserved.
*
*  �ļ�����"App_System.c"
*  ���ߣ�hrx
*  ����ʱ�䣺2022��4��11�� 
*  �޸�ʱ�䣺
*  ˵����ϵͳ����
*
==============================================================================*/


/* Includes ------------------------------------------------------------------*/

#include "includes.h"

/*=============================================================================
*  ������ ��RCC_Configuration
*  ��   �ߣ�hrx
*  ����ʱ�䣺2022��4��11�� 
*  �޸�ʱ�䣺
*  ��   �룺
*           
*  ��   ����
*  ˵   ����ϵͳʱ������
*/

void  RCC_Configuration(void)
{
	uint32_t           PLL_M;      
  uint32_t           PLL_N;
  uint32_t           PLL_P;
  uint32_t           PLL_Q;
	
	RCC_DeInit();                      // ��λRCC�ⲿ�豸�Ĵ�����Ĭ��ֵ
	RCC_HSEConfig(RCC_HSE_ON);         // ���ⲿ���پ���
    
	//�ȴ��ⲿ����ʱ��׼����
	do{
		RCC_WaitForHSEStartUp();
	}while( RCC_WaitForHSEStartUp() != SUCCESS );
	
	PLL_M         =    8;
	PLL_N        	=    336;
	PLL_P         =    2;
	PLL_Q         =    7;
	
	RCC_PLLConfig(RCC_PLLSource_HSE, PLL_M, PLL_N, PLL_P, PLL_Q);  // ����PLL������ʹ�ܣ���� 168Mhz �� System Clock ʱ��*/
    RCC_PLLCmd(ENABLE);
		
	RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK); // ѡ��PLLʱ����Ϊϵͳʱ��Դ*/
	
	/*�������� AHBʱ�ӣ�HCLK��. Ϊ�˻�ýϸߵ�Ƶ�ʣ����Ƕ� SYSCLK 1��Ƶ���õ�HCLK*/
  RCC_HCLKConfig(RCC_HCLK_Div1);
 
	/*APBxʱ�ӣ�PCLK����AHBʱ�ӣ�HCLK����Ƶ�õ��������������� PCLK*/
	
	/*APB1ʱ������. 4��Ƶ���� PCLK1 = 42 MHz*/
	RCC_PCLK1Config(RCC_HCLK_Div4);

	/*APB2ʱ������. 2��Ƶ���� PCLK2 = 84 MHz*/
	RCC_PCLK2Config(RCC_HCLK_Div2);
}


/*=============================================================================
*  ������ ��NVIC_Configuration
*  ��   �ߣ�hrx
*  ����ʱ�䣺2022��4��11�� 
*  �޸�ʱ�䣺
*  ��   �룺
*           
*  ��   ����
*  ˵   �����ж����ȼ�����
*/
void  NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure; 
	//�����ж���Ϊ1
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);	   					//�жϷ���
	//USART1
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;           //����USARTΪ�ж�Դ 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;   //��ռ���ȼ�Ϊ0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 7;          //�����ȼ�Ϊ1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;             //ʹ���ж� 
	NVIC_Init(&NVIC_InitStructure);	                            //��ʼ������NVIC 	
	//USART2
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;           //����USARTΪ�ж�Դ 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;   //��ռ���ȼ�Ϊ0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 6;          //�����ȼ�Ϊ1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;             //ʹ���ж� 
	NVIC_Init(&NVIC_InitStructure);	   
	//USART3
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;           //����USARTΪ�ж�Դ 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;   //��ռ���ȼ�Ϊ0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;          //�����ȼ�Ϊ1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;             //ʹ���ж� 
	NVIC_Init(&NVIC_InitStructure);	                            //��ʼ������NVIC 	
	//USART3
	NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;           	//����USARTΪ�ж�Դ 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;   //��ռ���ȼ�Ϊ0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 5;          //�����ȼ�Ϊ1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;             //ʹ���ж� 
	NVIC_Init(&NVIC_InitStructure);	        
	//USART3
	NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;           	//����USARTΪ�ж�Դ 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;   //��ռ���ȼ�Ϊ0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;          //�����ȼ�Ϊ1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;             //ʹ���ж� 
	NVIC_Init(&NVIC_InitStructure);	                            //��ʼ������NVIC 	
  //10us������ʱ��
	NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_TIM10_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;   //��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 6;	        //�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			        //IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	  
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM8_CC_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;   //��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 7;	        //�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			        //IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	 
	  //����
//	NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;   //��ռ���ȼ�3
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 7;	        //�����ȼ�3
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			        //IRQͨ��ʹ��
//	NVIC_Init(&NVIC_InitStructure);	
	
  //1ms������ʱ��
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;             //����USARTΪ�ж�Դ 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;   //��ռ���ȼ�Ϊ0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 7;          //�����ȼ�Ϊ1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;             //ʹ���ж� 
	NVIC_Init(&NVIC_InitStructure);	
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM7_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1 ;		//��ռ���ȼ�0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 6;					//�����ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;							//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);
	                               
 	NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream0_IRQn;     //����DMAΪ�ж�Դ 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;   //��ռ���ȼ�Ϊ0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 5;          //�����ȼ�Ϊ1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;             //ʹ���ж� 
	NVIC_Init(&NVIC_InitStructure);	
  //�ⲿ�ж�
	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;            //�ⲿ�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;   //��ռ���ȼ�Ϊ0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;          //�����ȼ�Ϊ1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;             //ʹ���ж� 
	NVIC_Init(&NVIC_InitStructure);	  
	//�ⲿ�ж�
	NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;            //�ⲿ�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;   //��ռ���ȼ�Ϊ0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;          //�����ȼ�Ϊ1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;             //ʹ���ж� 
	NVIC_Init(&NVIC_InitStructure);	  
	 //�ⲿ�ж�
//	NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;            //�ⲿ�ж�
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;   //��ռ���ȼ�Ϊ0
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;          //�����ȼ�Ϊ1
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;             //ʹ���ж� 
//	NVIC_Init(&NVIC_InitStructure);	  
	//�ⲿ�ж�
	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;          //�ⲿ�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;   //��ռ���ȼ�Ϊ0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;          //�����ȼ�Ϊ1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;             //ʹ���ж� 
	NVIC_Init(&NVIC_InitStructure);	                            //��ʼ������NVIC 	
   //�ⲿ�ж�
  NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;        //�ⲿ�ж�
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;   //��ռ���ȼ�3
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;          //�����ȼ�2
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;             //ʹ���ⲿ�ж�ͨ��
  NVIC_Init(&NVIC_InitStructure);                             //��ʼ������NVIC 		
	
}

/*=============================================================================
*  ������ ��System_Board_Init
*  ��   �ߣ�hrx
*  ����ʱ�䣺2022��4��11�� 
*  �޸�ʱ�䣺
*  ��   �룺
*           
*  ��   ����
*  ˵   ����ϵͳӲ����Դ��ʼ��
*/
void  System_Board_Init(void)
{
	delay_init(168);										//ʱ�ӳ�ʼ��

	LED_GPIO_Cfg_Init();    						//LED GPIO initialization
	BEEP_GPIO_Cfg_Init();   						//BEEP GPIO initialization
	Lamp_GPIO_Cfg_Init();
	PowerSys_Gpio_Cfg_Init();						//Power system control GPIO initialization
	PC_Power_Cfg_Init(PC_STARTUP_OFF);

	IR_Decoding_GPIO_Cfg_Init();				//��������ʼ��
	Crash_Gpio_Cfg_Init();
	
	LiftMoto_Gpio_Cfg_Init();
	LimitSwitch_Gpio_Cfg_Init();
		
	Ultrasonic_Exti_Init();
	
	ADC_Cfg_Init();    									//ADC Init
	
	TIMx_Cfg_Init(RCC_APB2Periph_TIM1, TIM1, 10, 168);   //1ms timer
	TIMx_Cfg_Init(RCC_APB1Periph_TIM2, TIM2, 1000, 84);  //10us timer
	
	TIM7_Cfg_Init(100, 840);      		//10ms timer  
	  
	//PWM_Wheel_Cfg_Init(1000, 84);			//PWM initialization.foc = 2K
	PWM_Head_Cfg_Init(20000, 84);   	//TIM3 PWM initialization,
	  
	//TIM_ICP_Cfg_Init(0xffff, 168);		//input capture
	//TIM_Encoder_Init(0xffff, 168);
	TIM8_Configuration();
	
  AT24CXX_Init();	
  USARTx_Cfg_Init(USART1, 115200);  //USART to printf 
	USARTx_Cfg_Init(USART2, 9600);    //USART to 
	USARTx_Cfg_Init(USART3, 9600);    //USART to PC
	//USARTx_Cfg_Init(UART4, 115200);		//USART to 
	USARTx_Cfg_Init(UART5, 9600);			//USART to Environmental
	RS485_Cfg_Init(115200);  					//UART4 to moto drv
	
	my_mem_init(SRAMIN);							//��ʼ���ڲ��ڴ�� 
	//my_mem_init(SRAMEX);							//��ʼ���ⲿ�ڴ��
	my_mem_init(SRAMCCM);							//��ʼ��CCM�ڴ�� 

	printf("Bsp init OK!\r\n");
}


/*=============================================================================
*  ������ ��System_StartTips
*  ��   �ߣ�hrx
*  ����ʱ�䣺2022��4��11�� 
*  �޸�ʱ�䣺
*  ��   �룺
*           
*  ��   ����
*  ˵   ����ϵͳ������������ʾ
*/
void System_StartTips(void)
{
	LED_GREEN_ON;
	LED_TEST_ON;
	BEEP_ON;
	delay_ms(100);
	LED_GREEN_OFF;
	LED_TEST_OFF;
	BEEP_OFF;
	delay_ms(200);
	
	LED_GREEN_ON;
	LED_TEST_ON;
	BEEP_ON ;
	delay_ms(100);
	LED_GREEN_OFF;
	LED_TEST_OFF;
	BEEP_OFF;
	delay_ms(200);	
	
	LED_GREEN_ON;
	LED_TEST_ON;
	BEEP_ON ;
	delay_ms(100);
	BEEP_OFF;
	LED_GREEN_OFF;
	LED_TEST_OFF;
	delay_ms(200);

}
