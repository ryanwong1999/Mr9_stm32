/*=============================================================================
*  Copyright (C) ".2016 " All rights reserved.
*
*  文件名："App_System.h"
*  作者：zz  (zhangzhen123good@163.com)
*  创建时间：2016年07月25日 
*  修改时间：
*  说明：
*
==============================================================================*/


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __INET_H
#define __INET_H

#ifdef __cplusplus
 extern "C" {
#endif 

	 
/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include "stm32f4xx.h"
	 
/*==========================================
   数据大小端转换
============================================
*/
// 短整型大小端互换
#define BigLittleSwap16(A)  ((((uint16_t)(A) & 0xff00) >> 8) | \
                            (((uint16_t)(A) & 0x00ff) << 8))
// 长整型大小端互换	 
#define BigLittleSwap32(A)  ((((uint32_t)(A) & 0xff000000) >> 24) | \
                            (((uint32_t)(A) & 0x00ff0000) >> 8) | \
                            (((uint32_t)(A) & 0x0000ff00) << 8) | \
                            (((uint32_t)(A) & 0x000000ff) << 24))
														
														
int checkCPUendian(void);
unsigned long int t_htonl(unsigned long int h);
unsigned long int t_ntohl(unsigned long int n);
unsigned short int t_htons(unsigned short int h);
unsigned short int t_ntohs(unsigned short int n);


#ifdef __cplusplus
}
#endif

#endif /*__BSP_SYSTICK_CFG_H */


/************************ (C) COPYRIGHT YzRobot  *****END OF FILE****/







