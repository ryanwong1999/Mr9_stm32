/*=============================================================================
*  Copyright (C) ".strftime("%Y")." All rights reserved.
*
*  文件名："App_Pms.c"
*  作者：hrx
*  创建时间：2022年4月11日 
*  修改时间：
*  说明：PMS处理
*
==============================================================================*/

/* Includes ------------------------------------------------------------------*/
#include "includes.h"
    
                                                                                                                                                                                                                                                                                        
PMS_Typedef Pms;
/*=============================================================================
*  函数名 ：Read_PMUData
*  作   者：hrx
*  创建时间：2022年4月11日 
*  修改时间：
*  输   入：
*           
*  输   出：
*  说   明：获取PMS数据
*/
void Read_PMUData(void)	
{
	static uint8_t low_cnt = 0;
	Pms.Bat_Voltage = ADC_Filter_Value[0] * 10;   				//iBat_Vol = After_filter[0] *3.3 * 6 * 1000/4096 ,??1mV
	Pms.Input_Voltage = ADC_Filter_Value[2] * 10 + 500;  	//iChg_Vol = After_filter[0] *3.3 * 6 * 1000/4096 mV
	Pms.Moto_Voltage = ADC_Filter_Value[1] * 10;
	Pms.LiftMoto_Cur = ADC_Filter_Value[4] * 3.125;   		//ma
	Pms.Moto_Cur = ADC_Filter_Value[5] * 31.25;      			//ma
	
  if(abs(ADC_Filter_Value[3] - 2000) < 5){
	  Pms.Bat_Current = 0;
	}else{
		Pms.Bat_Current = (ADC_Filter_Value[3] - 2000) * 18.75;		//电池电流
	}
	
	Pms.Capacity = Get_Batt_Capacity(Pms.Bat_Current, Pms.Bat_Voltage);		//计算电池容量
	
	if(Pms.Capacity <= 20){		//电量低
		Pms.Bat_Sta |= 0x10;
	}else{
		Pms.Bat_Sta &= 0xef;		//1110 1111
	}
	 
	
// if(Pms.Moto_Voltage < 10000 ){
//	 Robot_Sys.Mergency_Stop_flag = true;
//	 Moto.en_sta = 0;
//	 //Robot_Sys.Mergency_Stop_flag = false;
// }else{
//	 Robot_Sys.Mergency_Stop_flag = false;
// }   


  if(Pms.Bat_Voltage < EMPTY_VOL){
		low_cnt ++;
		if(low_cnt > 30){
			Pms.Low_Flag = 1;
		}
	}else{
		low_cnt = 0;
		Pms.Low_Flag = 0;
	}

}

/*=============================================================================
*  函数名 ：Get_Batt_Capacity
*  作   者：hrx
*  创建时间：2022年4月11日 
*  修改时间：
*  输   入：电池电流、电池电压
*           
*  输   出：电池容量
*  说   明：计算电池容量
*/
uint8_t Get_Batt_Capacity(int16_t _bat_cur,uint16_t _bat_vol)
{
	static uint32_t voltage_sum = 0;
	static int32_t current_sum = 0;
	static int8_t voltage_cnt = 0;
	static uint32_t vol_tmp;
	static int32_t cur_tmp;
	static uint8_t first = 0;
	static uint8_t  min_cnt = 0;
	static uint32_t batt_cap;
	
	if(first == 0){		//初次进入
		first = 1;
		batt_cap = Pms.Capacity_mah /(Pms.Capacity_Set/100);
	}
	voltage_cnt ++;     //1秒1次
	voltage_sum += _bat_vol;
	current_sum += _bat_cur;
	if(voltage_cnt >= 60){
		vol_tmp = voltage_sum/60;		//1分钟的平均值
	  cur_tmp = current_sum/60;		//1分钟的平均值
		
		if(cur_tmp < 0 && Pms.Capacity_mah < abs(cur_tmp/60)){
			Pms.Capacity_mah = 0;
		}else{
			Pms.Capacity_mah += cur_tmp/60;
		}
		
		if(vol_tmp <= 25040 && (Pms.Bat_Sta & 0x11) != 0){		//判断开始充电的时候电压低于20%，直到充满
			Pms.Reset_Flag = 1;		//自动校准容量标志位
		}

		if(vol_tmp < EMPTY_VOL && (Pms.Bat_Sta & 0x01) == 0){		//非充电中没电了
			Pms.Capacity_mah = EMPTY_CAP;
		}else if(vol_tmp > FULL_VOL && (Pms.Bat_Sta & 0x01) != 0 && cur_tmp < 800){		//充电中满电了
			if(Pms.Reset_Flag == 1 && Pms.Capacity_mah <= Pms.Capacity_Set && Pms.Capacity_mah >= 10000){
				Pms.Capacity_Set = Pms.Capacity_mah;
				//将校准的值写at24c02
				AT24CXX_WriteOneByte(2,(Pms.Capacity_Set>>8) & 0x00ff);
				AT24CXX_WriteOneByte(3,Pms.Capacity_Set& 0x00ff);
			}else{
				Pms.Capacity_mah = Pms.Capacity_Set;
			}
			Pms.Reset_Flag = 0;
		}else if(Pms.Capacity_mah >= Pms.Capacity_Set){
			Pms.Capacity_mah = Pms.Capacity_Set;
		}else if(Pms.Capacity_mah <= EMPTY_CAP){		//没电了
			Pms.Capacity_mah = EMPTY_CAP;
		}else {
			Pms.Capacity_mah = Pms.Capacity_mah;
		}
		
		if(vol_tmp < FULL_VOL && (Pms.Bat_Sta & 0x01) == 0){		//没充满就走了，本次校准失败
				Pms.Reset_Flag = 0;
		}
		
		batt_cap = Pms.Capacity_mah /(Pms.Capacity_Set/100);
		
		
		#ifdef FELI8S
		if(batt_cap > 20 && vol_tmp <= 25040 && cur_tmp >= -2000){		//根据电压校准电池，放电电流小于2A
			Pms.Capacity_mah = 2400;
			batt_cap = 20;
		}
		if(batt_cap > 5 && vol_tmp <= 22400 && cur_tmp >= -2000){
			Pms.Capacity_mah = 600;
			batt_cap = 5;
		}
		#endif
		
//		min_cnt ++;
//		if(min_cnt > 30){		//半分钟把电量写一次
//			min_cnt = 0;
//			AT24CXX_WriteOneByte(4,(Pms.Capacity_mah>>8) & 0x00ff);	//当前容量
//			AT24CXX_WriteOneByte(5,Pms.Capacity_mah& 0x00ff);
//		}
		voltage_sum = 0;
		voltage_cnt = 0;
		current_sum = 0;
	}
	return (uint8_t)batt_cap;
}

/*=============================================================================
*  函数名 ：Capacity_Init
*  作   者：hrx
*  创建时间：2022年4月11日 
*  修改时间：
*  输   入：
*           
*  输   出：
*  说   明：电池容量初始化
*/
void Capacity_Init(void)
{
	Pms.Bat_Voltage =  ADC_Filter_Value[0] * 10;
	#ifdef FELI8S
	if(Pms.Bat_Voltage < 24400){
		Pms.Capacity = 5;
	}else if(Pms.Bat_Voltage < 25040){
		Pms.Capacity = 20;
	}else if(Pms.Bat_Voltage < 26000){
		Pms.Capacity = 50;
	}else if(Pms.Bat_Voltage < 27000){
		Pms.Capacity = 75; 
	}else{
		Pms.Capacity = 90;
	}
	#elif FELI5S
	if(Pms.Bat_Voltage <  15500){
		Pms.Capacity = 20;
	}else if(Pms.Bat_Voltage < 16500){
		Pms.Capacity = 50;
	}else if(Pms.Bat_Voltage < 16800){
		Pms.Capacity = 75; 
	}else{
		Pms.Capacity = 90;
	}
  #endif
}




/* CopyRight : Copyright(c) 1998 - 2015 YZRobot. All Rights Reserved */

