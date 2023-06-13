/*=============================================================================
*  Copyright (C) ".2016 " All rights reserved.
*
*  �ļ�����"App_AnalysisCMD.h"
*  ���ߣ�zz  (zhangzhen123good@163.com)
*  ����ʱ�䣺2016��07��25�� 
*  �޸�ʱ�䣺
*  ˵����
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

#define QUERY_FLASH				0x10     // ��ѯAGV��Ϣ(RFID����,�����־)
#define QUERY_PMS         0x11     // ��ѯ���������Ϣ
#define QUERY_OBS			    0x12     // ��ѯ�ϰ�ʹ��
#define QUERY_ODOM        0x13     // ��ѯODOM
#define QUERY_ENVIRON     0x14     // ��ѯ����
#define QUERY_HEAD        0x15     // ��ѯͷ��
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