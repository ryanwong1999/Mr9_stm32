/*=============================================================================
*  Copyright (C) ".2016 " All rights reserved.
*
*  �ļ�����"Bsp_GPIO_Cfg.h"
*  ���ߣ�zz  (zhangzhen123good@163.com)
*  ����ʱ�䣺2016��07��25�� 
*  �޸�ʱ�䣺
*  ˵����
*
==============================================================================*/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __BSP_ADC_CFG_H
#define __BSP_ADC_CFG_H

#ifdef __cplusplus
extern "C" {
#endif 
	 
/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"


#define SAMPLE_NUM 				16						// ÿͨ����16��
#define CHAN_NUM 					6							// Ϊ6��ͨ�� 
	 
/* ���ź궨�� */
#define ADC_CLK						RCC_APB2Periph_ADC1	 
#define ADC_PORT_CLK      RCC_AHB1Periph_GPIOC
#define ADC_PORT          GPIOC
	 
#define VBAT_PIN 	        GPIO_Pin_1   	// ��ص�ѹ
#define STOPSW_PIN        GPIO_Pin_0	  // Emergency stop check
#define VIN_PIN 	        GPIO_Pin_2   	// charging input voltage check   
#define IDRV_PIN      		GPIO_Pin_3   	// ����������
	 
/* ���ź궨�� */
#define ADC1_CLK          RCC_APB2Periph_ADC1	 
#define ADC1_PORT_CLK     RCC_AHB1Periph_GPIOA
#define ADC1_PORT         GPIOA

#define IBAT_PIN      		GPIO_Pin_1   	// battery current
#define ILIFT_PIN      		GPIO_Pin_0   	// ����������

#define ILIFT_CH 					ADC_Channel_0
#define IBAT_CH    				ADC_Channel_1   
#define STOPSW_CH  				ADC_Channel_10 
#define VBAT_CH       		ADC_Channel_11	       
#define VIN_CH     				ADC_Channel_12	      
#define IDRV_CH 					ADC_Channel_13

extern uint16_t ADC_Filter_Value[CHAN_NUM];     // ��Ŵ�������
extern uint16_t AD_Value[SAMPLE_NUM][CHAN_NUM]; // �������ADCת�������Ҳ��DMA��Ŀ���ַ

void ADC_Cfg_Init(void);	 
void filter(void);

#ifdef __cplusplus
}
#endif

#endif /*__BSP_SYSTICK_CFG_H */


/************************ (C) COPYRIGHT YzRobot  *****END OF FILE****/


