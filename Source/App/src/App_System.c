
/*=============================================================================
*  Copyright (C) ".strftime("%Y")." All rights reserved.
*
*  文件名："App_System.c"
*  作者：hrx
*  创建时间：2022年4月11日 
*  修改时间：
*  说明：系统配置
*
==============================================================================*/


/* Includes ------------------------------------------------------------------*/

#include "includes.h"

/*=============================================================================
*  函数名 ：RCC_Configuration
*  作   者：hrx
*  创建时间：2022年4月11日 
*  修改时间：
*  输   入：
*           
*  输   出：
*  说   明：系统时钟配置
*/

void  RCC_Configuration(void)
{
	uint32_t           PLL_M;      
  uint32_t           PLL_N;
  uint32_t           PLL_P;
  uint32_t           PLL_Q;
	
	RCC_DeInit();                      // 复位RCC外部设备寄存器到默认值
	RCC_HSEConfig(RCC_HSE_ON);         // 打开外部高速晶振
    
	//等待外部高速时钟准备好
	do{
		RCC_WaitForHSEStartUp();
	}while( RCC_WaitForHSEStartUp() != SUCCESS );
	
	PLL_M         =    8;
	PLL_N        	=    336;
	PLL_P         =    2;
	PLL_Q         =    7;
	
	RCC_PLLConfig(RCC_PLLSource_HSE, PLL_M, PLL_N, PLL_P, PLL_Q);  // 配置PLL并将其使能，获得 168Mhz 的 System Clock 时钟*/
    RCC_PLLCmd(ENABLE);
		
	RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK); // 选择PLL时钟作为系统时钟源*/
	
	/*首先配置 AHB时钟（HCLK）. 为了获得较高的频率，我们对 SYSCLK 1分频，得到HCLK*/
  RCC_HCLKConfig(RCC_HCLK_Div1);
 
	/*APBx时钟（PCLK）由AHB时钟（HCLK）分频得到，下面我们配置 PCLK*/
	
	/*APB1时钟配置. 4分频，即 PCLK1 = 42 MHz*/
	RCC_PCLK1Config(RCC_HCLK_Div4);

	/*APB2时钟配置. 2分频，即 PCLK2 = 84 MHz*/
	RCC_PCLK2Config(RCC_HCLK_Div2);
}


/*=============================================================================
*  函数名 ：NVIC_Configuration
*  作   者：hrx
*  创建时间：2022年4月11日 
*  修改时间：
*  输   入：
*           
*  输   出：
*  说   明：中断优先级配置
*/
void  NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure; 
	//设置中断组为1
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);	   					//中断分组
	//USART1
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;           //配置USART为中断源 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;   //抢占优先级为0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 7;          //子优先级为1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;             //使能中断 
	NVIC_Init(&NVIC_InitStructure);	                            //初始化配置NVIC 	
	//USART2
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;           //配置USART为中断源 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;   //抢占优先级为0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 6;          //子优先级为1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;             //使能中断 
	NVIC_Init(&NVIC_InitStructure);	   
	//USART3
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;           //配置USART为中断源 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;   //抢占优先级为0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;          //子优先级为1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;             //使能中断 
	NVIC_Init(&NVIC_InitStructure);	                            //初始化配置NVIC 	
	//USART3
	NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;           	//配置USART为中断源 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;   //抢占优先级为0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 5;          //子优先级为1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;             //使能中断 
	NVIC_Init(&NVIC_InitStructure);	        
	//USART3
	NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;           	//配置USART为中断源 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;   //抢占优先级为0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;          //子优先级为1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;             //使能中断 
	NVIC_Init(&NVIC_InitStructure);	                            //初始化配置NVIC 	
  //10us基本定时器
	NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_TIM10_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;   //抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 6;	        //子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			        //IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	  
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM8_CC_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;   //抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 7;	        //子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			        //IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	 
	  //捕获
//	NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;   //抢占优先级3
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 7;	        //子优先级3
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			        //IRQ通道使能
//	NVIC_Init(&NVIC_InitStructure);	
	
  //1ms基本定时器
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;             //配置USART为中断源 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;   //抢占优先级为0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 7;          //子优先级为1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;             //使能中断 
	NVIC_Init(&NVIC_InitStructure);	
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM7_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1 ;		//抢占优先级0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 6;					//子优先级1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;							//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);
	                               
 	NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream0_IRQn;     //配置DMA为中断源 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;   //抢占优先级为0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 5;          //子优先级为1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;             //使能中断 
	NVIC_Init(&NVIC_InitStructure);	
  //外部中断
	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;            //外部中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;   //抢占优先级为0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;          //子优先级为1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;             //使能中断 
	NVIC_Init(&NVIC_InitStructure);	  
	//外部中断
	NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;            //外部中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;   //抢占优先级为0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;          //子优先级为1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;             //使能中断 
	NVIC_Init(&NVIC_InitStructure);	  
	 //外部中断
//	NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;            //外部中断
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;   //抢占优先级为0
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;          //子优先级为1
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;             //使能中断 
//	NVIC_Init(&NVIC_InitStructure);	  
	//外部中断
	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;          //外部中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;   //抢占优先级为0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;          //子优先级为1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;             //使能中断 
	NVIC_Init(&NVIC_InitStructure);	                            //初始化配置NVIC 	
   //外部中断
  NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;        //外部中断
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;   //抢占优先级3
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;          //子优先级2
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;             //使能外部中断通道
  NVIC_Init(&NVIC_InitStructure);                             //初始化配置NVIC 		
	
}

/*=============================================================================
*  函数名 ：System_Board_Init
*  作   者：hrx
*  创建时间：2022年4月11日 
*  修改时间：
*  输   入：
*           
*  输   出：
*  说   明：系统硬件资源初始化
*/
void  System_Board_Init(void)
{
	delay_init(168);										//时钟初始化

	LED_GPIO_Cfg_Init();    						//LED GPIO initialization
	BEEP_GPIO_Cfg_Init();   						//BEEP GPIO initialization
	Lamp_GPIO_Cfg_Init();
	PowerSys_Gpio_Cfg_Init();						//Power system control GPIO initialization
	PC_Power_Cfg_Init(PC_STARTUP_OFF);

	IR_Decoding_GPIO_Cfg_Init();				//红外解码初始化
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
	
	my_mem_init(SRAMIN);							//初始化内部内存池 
	//my_mem_init(SRAMEX);							//初始化外部内存池
	my_mem_init(SRAMCCM);							//初始化CCM内存池 

	printf("Bsp init OK!\r\n");
}


/*=============================================================================
*  函数名 ：System_StartTips
*  作   者：hrx
*  创建时间：2022年4月11日 
*  修改时间：
*  输   入：
*           
*  输   出：
*  说   明：系统启动蜂鸣器提示
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
