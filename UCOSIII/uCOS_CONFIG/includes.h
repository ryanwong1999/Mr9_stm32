/*
*********************************************************************************************************
*                                              EXAMPLE CODE
*
*                             (c) Copyright 2013; Micrium, Inc.; Weston, FL
*
*               All rights reserved.  Protected by international copyright laws.
*               Knowledge of the source code may NOT be used to develop a similar product.
*               Please help us continue to provide the Embedded community with the finest
*               software available.  Your honesty is greatly appreciated.
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*
*                                           MASTER INCLUDES
*
*                                       IAR Development Kits
*                                              on the
*
*                                    STM32F429II-SK KICKSTART KIT
*
* Filename      : includes.h
* Version       : V1.00
* Programmer(s) : FT
*********************************************************************************************************
*/

#ifndef  INCLUDES_MODULES_PRESENT
#define  INCLUDES_MODULES_PRESENT


/*
*********************************************************************************************************
*                                         STANDARD LIBRARIES
*********************************************************************************************************
*/
#include  <stdio.h>
#include  <string.h>
#include  <ctype.h>
#include  <stdlib.h>
#include  <stdarg.h>
#include  <math.h>


/*
*********************************************************************************************************
*                                                 OS
*********************************************************************************************************
*/
#include  <os.h>


/*
*********************************************************************************************************
*                                              LIBRARIES
*********************************************************************************************************
*/
#include  <cpu.h>
#include  <lib_def.h>
#include  <lib_ascii.h>
#include  <lib_math.h>
#include  <lib_mem.h>
#include  <lib_str.h>


/*
*********************************************************************************************************
*                                            UCOSIII  APP / BSP
*********************************************************************************************************
*/
#include  <app_cfg.h>
#include  <bsp.h>
//#include  <bsp_int.h>


/*
*********************************************************************************************************
*                                            STM32  SYSTEM
*********************************************************************************************************
*/
#include "delay.h"
#include "sys.h"
#include "usart.h"	
#include "malloc.h"	 


/*
*********************************************************************************************************
*                                            STM32  BSP
*********************************************************************************************************
*/
#include "Bsp_GPIO_Cfg.h"
#include "Bsp_ADC_Cfg.h"
#include "Bsp_CAP_Cfg.h"
#include "Bsp_PWM_Cfg.h"
#include "Bsp_TIM_Cfg.h"
#include "Bsp_USART_Cfg.h"
#include "Bsp_RS485_Cfg.h"
#include "Bsp_IIC.h"
#include "Bsp_SPI.h"
#include "Bsp_iwdg.h"
//#include "Bsp_esp8266.h"


/*
*********************************************************************************************************
*                                            STM32  APP
*********************************************************************************************************
*/
#include "App_Task.h"
#include "App_TIM.h"
#include "App_Pms.h"
#include "App_System.h"
#include "App_MotoCtrl.h"
#include "App_AnalysisCMD.h"
#include "App_Ultrasonic.h"
#include "App_Exti.h"
#include "App_AutoCharge.h"
#include "App_Pms.h"
#include "App_Head_Done.h"
#include "App_MotoCtrl.h"
#include "App_USART.h"
#include "App_Cap.h"
#include "App_LiftMoto.h"
#include "App_Environmental.h"
#include "App_24cxx.h" 
#include "App_PID.h"
#include "App_Test.h"
#include "App_MotoDrv.h"
//#include "App_HeadCtrl.h"
//#include "App_Rs485Bus.h"
//#include "Speed_Governing.h"
//#include "App_Key.h"
//#include "App_Obs.h"
//#include "App_ReadGyro.h"
//#include "App_w25qxx.h" 
//#include "App_esp8266.h"


#endif
