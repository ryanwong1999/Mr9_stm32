/*=============================================================================
*  Copyright (C) ".2016 " All rights reserved.
*
*  文件名："App_AnalysisCMD.h"
*  作者：zz  (zhangzhen123good@163.com)
*  创建时间：2016年07月25日 
*  修改时间：
*  说明：
*
==============================================================================*/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __APP_TEST_H
#define __APP_TEST_H

#ifdef __cplusplus
extern "C" {
#endif 
/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include "stdbool.h"

#define QUERY_FLASH				0x10     // 查询AGV信息(RFID坐标,到点标志)
#define QUERY_PMS         0x11     // 查询升降电机信息
#define QUERY_OBS			    0x12     // 查询障碍使能
#define QUERY_ODOM        0x13     // 查询ODOM
#define QUERY_ENVIRON     0x14     // 查询环境
#define QUERY_HEAD        0x15     // 查询头部
//#define QUERY_WIFI				0x16
	 
void Test_Cmd_Analysis(void);
void Send_Test_Flash(void);
void Send_Test_PMS(void);
void Send_Test_OBS(void);
void Send_Test_Odom(void);
void Send_Test_Enviro(void);
void Send_Test_Head(void);

#ifdef __cplusplus
}
#endif

#endif /*__BSP_SYSTICK_CFG_H */