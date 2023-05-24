/*=============================================================================
*  Copyright (C) ".2016 " All rights reserved.
*
*  文件名："App_MotoDrv.h"
*  作者：
*  创建时间：
*  修改时间：
*  说明：
*
==============================================================================*/


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __APP_MOTODRV_H
#define __APP_MOTODRV_H

#ifdef __cplusplus
 extern "C" {
#endif 

/* Includes ------------------------------------------------------------------*/
#include "stdbool.h"
#include "stm32f4xx.h"

	 
#define MDRV_SET_SPEED  0xea
#define MDRV_READ  			0x43
#define MDRV_WRITE			0x44
#define MDRV_ALL   			0x45	 
typedef struct {
	
	int16_t set_lear;
	int16_t set_angle;
	int16_t set_left_rpm;
	int16_t set_right_rpm;
	int16_t set_left_pwm;
	int16_t set_right_pwm;
	
	int16_t lear;
	int16_t angle;
	int16_t left_rpm;
	int16_t right_rpm;

	uint16_t left_pos;
	uint16_t right_pos;
	
	int16_t left_pwm;
	int16_t right_pwm;
	
	uint8_t en_sta;
	//uint8_t en_set; 
	
	uint8_t stop_sta;
	uint8_t vcc_sta;
	
	uint16_t cmd_addr1;
	uint16_t cmd_addr2;
	
	bool  over_cur_flag;
}Moto_Drv;	


extern Moto_Drv Moto;

void Moto_mdrv_init(void);
void Moto_mdrv_analysis(void);
bool Get_Drv_OverCur_Flag(uint16_t drv_cur, int16_t bat_cur);
void Get_odom_pulse(int16_t l_pos, int16_t r_pos, int16_t *l_pulse, int16_t *r_pulse);
void Send_read_mdrv_cmd(uint16_t addr1, uint16_t addr2);
void Send_write_mdrv_cmd(uint16_t addr1, uint16_t addr2,int16_t dat1,int16_t dat2);
void Send_wr_all_mdrv_cmd(uint16_t addr1, uint16_t addr2,int16_t set_l,int16_t set_a, int16_t l_pwm, int16_t r_pwm);
void Send_mdrv_en_set(int16_t l_en, int16_t r_en);
void Send_code_disk_query(void);
void Send_speed_set(int16_t set_lear, int16_t set_angle);
void Send_wheel_pwm_set(int16_t l_pwm, int16_t r_pwm);
unsigned short ModBusCRC16(const void *s, int n);

#ifdef __cplusplus
}
#endif

#endif /*__BSP_TIM2_CFG_H */
	 