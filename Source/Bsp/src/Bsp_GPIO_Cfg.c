/*=============================================================================
*  Copyright (C) ".strftime("%Y")." All rights reserved.
*
*  文件名："Bsp_CAP_Cfg.c"
*  作者：hrx
*  创建时间：2022年3月5日 
*  修改时间：
*  说明：输入捕获初始化
*
==============================================================================*/

/* Includes ------------------------------------------------------------------*/
#include "Bsp_GPIO_Cfg.h"
#include "includes.h"
/*=============================================================================
*  函数名 ：LED_GPIO_Cfg_Init
*  作   者：hrx
*  创建时间：2022年3月5日 
*  修改时间：
*  输   入：
*          
*  输   出：
*  说   明：LED指示灯GPIO初始化
*/

void  LED_GPIO_Cfg_Init(void)
{
		/*定义一个GPIO_InitTypeDef类型的结构体*/
	  GPIO_InitTypeDef GPIO_InitStructure;
	
		/*开启LED相关的GPIO外设时钟*/
		RCC_AHB1PeriphClockCmd (LEDG_PORT_CLK , ENABLE); 	
	
		/*选择要控制的GPIO引脚*/															   
		GPIO_InitStructure.GPIO_Pin = LED_GREEN_PIN  ;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;   		/* 设置引脚模式为输出模式       */
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      /* 设置引脚的输出类型为推挽输出 */
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        /* 设置引脚为上拉模式          */
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz; 		/*设置引脚速率为2MHz */  
		GPIO_Init(LEDG_PORT, &GPIO_InitStructure);			      /*调用库函数，使用上面配置的GPIO_InitStructure初始化GPIO*/
 
		RCC_AHB1PeriphClockCmd (LEDR_PORT_CLK , ENABLE); 	
	
		/*选择要控制的GPIO引脚*/															   
		GPIO_InitStructure.GPIO_Pin = LED_RED_PIN   ;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;   		/* 设置引脚模式为输出模式       */
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      /* 设置引脚的输出类型为推挽输出 */
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        /* 设置引脚为上拉模式          */
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz; 		/*设置引脚速率为2MHz */  
		GPIO_Init(LEDR_PORT, &GPIO_InitStructure);			      /*调用库函数，使用上面配置的GPIO_InitStructure初始化GPIO*/
		
	
	  RCC_AHB1PeriphClockCmd (LED_TEST_PORT_CLK , ENABLE); 	
	
	 	GPIO_InitStructure.GPIO_Pin = LED_TEST_PIN   ;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;   		/* 设置引脚模式为输出模式       */
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      /* 设置引脚的输出类型为推挽输出 */
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        /* 设置引脚为上拉模式          */
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz; 		/*设置引脚速率为2MHz */  
		GPIO_Init(LED_TEST_PORT, &GPIO_InitStructure);			      /*调用库函数，使用上面配置的GPIO_InitStructure初始化GPIO*/
     
		 LED_RED_OFF;                                                                                                                                                                                                                
		 LED_GREEN_OFF;
		 LED_TEST_OFF;
}


/*=============================================================================
*  函数名 ：Lamp_GPIO_Cfg_Init
*  作   者：hrx
*  创建时间：2022年3月5日 
*  修改时间：
*  输   入：
*          
*  输   出：
*  说   明：指示灯GPIO初始化
*/

void  Lamp_GPIO_Cfg_Init(void)
{
		/*定义一个GPIO_InitTypeDef类型的结构体*/
	  GPIO_InitTypeDef GPIO_InitStructure;
	
		/*开启LED相关的GPIO外设时钟*/
		RCC_AHB1PeriphClockCmd (LAMP_PORT_CLK , ENABLE); 	
	
		/*选择要控制的GPIO引脚*/															   
		GPIO_InitStructure.GPIO_Pin = LAMP_RED_PIN  | LAMP_GREEN_PIN |LAMP_YELLOW_PIN | LAMP_BEEP_PIN;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;   		/* 设置引脚模式为输出模式       */
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      /* 设置引脚的输出类型为推挽输出 */
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        /* 设置引脚为上拉模式          */
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz; 		/*设置引脚速率为2MHz */  
		GPIO_Init(LAMP_PORT, &GPIO_InitStructure);			      /*调用库函数，使用上面配置的GPIO_InitStructure初始化GPIO*/
 
		RCC_AHB1PeriphClockCmd (LEDR_PORT_CLK , ENABLE); 	
	
    LAMP_RED_OFF;
	  LAMP_GREEN_OFF;
	  LAMP_YELLOW_OFF;
	  LAMP_BEEP_OFF;
		
}


/*=============================================================================
*  函数名 ：BEEP_GPIO_Cfg_Init
*  作   者：hrx
*  创建时间：2022年3月5日 
*  修改时间：
*  输   入：
*          
*  输   出：
*  说   明：蜂鸣器GPIO初始化
*/
void  BEEP_GPIO_Cfg_Init(void)
{
	/*定义一个GPIO_InitTypeDef类型的结构体*/
  GPIO_InitTypeDef GPIO_InitStructure;
	
  RCC_AHB1PeriphClockCmd(BEEP_PORT_CLK 	, ENABLE); 

  GPIO_InitStructure.GPIO_Pin =  BEEP_PIN  ;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;		      // 输出
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;        // 推挽输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;      // 不上拉 ,也不下拉
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	 
  GPIO_Init(BEEP_PORT, &GPIO_InitStructure);	
	
	BEEP_DISABLE ;
}


/*=============================================================================
*  函数名 ：PowerSys_Gpio_Cfg_Init
*  作   者：hrx
*  创建时间：2022年3月5日 
*  修改时间：
*  输   入：
*          
*  输   出：
*  说   明：系统控制输入输出GPIO口初始化
*/
void  PowerSys_Gpio_Cfg_Init(void)
{
	/*定义一个GPIO_InitTypeDef类型的结构体*/
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd ( EN_CHG_PORT_CLK, ENABLE);   //开启相关的GPIO外设时钟	
															 
	GPIO_InitStructure.GPIO_Pin = EN_CHG_PIN;        //充电使能口
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;   		/* 设置引脚模式为输出模式       */
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      /* 设置引脚的输出类型为推挽输出 */
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        /* 设置引脚为上拉模式          */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz; 		/*设置引脚速率为2MHz */  
	GPIO_Init(EN_CHG_PORT, &GPIO_InitStructure);			  /*调用库函数，使用上面配置的GPIO_InitStructure初始化GPIO*/
 
	RCC_AHB1PeriphClockCmd ( EN_OUT_PORT_CLK, ENABLE);   //开启相关的GPIO外设时钟	
													 
	GPIO_InitStructure.GPIO_Pin = EN_OUT_PIN;        //系统供电使能
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;   		/* 设置引脚模式为输出模式       */
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      /* 设置引脚的输出类型为推挽输出 */
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;        /* 设置引脚为上拉模式          */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz; 		/*设置引脚速率为2MHz */  
	GPIO_Init(EN_OUT_PORT, &GPIO_InitStructure);			  /*调用库函数，使用上面配置的GPIO_InitStructure初始化GPIO*/

	RCC_AHB1PeriphClockCmd ( EN24_PORT_CLK, ENABLE);   //开启相关的GPIO外设时钟	
													 
	GPIO_InitStructure.GPIO_Pin = EN24_PIN;           //24V输出使能
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;   		/* 设置引脚模式为输出模式       */
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      /* 设置引脚的输出类型为推挽输出 */
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;        /* 设置引脚为上拉模式          */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz; 		/*设置引脚速率为2MHz */  
	GPIO_Init(EN24_PORT, &GPIO_InitStructure);			  /*调用库函数，使用上面配置的GPIO_InitStructure初始化GPIO*/
	
	RCC_AHB1PeriphClockCmd ( EN_MDRV_PORT_CLK, ENABLE);   //开启相关的GPIO外设时钟	
						 
	GPIO_InitStructure.GPIO_Pin = EN_MDRV_PIN;        //电机驱动供电输出使能
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;   		/* 设置引脚模式为输出模式       */
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      /* 设置引脚的输出类型为推挽输出 */
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;        /* 设置引脚为上拉模式          */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz; 		/*设置引脚速率为2MHz */  
	GPIO_Init(EN_MDRV_PORT, &GPIO_InitStructure);			  /*调用库函数，使用上面配置的GPIO_InitStructure初始化GPIO*/

	RCC_AHB1PeriphClockCmd ( POWERKEY_PORT_CLK, ENABLE);   //开启相关的GPIO外设时钟	
														 
	GPIO_InitStructure.GPIO_Pin = POWERKEY_PIN;        //开关机按键输入口

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;   		/* 设置引脚模式为输出模式       */
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;        /* 设置引脚为下拉模式          */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; 		/*设置引脚速率为2MHz */  
	GPIO_Init(POWERKEY_PORT, &GPIO_InitStructure);			  /*调用库函数，使用上面配置的GPIO_InitStructure初始化GPIO*/
			
	RCC_AHB1PeriphClockCmd ( EN_DC12V_PORT_CLK, ENABLE);   //开启相关的GPIO外设时钟	
															 
	GPIO_InitStructure.GPIO_Pin = EN_DC12V_PIN;        //12V输出使能
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;   		//设置引脚模式为输出模式       
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      // 设置引脚的输出类型为推挽输出 
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;        //设置引脚为下拉模式          
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz; 		//设置引脚速率为2MHz  
	GPIO_Init(EN_DC12V_PORT, &GPIO_InitStructure);			  //调用库函数，使用上面配置的GPIO_InitStructure初始化GPIO

	RCC_AHB1PeriphClockCmd ( PC_EN_PORT_CLK, ENABLE);   //开启相关的GPIO外设时钟	
															 
	GPIO_InitStructure.GPIO_Pin = PC_EN_PIN;          //PC机供电使能
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;   		//设置引脚模式为输出模式       
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      // 设置引脚的输出类型为推挽输出 
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;        //设置引脚为下拉模式          
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz; 		//设置引脚速率为2MHz  
	GPIO_Init(PC_EN_PORT, &GPIO_InitStructure);			  //调用库函数，使用上面配置的GPIO_InitStructure初始化GPIO
	
	RCC_AHB1PeriphClockCmd(VOICE_EN_CLK, ENABLE);		

	GPIO_InitStructure.GPIO_Pin = VOICE_EN_PIN ;         //呼叫按键输入检测
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;		      //
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        // 不上拉 ,也不下拉
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	 
	GPIO_Init(VOICE_EN_PORT, &GPIO_InitStructure);	

	RCC_AHB1PeriphClockCmd(STOP_KEY_CLK, ENABLE);		

	GPIO_InitStructure.GPIO_Pin = STOP_KEY_PIN;        //急停按键输入检测
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;		      //
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        // 不上拉 ,也不下拉
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	 
	GPIO_Init(STOP_KEY_PORT, &GPIO_InitStructure);	

	EN_CHG_DISABLE;
	EN_OUT_DISABLE;
	EN_MDRV_DISABLE;
	EN24_DISABLE;
	EN_DC12V_ENABLE;
	PC_EN_ENABLE;
}


/*=============================================================================
*  函数名 ：PC_Power_Cfg_Init
*  作   者：hrx
*  创建时间：2022年3月5日 
*  修改时间：
*  输   入：
*          
*  输   出：
*  说   明：系统开关机GPIO初始化
*/
void PC_Power_Cfg_Init(uint8_t on_off)
{
	/*定义一个GPIO_InitTypeDef类型的结构体*/
	GPIO_InitTypeDef GPIO_InitStructure;
	/*开启LED相关的GPIO外设时钟*/
	RCC_AHB1PeriphClockCmd ( PC_STARTUP_CLK, ENABLE); 	
	/*选择要控制的GPIO引脚*/															   
	switch(on_off)
	{
		case PC_STARTUP_ON:
				GPIO_InitStructure.GPIO_Pin = PC_STARTUP_PIN;    		//PC开机信号
				GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;   		/* 设置引脚模式为输出模式       */
				GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      /* 设置引脚的输出类型为推挽输出  */
				GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        /* 设置引脚为上拉模式           */
				GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz; 		/*设置引脚速率为2MHz */  
				GPIO_Init(PC_STARTUP_PORT, &GPIO_InitStructure);		/*调用库函数，使用上面配置的GPIO_InitStructure初始化GPIO*/
				PC_STARTUP_LOW;
			break;
		case PC_STARTUP_OFF:
				GPIO_InitStructure.GPIO_Pin = PC_STARTUP_PIN;
				GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;        /* 设置引脚模式为输入模式       */
				GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        /* 设置引脚为上拉模式          */
				GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
				GPIO_Init(PC_STARTUP_PORT,&GPIO_InitStructure);
			break;
		default:
			break;
	}
}


/*=============================================================================
*  函数名 ：Ultrasonic_Exti_Init
*  作   者：hrx
*  创建时间：2022年3月5日 
*  修改时间：
*  输   入：
*          
*  输   出：
*  说   明：超声模块GPIO初始化 ，ECHO口定义为外部中断输入初始化
*/

void  Ultrasonic_Exti_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef   EXTI_InitStructure;	
	
  RCC_AHB1PeriphClockCmd(ULTRA1_PORT_CLK, ENABLE); 

  GPIO_InitStructure.GPIO_Pin =  TRIG1_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;		      // 输出
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;        // 推挽输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;      // 不上拉 ,也不下拉
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	 
  GPIO_Init(ULTRA1_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = ECHO1_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;		      // 输入
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;        // 不上拉 ,也不下拉
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	 
  GPIO_Init(ULTRA1_PORT, &GPIO_InitStructure);
	
	RCC_AHB1PeriphClockCmd(ULTRA2_PORT_CLK, ENABLE); 

  GPIO_InitStructure.GPIO_Pin =  TRIG2_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;		      // 输出
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;        // 推挽输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;      // 不上拉 ,也不下拉
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	 
  GPIO_Init(ULTRA2_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = ECHO2_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;		      // 输入
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;        // 不上拉 ,也不下拉
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	 
  GPIO_Init(ULTRA2_PORT, &GPIO_InitStructure);

	RCC_AHB1PeriphClockCmd(ULTRA3_PORT_CLK, ENABLE); 

  GPIO_InitStructure.GPIO_Pin =  TRIG3_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;		      // 输出
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;        // 推挽输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;      // 不上拉 ,也不下拉
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	 
  GPIO_Init(ULTRA3_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = ECHO3_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;		      // 输入
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;        // 不上拉 ,也不下拉
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	 
  GPIO_Init(ULTRA3_PORT, &GPIO_InitStructure);
	
	RCC_AHB1PeriphClockCmd(ULTRA4_PORT_CLK, ENABLE); 

  GPIO_InitStructure.GPIO_Pin =  TRIG4_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;		      // 输出
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;        // 推挽输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;      // 不上拉 ,也不下拉
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	 
  GPIO_Init(ULTRA4_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = ECHO4_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;		      // 输入
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;        // 不上拉 ,也不下拉
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	 
  GPIO_Init(ULTRA4_PORT, &GPIO_InitStructure);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);					//使能SYSCFG时钟
	
  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOD, EXTI_PinSource7);		// GPIOC.11
	
  EXTI_InitStructure.EXTI_Line = EXTI_LINE_ECHO1;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;	// 脉冲触发
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure); 
	
  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOD, EXTI_PinSource5);		// GPIOC.11
	
  EXTI_InitStructure.EXTI_Line = EXTI_LINE_ECHO2;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;	// 脉冲触发
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure); 
		
  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource9);		// GPIOC.11
	
  EXTI_InitStructure.EXTI_Line = EXTI_LINE_ECHO3;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;	// 脉冲触发
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure); 

  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource11);	// GPIOC.11
	
  EXTI_InitStructure.EXTI_Line = EXTI_LINE_ECHO4;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;	// 脉冲触发
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure); 
}



/*=============================================================================
*  函数名 ：IR_Decoding_GPIO_Cfg_Init
*  作   者：hrx
*  创建时间：2022年3月5日 
*  修改时间：
*  输   入：
*          
*  输   出：
*  说   明：自动充电红外解码GPIO初始化，定义为外部中断
*/

void  IR_Decoding_GPIO_Cfg_Init(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;    
	EXTI_InitTypeDef  EXTI_InitStructure;
	
	RCC_AHB1PeriphClockCmd(IRCODE_PORT_CLK, ENABLE);		
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);					//注意要打开SYSCFG时钟	
	GPIO_InitStructure.GPIO_Pin = IRCODE_BL_PIN | IRCODE_BR_PIN ;		//红外解码
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;         						//输入模式
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;         						//上拉输入
 	GPIO_Init(IRCODE_PORT, &GPIO_InitStructure);         						//初始化 红外解码端口

	// 后面右边解码GPIO
  SYSCFG_EXTILineConfig(EXIT_SOURCE_IRPORT, EXTI_SOURCE_IRBR);                 // GPIOC.11
  EXTI_InitStructure.EXTI_Line = EXTI_LINE_IRBR;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;               // 脉冲触发
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure); 

  SYSCFG_EXTILineConfig(EXIT_SOURCE_IRPORT, EXTI_SOURCE_IRBL);                 // GPIOA.15
  EXTI_InitStructure.EXTI_Line = EXTI_LINE_IRBL;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;               // 脉冲触发
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure); 
}


/*=============================================================================
*  函数名 ：Crash_Gpio_Cfg_Init
*  作   者：hrx
*  创建时间：2022年3月5日 
*  修改时间：
*  输   入：
*          
*  输   出：
*  说   明：碰撞GPIO初始化
*/

void Crash_Gpio_Cfg_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;    
	
	RCC_AHB1PeriphClockCmd(CRASH_FRONT_PORT_CLK, ENABLE);		

	GPIO_InitStructure.GPIO_Pin = CRASH_FRONT_PIN ;     //前碰撞IO
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;		    //输入
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //上拉 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	 
  GPIO_Init(CRASH_FRONT_PORT, &GPIO_InitStructure);
	
	RCC_AHB1PeriphClockCmd(CRASH_BACK_PORT_CLK, ENABLE);		

	GPIO_InitStructure.GPIO_Pin = CRASH_BACK_PIN ;      //前碰撞IO
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;		    //输入
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //上拉 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	 
  GPIO_Init(CRASH_BACK_PORT, &GPIO_InitStructure);	
}


/*=============================================================================
*  函数名 ：LiftMoto_Gpio_Cfg_Init
*  作   者：hrx
*  创建时间：2022年3月5日 
*  修改时间：
*  输   入：
*          
*  输   出：
*  说   明：升降电机GPIO初始化
*/
void LiftMoto_Gpio_Cfg_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;    //定义一个GPIO_InitTypeDef类型的结构体
	
	RCC_AHB1PeriphClockCmd(LIFTMOTO_PORT_CLK, ENABLE);    	//开启 GPIO 的时钟
	
	GPIO_InitStructure.GPIO_Pin = LIFTMOTO_EN_PIN | LIFTMOTO_DIR_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;          	//输出
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;          //推挽输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;        //浮空输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;				
	GPIO_Init(LIFTMOTO_PORT,&GPIO_InitStructure);
	
	RCC_AHB1PeriphClockCmd(LIFTMOTO_PWM_CLK, ENABLE);    		//开启 GPIO 的时钟
	
	GPIO_InitStructure.GPIO_Pin =  LIFTMOTO_PWM_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;          	//输出
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;          //推挽输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;        //浮空输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;				
	GPIO_Init(LIFTMOTO_PWM_PORT,&GPIO_InitStructure);
	
  #ifdef LIFTMOTO_TASK_OS
	LIFTMOTO_DISABLE;
	LIFTMOTO_DOWN;
	LIFTMOTO_PWM_EN;   
  #else	
 	LIFTMOTO_ENABLE;
	LIFTMOTO_UP;
	LIFTMOTO_PWM_EN;   
	#endif
}


/*=============================================================================
*  函数名 ：LimitSwitch_Gpio_Cfg_Init
*  作   者：hrx
*  创建时间：2022年3月5日 
*  修改时间：
*  输   入：
*          
*  输   出：
*  说   明：升降电机限位开关GPIO初始化
*/
void LimitSwitch_Gpio_Cfg_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;    
	
	RCC_AHB1PeriphClockCmd(LIMIT_UP_PORT_CLK, ENABLE);		

	GPIO_InitStructure.GPIO_Pin = LIMIT_UP_PIN;        		//上限位
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;		      //输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;          //上拉 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	 
  GPIO_Init(LIMIT_UP_PORT, &GPIO_InitStructure);
	
	RCC_AHB1PeriphClockCmd(LIMIT_DOWN_PORT_CLK, ENABLE);		

	GPIO_InitStructure.GPIO_Pin = LIMIT_DOWN_PIN;    			//下限位
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;		      //输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;          //上拉 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	 
  GPIO_Init(LIMIT_DOWN_PORT, &GPIO_InitStructure);	
}

