/*****************************************************************
* CopyRight : Copyright(c) 1998 - 2015 YZRobot. All Rights Reserved
* FileName  : Bsp_GPIOCfg.h
* Author    : zhangzhen
* Date      : 2015年8月21日
*****************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __APP_EXTI_H
#define __APP_EXTI_H

#ifdef __cplusplus
 extern "C" {
#endif 
  
/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"



/********************************************************
 红外输入状态
********************************************************/
extern uint8_t CH1_IrDa_STA;
extern uint8_t CH1_IrDa_Data;

extern uint8_t CH2_IrDa_STA;
extern uint8_t CH2_IrDa_Data;
extern uint8_t CH3_IrDa_STA;
extern uint8_t CH3_IrDa_Data;

extern uint16_t BaseTimeCount;          // 10us 时间基准计数 

//void  IRInput_GPIO_Cfg_Init(void);			// 红外输入
void IR_Code_Delay10us(void);


void IRObstacle_GPIO_EN_Init(void);
void IRObstacle_GPIO_Cfg_Init(void);
void IRBackward_Decoding_GPIO_Cfg_Init(void);
void IRForward_Decoding_GPIO_Cfg_Init(void);
void Send_Charge_Code(void);

#ifdef __cplusplus
}
#endif
  
#endif


/* CopyRight : Copyright(c) 1998 - 2015 YZRobot. All Rights Reserved */

