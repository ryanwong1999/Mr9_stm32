/*=============================================================================
*  Copyright (C) ".strftime("%Y")." All rights reserved.
*
*  文件名："Bsp_GPIO_Cfg.c"
*  作者：hrx
*  创建时间：2022年3月5日 
*  修改时间：
*  说明：ADC初始化及ADC中断处理
*
==============================================================================*/

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include "Bsp_ADC_Cfg.h"
#include "includes.h"


uint16_t ADC_Filter_Value[CHAN_NUM];      //存放处理后的数

uint16_t AD_Value[SAMPLE_NUM][CHAN_NUM]; //用来存放ADC转换结果，也是DMA的目标地址


/*=============================================================================
*  函数名 ：ADC_GOIO_Cfg_Init
*  作   者：hrx
*  创建时间：2022年3月5日 
*  修改时间：
*  输   入：
*          
*  输   出：
*  说   明：ADC GPIO初始化配置
*/

static void  ADC_GOIO_Cfg_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
	
  // ??ADC ???
	RCC_AHB1PeriphClockCmd( ADC_PORT_CLK , ENABLE);
	
  GPIO_InitStructure.GPIO_Pin = VIN_PIN | VBAT_PIN  | STOPSW_PIN | IDRV_PIN;	 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;	                      //ADC模式必须是模拟输入
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;                   //不带上下拉
	GPIO_Init(ADC_PORT, &GPIO_InitStructure);  
	
	RCC_AHB1PeriphClockCmd( ADC1_PORT_CLK , ENABLE);
	
  GPIO_InitStructure.GPIO_Pin = IBAT_PIN | ILIFT_PIN;	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;	                     
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;                  
	GPIO_Init(ADC1_PORT, &GPIO_InitStructure);  
	
}


/*=============================================================================
*  函数名 ：ADC_DMA_Cfg_Init
*  作   者：hrx
*  创建时间：2022年3月5日 
*  修改时间：
*  输   入：
*          
*  输   出：
*  说   明：ADC DMA初始化配置
*/

void  ADC_DMA_Cfg_Init(void)
{
	DMA_InitTypeDef DMA_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2,ENABLE);    //DMA2时钟使能
		
	while(DMA_GetCmdStatus(DMA2_Stream0)!=DISABLE){;}       //等待DMA可以配置

  DMA_InitStructure.DMA_Channel = DMA_Channel_0;                              	//DMA通道0 
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&ADC1->DR;            	 	//外设地址
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&AD_Value;            			//存取器地址  
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;               				//方向从外设到内存     
  DMA_InitStructure.DMA_BufferSize = SAMPLE_NUM*CHAN_NUM;   										//数据传输的数量
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;   						//地址不增加
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;                     	//地址增加
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;  	//外设数据长度半字,16位
  DMA_InitStructure.DMA_MemoryDataSize = DMA_PeripheralDataSize_HalfWord;      	//存储长度半字 16位
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;  															//若用中断,则用一般模式,若不用中断,则循环模式
	
  DMA_InitStructure.DMA_Priority = DMA_Priority_High; 									//优先级
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;    						//禁止FIFO    
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;    	//FIFO的值
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;   				//单次传输
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single; 	//单次传输
  DMA_Init(DMA2_Stream0, &DMA_InitStructure); 
	
  DMA_Cmd(DMA2_Stream0, ENABLE); 								//DMA2_Stream0 enable 
  DMA_ClearFlag(DMA2_Stream0,DMA_IT_TC);    		//清中断标志位
  DMA_ITConfig(DMA2_Stream0,DMA_IT_TC,ENABLE); 	//使能中断
}


/*=============================================================================
*  函数名 ：ADC_Cfg_Init
*  作   者：hrx
*  创建时间：2022年3月5日 
*  修改时间：
*  输   入：
*          
*  输   出：
*  说   明：ADC初始化配置
*/

void  ADC_Cfg_Init(void)
{
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	ADC_InitTypeDef       ADC_InitStructure;

	RCC_APB2PeriphClockCmd(ADC_CLK, ENABLE);  
	
	ADC_GOIO_Cfg_Init();            // 配置ADC_GPIO
  ADC_DMA_Cfg_Init();             // 配置ADC DMA
	 
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,ENABLE);	   
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,DISABLE);		 
	
	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;        	// DMA失能
	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;              					// 独立模式
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;               			// APB2的4分频 即84/4=21M
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;  	// 两个采样阶段的延时5个时钟
	ADC_CommonInit(&ADC_CommonInitStructure);                                      	// 初始化

	ADC_InitStructure.ADC_Resolution  = ADC_Resolution_12b;                       	// 12位模式
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;                                  	// 扫描模式
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;                            	// 连续转换
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;   	// 禁止触发检测 使用软件触发
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;                      		// 右对齐
	ADC_InitStructure.ADC_NbrOfConversion = CHAN_NUM;                               // 2通道 1
	ADC_Init(ADC1,&ADC_InitStructure);                                            	// 初始化
 
  ADC_Cmd(ADC1, ENABLE);   //使能ADC传输
	 
	ADC_RegularChannelConfig(ADC1, VBAT_CH , 1, ADC_SampleTime_144Cycles);
	ADC_RegularChannelConfig(ADC1, STOPSW_CH, 2, ADC_SampleTime_144Cycles);
	ADC_RegularChannelConfig(ADC1, VIN_CH , 3, ADC_SampleTime_144Cycles);
	ADC_RegularChannelConfig(ADC1, IBAT_CH , 4, ADC_SampleTime_144Cycles);
	ADC_RegularChannelConfig(ADC1, ILIFT_CH, 5, ADC_SampleTime_144Cycles);
	ADC_RegularChannelConfig(ADC1, IDRV_CH, 6, ADC_SampleTime_144Cycles);
	
	ADC_SoftwareStartConv(ADC1);    //启动软件转换
	ADC_DMARequestAfterLastTransferCmd(ADC1,ENABLE); //源数据变化时开启DMA传输
  ADC_DMACmd(ADC1, ENABLE);   //使能ADC DMA传输
	
}


/*=============================================================================
*  函数名 ：filter
*  作   者：hrx
*  创建时间：2022年3月5日 
*  修改时间：
*  输   入：
*          
*  输   出：
*  说   明：ADC采样值处理
*/
void filter(void)
{
	static u32 sum = 0;
  u8 ch_num = 0;    		// 通道数
  u8 ave_num;        		// 采样平均数
	
	for(ch_num=0;ch_num<CHAN_NUM;ch_num++)
	{
		for (ave_num=0;ave_num<SAMPLE_NUM;ave_num++)
    {
      sum += AD_Value[ave_num][ch_num];
    }

		ADC_Filter_Value[ch_num]=sum/SAMPLE_NUM;   
    sum=0;
	}

}


/*=============================================================================
*  函数名 ：DMA2_Stream0_IRQHandler
*  作   者：hrx
*  创建时间：2022年3月5日 
*  修改时间：
*  输   入：
*          
*  输   出：
*  说   明：ADC DMA中断值处理
*/
void DMA2_Stream0_IRQHandler(void) 
{

	#if SYSTEM_SUPPORT_OS  //使用UCOS操作系统
	OSIntEnter();    
	#endif
	
	if(DMA_GetITStatus(DMA2_Stream0, DMA_IT_TCIF0))
  {
     DMA_ClearITPendingBit(DMA2_Stream0, DMA_IT_TCIF0);
     filter();
  } 
 #if SYSTEM_SUPPORT_OS  
	OSIntExit();    	
	#endif
}
