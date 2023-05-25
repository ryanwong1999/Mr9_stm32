/*=============================================================================
*  Copyright (C) ".2016 " All rights reserved.
*
*  文件名："Bsp_GPIO_Cfg.h"
*  作者：zz  (zhangzhen123good@163.com)
*  创建时间：2016年07月25日 
*  修改时间：
*  说明：
*
==============================================================================*/


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __BSP_GPIO_CFG_H
#define __BSP_GPIO_CFG_H

#ifdef __cplusplus
 extern "C" {
#endif 

	 
/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
	 
/* 直接操作寄存器的方法控制IO */
#define	digitalHi(p,i)			 {p->BSRRL=i;}		//设置为高电平
#define digitalLo(p,i)			 {p->BSRRH=i;}		//输出低电平
#define digitalToggle(p,i)	 {p->ODR ^=i;}		//输出反转状态	 
	 

//LED GPIO宏定义
#define LEDG_PORT_CLK          	RCC_AHB1Periph_GPIOC
#define LEDG_PORT              	GPIOC
#define LED_GREEN_PIN	        	GPIO_Pin_5

#define LEDR_PORT_CLK          	RCC_AHB1Periph_GPIOC
#define LEDR_PORT              	GPIOC
#define LED_RED_PIN	          	GPIO_Pin_4

#define LED_TEST_PORT_CLK      	RCC_AHB1Periph_GPIOD
#define LED_TEST_PORT          	GPIOD
#define LED_TEST_PIN	          GPIO_Pin_8
				
#define LED_RED_TOGGLE					digitalToggle(LEDR_PORT, LED_RED_PIN)
#define LED_RED_ON			    		digitalHi(LEDR_PORT, LED_RED_PIN)
#define LED_RED_OFF			    		digitalLo(LEDR_PORT, LED_RED_PIN)

#define LED_GREEN_TOGGLE				digitalToggle(LEDG_PORT, LED_GREEN_PIN)
#define LED_GREEN_ON			  		digitalHi(LEDG_PORT, LED_GREEN_PIN)
#define LED_GREEN_OFF			  		digitalLo(LEDG_PORT, LED_GREEN_PIN)

#define LED_TEST_TOGGLE      		digitalToggle(LED_TEST_PORT, LED_TEST_PIN)
#define LED_TEST_ON			    		GPIO_SetBits(LED_TEST_PORT, LED_TEST_PIN)
#define LED_TEST_OFF			  		GPIO_ResetBits(LED_TEST_PORT, LED_TEST_PIN)


#define LAMP_PORT_CLK          	RCC_AHB1Periph_GPIOD
#define LAMP_PORT              	GPIOD
#define LAMP_RED_PIN	         	GPIO_Pin_10
#define LAMP_GREEN_PIN	        GPIO_Pin_11
#define LAMP_YELLOW_PIN	      	GPIO_Pin_12
#define LAMP_BEEP_PIN          	GPIO_Pin_13
#define LAMP_WHITE_PIN					GPIO_Pin_10
#define LAMP_LEFT_PIN						GPIO_Pin_12
#define LAMP_RIGHT_PIN					GPIO_Pin_13

#define LAMP_RED_TOGGLE      		digitalToggle(LAMP_PORT, LAMP_RED_PIN)
#define LAMP_RED_ON			    		GPIO_SetBits(LAMP_PORT, LAMP_RED_PIN)
#define LAMP_RED_OFF			  		GPIO_ResetBits(LAMP_PORT, LAMP_RED_PIN)

#define LAMP_GREEN_TOGGLE     	digitalToggle(LAMP_PORT, LAMP_GREEN_PIN)
#define LAMP_GREEN_ON			    	GPIO_SetBits(LAMP_PORT, LAMP_GREEN_PIN)
#define LAMP_GREEN_OFF			  	GPIO_ResetBits(LAMP_PORT, LAMP_GREEN_PIN)

#define LAMP_YELLOW_TOGGLE    	digitalToggle(LAMP_PORT, LAMP_YELLOW_PIN)
#define LAMP_YELLOW_ON			  	GPIO_SetBits(LAMP_PORT, LAMP_YELLOW_PIN)
#define LAMP_YELLOW_OFF			  	GPIO_ResetBits(LAMP_PORT, LAMP_YELLOW_PIN)

#define LAMP_BEEP_ON			    	GPIO_SetBits(LAMP_PORT, LAMP_BEEP_PIN)
#define LAMP_BEEP_OFF			  		GPIO_ResetBits(LAMP_PORT, LAMP_BEEP_PIN)

#define LAMP_WHITE_ON			    	GPIO_SetBits(LAMP_PORT, LAMP_WHITE_PIN)
#define LAMP_WHITE_OFF			  	GPIO_ResetBits(LAMP_PORT, LAMP_WHITE_PIN)

#define LAMP_LEFT_ON			    	GPIO_SetBits(LAMP_PORT, LAMP_LEFT_PIN)
#define LAMP_LEFT_OFF			  		GPIO_ResetBits(LAMP_PORT, LAMP_LEFT_PIN)

#define LAMP_RIGHT_ON			    	GPIO_SetBits(LAMP_PORT, LAMP_RIGHT_PIN)
#define LAMP_RIGHT_OFF			  	GPIO_ResetBits(LAMP_PORT, LAMP_RIGHT_PIN)


// BEEP
#define BEEP_PORT_CLK          	RCC_AHB1Periph_GPIOD
#define BEEP_PORT              	GPIOD
#define BEEP_PIN               	GPIO_Pin_9
       
#define BEEP_ENABLE            	GPIO_SetBits(BEEP_PORT, BEEP_PIN);
#define BEEP_DISABLE           	GPIO_ResetBits(BEEP_PORT, BEEP_PIN);

#define BEEP_TOGGLE		   				digitalToggle(BEEP_PORT,BEEP_PIN)
#define BEEP_ON		       				digitalHi(BEEP_PORT, BEEP_PIN)
#define BEEP_OFF				 				digitalLo(BEEP_PORT, BEEP_PIN)	 

#define ON  			0
#define OFF 			1


//charging control
#define EN_CHG_PORT_CLK        	RCC_AHB1Periph_GPIOA
#define EN_CHG_PORT            	GPIOA
#define EN_CHG_PIN             	GPIO_Pin_6
       
#define EN_CHG_ENABLE          	GPIO_ResetBits(EN_CHG_PORT, EN_CHG_PIN);
#define EN_CHG_DISABLE         	GPIO_SetBits(EN_CHG_PORT, EN_CHG_PIN);


// OUT CONTROL
#define EN_OUT_PORT_CLK        	RCC_AHB1Periph_GPIOE
#define EN_OUT_PORT            	GPIOE
#define EN_OUT_PIN             	GPIO_Pin_6
       
#define EN_OUT_ENABLE          	GPIO_SetBits(EN_OUT_PORT, EN_OUT_PIN);
#define EN_OUT_DISABLE         	GPIO_ResetBits(EN_OUT_PORT, EN_OUT_PIN);

// OUT24V CONTROL	24V
#define EN24_PORT_CLK          	RCC_AHB1Periph_GPIOE
#define EN24_PORT              	GPIOE
#define EN24_PIN               	GPIO_Pin_5
       
#define EN24_ENABLE            	GPIO_SetBits(EN24_PORT, EN24_PIN);
#define EN24_DISABLE           	GPIO_ResetBits(EN24_PORT, EN24_PIN);

// OUT CONTROL
#define EN_MDRV_PORT_CLK       	RCC_AHB1Periph_GPIOA
#define EN_MDRV_PORT           	GPIOA
#define EN_MDRV_PIN            	GPIO_Pin_5
       
#define EN_MDRV_ENABLE         	GPIO_SetBits(EN_MDRV_PORT, EN_MDRV_PIN);
#define EN_MDRV_DISABLE        	GPIO_ResetBits(EN_MDRV_PORT, EN_MDRV_PIN);


// POWER SWITCH CHECK
#define POWERKEY_PORT_CLK      	RCC_AHB1Periph_GPIOA
#define POWERKEY_PORT          	GPIOA
#define POWERKEY_PIN           	GPIO_Pin_4
       

#define POWERKEY_IN       			GPIO_ReadInputDataBit(POWERKEY_PORT,POWERKEY_PIN)
// POWER SWITCH CHECK    					RCC_AHB1Periph_GPIOD

#define VOICE_EN_CLK	   				RCC_AHB1Periph_GPIOE
#define VOICE_EN_PORT      			GPIOE
#define VOICE_EN_PIN       			GPIO_Pin_1

#define VOICE_EN_READ       		GPIO_ReadInputDataBit(VOICE_EN_PORT,VOICE_EN_PIN)


#define STOP_KEY_CLK	    			RCC_AHB1Periph_GPIOA
#define STOP_KEY_PORT      			GPIOA
#define STOP_KEY_PIN       			GPIO_Pin_7

#define STOP_KEY_READ       		GPIO_ReadInputDataBit(STOP_KEY_PORT,STOP_KEY_PIN)


// OUT CONTROL
#define EN_DC12V_PORT_CLK      	RCC_AHB1Periph_GPIOE
#define EN_DC12V_PORT          	GPIOE
#define EN_DC12V_PIN           	GPIO_Pin_4
       
#define EN_DC12V_ENABLE        	GPIO_SetBits(EN_DC12V_PORT, EN_DC12V_PIN);
#define EN_DC12V_DISABLE       	GPIO_ResetBits(EN_DC12V_PORT, EN_DC12V_PIN);

// OUT CONTROL
#define PC_EN_PORT_CLK         	RCC_AHB1Periph_GPIOE
#define PC_EN_PORT             	GPIOE
#define PC_EN_PIN              	GPIO_Pin_3
       
#define PC_EN_ENABLE           	GPIO_SetBits(PC_EN_PORT, PC_EN_PIN);
#define PC_EN_DISABLE          	GPIO_ResetBits(PC_EN_PORT, PC_EN_PIN);
//PC机开关GPIO宏定义
//#define PC_STARTUP_CLK        RCC_AHB1Periph_GPIOD
//#define PC_STARTUP_PORT       GPIOD
//#define PC_STARTUP_PIN	      GPIO_Pin_6

#define PC_STARTUP_CLK         	RCC_AHB1Periph_GPIOE
#define PC_STARTUP_PORT        	GPIOE
#define PC_STARTUP_PIN	        GPIO_Pin_2

#define PC_STARTUP_ON    				0
#define PC_STARTUP_OFF   				1

#define PC_STARTUP_HIGH			  	GPIO_SetBits(PC_STARTUP_PORT,PC_STARTUP_PIN);
#define PC_STARTUP_LOW			  	GPIO_ResetBits(PC_STARTUP_PORT,PC_STARTUP_PIN);



/********************************************************
* 红外解码GPIO,定义为外部中断
* PC10 -> IRCODE_BM_PIN 中间红外管解码IO
* PC11 -> IRCODE_BR_PIN 右边红外管解码IO
* PA15 -> IRCODE_BL_PIN 左边红外管解码IO
********************************************************/	
#define IRCODE_PORT_CLK     		RCC_AHB1Periph_GPIOB
#define IRCODE_PORT         		GPIOB

#define IRCODE_BL_PIN       		GPIO_Pin_0
#define IRCODE_BR_PIN       		GPIO_Pin_1

#define EXIT_SOURCE_IRPORT  		EXTI_PortSourceGPIOB

#define EXTI_SOURCE_IRBL    		EXTI_PinSource0
#define EXTI_SOURCE_IRBR    		EXTI_PinSource1

#define EXTI_LINE_IRBL    			EXTI_Line0
#define EXTI_LINE_IRBR    			EXTI_Line1	


//ULTRASONIC
#define ULTRA1_PORT_CLK			  	RCC_AHB1Periph_GPIOD
#define ULTRA1_PORT							GPIOD
#define ECHO1_PIN			    			GPIO_Pin_7
#define TRIG1_PIN			    			GPIO_Pin_6

#define ULTRA2_PORT_CLK			  	RCC_AHB1Periph_GPIOD
#define ULTRA2_PORT							GPIOD
#define ECHO2_PIN				      	GPIO_Pin_5		
#define TRIG2_PIN				      	GPIO_Pin_4

#define ULTRA3_PORT_CLK			  	RCC_AHB1Periph_GPIOE
#define ULTRA3_PORT							GPIOE
#define ECHO3_PIN				      	GPIO_Pin_9
#define TRIG3_PIN				      	GPIO_Pin_8


//#define ULTRA4_PORT_CLK			   RCC_AHB1Periph_GPIOE
//#define ULTRA4_PORT						 GPIOE
//#define ECHO4_PIN				       GPIO_Pin_11
//#define TRIG4_PIN			         GPIO_Pin_10

#define ULTRA4_PORT_CLK			  	RCC_AHB1Periph_GPIOE
#define ULTRA4_PORT							GPIOE
#define ECHO4_PIN				      	GPIO_Pin_11
#define TRIG4_PIN			        	GPIO_Pin_10

#define EXTI_LINE_ECHO1					EXTI_Line7  
#define EXTI_LINE_ECHO2					EXTI_Line5	 
#define EXTI_LINE_ECHO3					EXTI_Line9
#define EXTI_LINE_ECHO4					EXTI_Line11

#define TRIG1_ON			  				digitalHi(ULTRA1_PORT, TRIG1_PIN)
#define TRIG1_OFF			  				digitalLo(ULTRA1_PORT, TRIG1_PIN)

#define TRIG2_ON			  				digitalHi(ULTRA2_PORT, TRIG2_PIN)
#define TRIG2_OFF								digitalLo(ULTRA2_PORT, TRIG2_PIN)

#define TRIG3_ON			  				digitalHi(ULTRA3_PORT, TRIG3_PIN)
#define TRIG3_OFF								digitalLo(ULTRA3_PORT, TRIG3_PIN)

#define TRIG4_ON			  				digitalHi(ULTRA4_PORT, TRIG4_PIN)
#define TRIG4_OFF								digitalLo(ULTRA4_PORT, TRIG4_PIN)


#define ECHO_READ_1       			GPIO_ReadInputDataBit(ULTRA1_PORT,ECHO1_PIN)
#define ECHO_READ_2       			GPIO_ReadInputDataBit(ULTRA2_PORT,ECHO2_PIN)
#define ECHO_READ_3       			GPIO_ReadInputDataBit(ULTRA3_PORT,ECHO3_PIN)
#define ECHO_READ_4       			GPIO_ReadInputDataBit(ULTRA4_PORT,ECHO4_PIN)

/********************************************************
* 碰撞IO初始化
********************************************************/	

#define CRASH_FRONT_PORT_CLK	  RCC_AHB1Periph_GPIOA
#define CRASH_FRONT_PORT       	GPIOA
#define CRASH_FRONT_PIN        	GPIO_Pin_12

#define CRASH_BACK_PORT_CLK	   	RCC_AHB1Periph_GPIOE
#define CRASH_BACK_PORT       	GPIOE
#define CRASH_BACK_PIN        	GPIO_Pin_12

#define CRASH_FRONT_READ       	GPIO_ReadInputDataBit(CRASH_FRONT_PORT,CRASH_FRONT_PIN)
#define CRASH_BACK_READ        	GPIO_ReadInputDataBit(CRASH_BACK_PORT,CRASH_BACK_PIN)

//MOTO_UPDOWN
#define LIFTMOTO_PORT_CLK     	RCC_AHB1Periph_GPIOC
#define LIFTMOTO_PORT						GPIOC

#define LIFTMOTO_EN_PIN					GPIO_Pin_8
#define LIFTMOTO_DIR_PIN				GPIO_Pin_9

//MOTO_UPDOWN
#define LIFTMOTO_PWM_CLK     		RCC_AHB1Periph_GPIOA
#define LIFTMOTO_PWM_PORT				GPIOA

#define LIFTMOTO_PWM_PIN				GPIO_Pin_8


#define LIFTMOTO_ENABLE					GPIO_ResetBits(LIFTMOTO_PORT,LIFTMOTO_EN_PIN);
#define LIFTMOTO_DISABLE				GPIO_SetBits(LIFTMOTO_PORT,LIFTMOTO_EN_PIN);

#define LIFTMOTO_DOWN         	GPIO_ResetBits(LIFTMOTO_PORT,LIFTMOTO_DIR_PIN);
#define LIFTMOTO_UP							GPIO_SetBits(LIFTMOTO_PORT,LIFTMOTO_DIR_PIN);

#define LIFTMOTO_PWM_EN	        GPIO_ResetBits (LIFTMOTO_PWM_PORT,LIFTMOTO_PWM_PIN);
#define LIFTMOTO_PWM_DISABLE	  GPIO_SetBits (LIFTMOTO_PWM_PORT,LIFTMOTO_PWM_PIN);

/********************************************************
* 限位开关检测
********************************************************/	

#define LIMIT_UP_PORT_CLK	 	 		RCC_AHB1Periph_GPIOE
#define LIMIT_UP_PORT       		GPIOE
#define LIMIT_UP_PIN        		GPIO_Pin_13		//上限位

#define LIMIT_DOWN_PORT_CLK	  	RCC_AHB1Periph_GPIOE
#define LIMIT_DOWN_PORT       	GPIOE
#define LIMIT_DOWN_PIN        	GPIO_Pin_14		//下限位

#define LIMIT_UP_READ       		GPIO_ReadInputDataBit(LIMIT_UP_PORT,LIMIT_UP_PIN)
#define LIMIT_DOWN_READ     		GPIO_ReadInputDataBit(LIMIT_DOWN_PORT,LIMIT_DOWN_PIN)


// 电机控制GPIO宏定义
#define MOTO_LEFT_DIR_CLK       RCC_AHB1Periph_GPIOD
#define MOTO_LEFT_DIR_PORT      GPIOD
#define MOTO_LEFT_DIR_PIN       GPIO_Pin_10

// 电机控制GPIO宏定义
#define MOTO_LEFT_PORT_CLK      RCC_AHB1Periph_GPIOD
#define MOTO_LEFT_PORT          GPIOD
#define MOTO_LEFT_EN_PIN       	GPIO_Pin_11
//#define MOTO_LEFT_DIR_PIN      GPIO_Pin_5     // Dir




#define MOTO_RIGHT_PORT_CLK     RCC_AHB1Periph_GPIOD
#define MOTO_RIGHT_PORT         GPIOD
#define MOTO_RIGHT_DIR_PIN      GPIO_Pin_8      // Dir
#define MOTO_RIGHT_EN_PIN       GPIO_Pin_11


#define MOTO_LEFT_EN_ENABLE    	GPIO_ResetBits(MOTO_LEFT_PORT,MOTO_LEFT_EN_PIN);
#define MOTO_LEFT_EN_DISABLE    GPIO_SetBits(MOTO_LEFT_PORT,MOTO_LEFT_EN_PIN);

#define MOTO_RIGHT_EN_ENABLE    GPIO_ResetBits(MOTO_RIGHT_PORT,MOTO_RIGHT_EN_PIN);
#define MOTO_RIGHT_EN_DISABLE   GPIO_SetBits(MOTO_RIGHT_PORT,MOTO_RIGHT_EN_PIN);

#define MOTO_LEFT_DIR_FRONT    	GPIO_SetBits(MOTO_LEFT_DIR_PORT,MOTO_LEFT_DIR_PIN);
#define MOTO_LEFT_DIR_BACK      GPIO_ResetBits(MOTO_LEFT_DIR_PORT,MOTO_LEFT_DIR_PIN);

#define MOTO_RIGHT_DIR_FRONT    GPIO_ResetBits(MOTO_RIGHT_PORT,MOTO_RIGHT_DIR_PIN);
#define MOTO_RIGHT_DIR_BACK     GPIO_SetBits(MOTO_RIGHT_PORT,MOTO_RIGHT_DIR_PIN);



void LED_GPIO_Cfg_Init(void);
void Lamp_GPIO_Cfg_Init(void);
void BEEP_GPIO_Cfg_Init(void);
void PowerSys_Gpio_Cfg_Init(void);
void PC_Power_Cfg_Init(uint8_t on_off);

//void  LiftMoto_Gpio_Cfg_Init(void);
void  Obstacle_GPIO_Init(void);

void Ultrasonic_Exti_Init(void);
void IR_Decoding_GPIO_Cfg_Init(void);
void Crash_Gpio_Cfg_Init(void);
void LiftMoto_Gpio_Cfg_Init(void);
void LimitSwitch_Gpio_Cfg_Init(void);

#ifdef __cplusplus
}
#endif

#endif /*__BSP_SYSTICK_CFG_H */


/************************ (C) COPYRIGHT YzRobot  *****END OF FILE****/


