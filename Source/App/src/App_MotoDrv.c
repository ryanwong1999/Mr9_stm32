/*=============================================================================
*  Copyright (C) ".strftime("%Y")." All rights reserved.
*
*  文件名："App_MotoDrv.c"
*  作者：hrx
*  创建时间：2022年4月11日 
*  修改时间：
*  说明：电机驱动器通信
*
==============================================================================*/

/* Includes ------------------------------------------------------------------*/
#include "includes.h"

Moto_Drv Moto;

/*=============================================================================
*  函数名 ：Moto_mdrv_init
*  作   者：hrx
*  创建时间：2022年4月11日 
*  修改时间：
*  输   入：
*           
*  输   出：
*  说   明：驱动器参数初始化
*/
void Moto_mdrv_init(void)
{
	Moto.en_sta = 0;
	Moto.lear = 0;
	Moto.angle = 0;
	Moto.left_pos = 0;
	Moto.right_pos = 0;
	Moto.set_angle = 0;
	Moto.set_lear = 0;
	Moto.vcc_sta = 0;
}

/*=============================================================================
*  函数名 ：Moto_mdrv_analysis
*  作   者：hrx
*  创建时间：2022年4月11日 
*  修改时间：
*  输   入：
*           
*  输   出：
*  说   明：驱动器命令解析
*/
void Moto_mdrv_analysis(void)
{
	uint8_t i;
	uint16_t rx_crc;
	uint16_t cal_crc;
	uint8_t cmd;
	uint16_t addr1,addr2;
	int16_t l_pulse_tmp;
	int16_t r_pulse_tmp;
	
	if(UsartToDrv.Usart_Rx_OK == true){
		delay_us(10);
		UsartToDrv.Usart_Rx_OK = false;
		UsartToDrv.Comm_TimeOut = 0;
		UsartToDrv.Disconnect_flag = 0;
		if(UsartToDrv.Rx_Buf[0] == 0x01){		
			if(UsartToDrv.Rx_Len%2 != 0){
				UsartToDrv.Rx_Len--;
			}
		  rx_crc = (uint16_t)UsartToDrv.Rx_Buf[UsartToDrv.Rx_Len - 2] << 8 | UsartToDrv.Rx_Buf[UsartToDrv.Rx_Len - 1];
			cal_crc = ModBusCRC16(UsartToDrv.Rx_Buf,UsartToDrv.Rx_Len - 2);
			cal_crc = rx_crc;	
			//printf("drv_cmd : %02x\r\n",UsartToDrv.Rx_Buf[1]);
			if(cal_crc == rx_crc){
				cmd = UsartToDrv.Rx_Buf[1];
				switch(cmd)
				{
					case MDRV_SET_SPEED:
						Moto.lear = (int16_t)UsartToDrv.Rx_Buf[3]<<8 | UsartToDrv.Rx_Buf[2];
						Moto.angle = (int16_t)UsartToDrv.Rx_Buf[5]<<8 | UsartToDrv.Rx_Buf[4];
				    if(Moto.lear == 0 && Moto.angle == 0){
							Moto.stop_sta = 1;
						}else{
							Moto.stop_sta = 0;
						}
						#ifndef ROBOT_YZ01
						if(UsartToPC.Disconnect_flag == 0 && Robot_Sys.Speed_Timeout_cnt < 2000){
							Send_Speed_reply(1, 0xff, Moto.lear, Moto.angle);
						}
            #endif
					 break;
					
					case MDRV_READ:
						addr1 = (uint16_t)UsartToDrv.Rx_Buf[2]<<8 | UsartToDrv.Rx_Buf[3];
						addr2 = (uint16_t)UsartToDrv.Rx_Buf[4]<<8 | UsartToDrv.Rx_Buf[5];
					
						if(addr1 == 0x5000 && addr2 == 0x5100){
							Moto.left_rpm = (int16_t)UsartToDrv.Rx_Buf[6]<<8 | UsartToDrv.Rx_Buf[7];
							Moto.right_rpm = (int16_t)UsartToDrv.Rx_Buf[8]<<8 | UsartToDrv.Rx_Buf[9];
						}else if(addr1 == 0x5002 && addr2 == 0x5102){
						}else if(addr1 == 0x5004 && addr2 == 0x5104){
							Moto.right_pos= (uint16_t)UsartToDrv.Rx_Buf[6]<<8 | UsartToDrv.Rx_Buf[7];
							Moto.left_pos = (uint16_t)UsartToDrv.Rx_Buf[8]<<8 | UsartToDrv.Rx_Buf[9];
							Get_odom_pulse(Moto.left_pos, 	Moto.right_pos, &Moto_Odom.Left_Value, &Moto_Odom.Right_Value);				
							if(UsartToPC.Disconnect_flag == 0 &&  Robot_Sys.Odom_Timeout_cnt < 5000){
								Send_OdomUpdata(1, 0xff, Moto_Odom );
							}
							Moto_Odom.Left_Value = 0;
							Moto_Odom.Right_Value = 0;
						}
						break;
						
					case MDRV_WRITE:
						addr1 = (uint16_t)UsartToDrv.Rx_Buf[2]<<8 | UsartToDrv.Rx_Buf[3];
					  addr2 = (uint16_t)UsartToDrv.Rx_Buf[4]<<8 | UsartToDrv.Rx_Buf[5];
						if(addr1 == 0x2318 && addr2 == 0x3318){
						  int16_t l_rpm,r_rpm;
							l_rpm = (int16_t)UsartToDrv.Rx_Buf[6]<<8 | UsartToDrv.Rx_Buf[7];
							r_rpm = (int16_t)UsartToDrv.Rx_Buf[8]<<8 | UsartToDrv.Rx_Buf[9];
						}else if(addr1 == 0x2100 && addr2 == 0x3100){
							uint8_t rx_l_en,rx_r_en;
							rx_l_en = UsartToDrv.Rx_Buf[7];
							rx_r_en = UsartToDrv.Rx_Buf[9];
							if(rx_l_en == 1 || rx_r_en == 1){
								Moto.en_sta = 1;
							}else{
							 Moto.en_sta = 0;
							}
							Send_speed_set(Moto.set_lear, Moto.set_angle);
						}
						break;
						
					case MDRV_ALL:
						addr1 = (uint16_t)UsartToDrv.Rx_Buf[2]<<8 | UsartToDrv.Rx_Buf[3];
					  addr2 = (uint16_t)UsartToDrv.Rx_Buf[4]<<8 | UsartToDrv.Rx_Buf[5];
					  if(addr1 == 0x2610 ||  addr1 == 0x2620){
							Moto.lear = (int16_t)UsartToDrv.Rx_Buf[6]<<8 | UsartToDrv.Rx_Buf[7];
							Moto.angle = (int16_t)UsartToDrv.Rx_Buf[8]<<8 | UsartToDrv.Rx_Buf[9];
							
							l_pulse_tmp = (uint16_t)UsartToDrv.Rx_Buf[10]<<8 | UsartToDrv.Rx_Buf[11];
							r_pulse_tmp = (uint16_t)UsartToDrv.Rx_Buf[12]<<8 | UsartToDrv.Rx_Buf[13];
							Moto_Odom.Left_Value += l_pulse_tmp;
							Moto_Odom.Right_Value += r_pulse_tmp;
							
							Moto.left_pwm = (int16_t)UsartToDrv.Rx_Buf[14]<<8 | UsartToDrv.Rx_Buf[15];
							Moto.right_pwm = (int16_t)UsartToDrv.Rx_Buf[16]<<8 | UsartToDrv.Rx_Buf[17];
							#ifndef ROBOT_YZ01	
							if(UsartToPC.Disconnect_flag == 0 && Robot_Sys.Speed_Timeout_cnt < 5000){
								Send_Speed_reply(1, 0xff,Moto.lear, Moto.angle);
							}
							#endif
						}
						break;
				}
			}
		}
		UsartToDrv.Rx_Len = 0;
	}
}


bool Get_Drv_OverCur_Flag(uint16_t drv_cur, int16_t bat_cur)	//过流保护超过40A 3秒
{
	static bool sta = 0;
	static uint8_t over_cnt = 0, over_rst_cnt = 0, stop_flag_cnt = 0, timeout = 0;
	Robot_Sys.Stop_flag = 0;
	//if(drv_cur >= 30000 || bat_cur <= -30000){
	if(drv_cur >= 38000){
		over_cnt ++;
		over_rst_cnt = 0;
		if(over_cnt >= 3){
			stop_flag_cnt++;
			over_cnt = 0;
			sta = true;
		}
	}else{
		if(stop_flag_cnt < 3){
			if(stop_flag_cnt == 2){		//两分钟内如果没有触发第三次就置0
				timeout++;
				if(timeout >= 240){
					stop_flag_cnt = 0;
					timeout = 0;
				}
			}
			over_rst_cnt ++;
			over_cnt = 0;
			if(over_rst_cnt > 15){
				over_rst_cnt = 0;
				sta = false;
			}
		}else{
			timeout++;
			Robot_Sys.Stop_flag = 1;
			if(Robot_Sys.StopDisable_flag == 1 || timeout >= 240){
				stop_flag_cnt = 0;
				sta = false;
				Robot_Sys.Stop_flag = 0;
			}
		}
	}
	
	return sta;
}
/*=============================================================================
*  函数名 ：Get_odom_pulse
*  作   者：hrx
*  创建时间：2022年4月11日 
*  修改时间：
*  输   入：
*           
*  输   出：
*  说   明：计算脉冲数
*/
void Get_odom_pulse(int16_t l_pos, int16_t r_pos, int16_t *l_pulse, int16_t *r_pulse)
{
	static int16_t last_l_pos = 0;
	static int16_t last_r_pos = 0;
	
	int16_t pulse_tmp;
	
	if(l_pos > PULSE_CYCLE * 0.75 && last_l_pos < PULSE_CYCLE * 0.25){
		pulse_tmp = PULSE_CYCLE;
	}else if(l_pos < PULSE_CYCLE * 0.25 && last_l_pos > PULSE_CYCLE * 0.75){
		pulse_tmp = 0 - PULSE_CYCLE;
	}else{
		pulse_tmp = 0;
	}
	*l_pulse = pulse_tmp + last_l_pos - l_pos;
	
	*l_pulse = *l_pulse/4;  
	if(r_pos > PULSE_CYCLE * 0.75 && last_r_pos < PULSE_CYCLE * 0.25){
		pulse_tmp = 0 - PULSE_CYCLE;
	}else if(r_pos < PULSE_CYCLE * 0.25 && last_r_pos > PULSE_CYCLE * 0.75){
		pulse_tmp =  PULSE_CYCLE;
	}else{
		pulse_tmp = 0;
	}
	*r_pulse = pulse_tmp + r_pos - last_r_pos;
	*r_pulse = *r_pulse/4;
	last_l_pos = l_pos;
	last_r_pos = r_pos;
}

/*=============================================================================
*  函数名 ：Send_code_disk_query
*  作   者：hrx
*  创建时间：2022年4月11日 
*  修改时间：
*  输   入：
*           
*  输   出：
*  说   明：查询脉冲位置
*/
void Send_code_disk_query(void)
{
	Send_read_mdrv_cmd(0x5004, 0x5104);
}

/*=============================================================================
*  函数名 ：Send_mdrv_en_set
*  作   者：hrx
*  创建时间：2022年4月11日 
*  修改时间：
*  输   入：
*           
*  输   出：
*  说   明：设置电机使能
*/
void Send_mdrv_en_set(int16_t l_en, int16_t r_en)
{
	Send_write_mdrv_cmd(0x2100, 0x3100,l_en,r_en);
}

/*=============================================================================
*  函数名 ：Send_mdrv_en_set
*  作   者：hrx
*  创建时间：2022年4月11日 
*  修改时间：
*  输   入：
*           
*  输   出：
*  说   明：设置电机使能
*/
void Send_wheel_speed_set(int16_t l_speed, int16_t r_speed)
{
  Send_write_mdrv_cmd(0x2318, 0x3318,r_speed,l_speed);
}

/*=============================================================================
*  函数名 ：Send_wheel_pwm_set
*  作   者：hrx
*  创建时间：2022年4月11日 
*  修改时间：
*  输   入：
*           
*  输   出：
*  说   明：设置电机PWM,只适用于M100自己开发的直流电机驱动器
*/
void Send_wheel_pwm_set(int16_t l_pwm, int16_t r_pwm)
{
  Send_write_mdrv_cmd(0x2600, 0x3600,l_pwm,r_pwm);
}

/*=============================================================================
*  函数名 ：Send_speed_set
*  作   者：hrx
*  创建时间：2022年4月11日 
*  修改时间：
*  输   入：
*           
*  输   出：
*  说   明：设置电机线速度和角速度
*/
void Send_speed_set(int16_t set_lear, int16_t set_angle)
{
	uint8_t i;
  uint16_t crc16_data;
	uint8_t *buf;
	uint8_t sramx=0;					//默认为内部sram
  buf = mymalloc(sramx,10);	//申请2K字节
	
	buf[0] = 0x01;
	buf[1] = 0xEA;
	buf[2] = set_lear ;
	buf[3] = set_lear>>8;
	buf[4] = set_angle;
	buf[5] = set_angle>>8;
	crc16_data = ModBusCRC16(buf,6);
	buf[6] = crc16_data >> 8;
	buf[7] = crc16_data;
	RS485_SendMultibyte(USART_RS485, buf, 8);
	//USARTx_SendMultibyte(USART1, buf, 8);
	myfree(sramx,buf);				//释放内存
}

/*=============================================================================
*  函数名 ：Send_read_mdrv_cmd
*  作   者：hrx
*  创建时间：2022年4月11日 
*  修改时间：
*  输   入：
*           
*  输   出：
*  说   明：读命令
*/
void Send_read_mdrv_cmd(uint16_t addr1, uint16_t addr2)
{
  uint16_t crc16_data;
  uint8_t *buf;
	uint8_t sramx=0;					//默认为内部sram
  buf = mymalloc(sramx,10);	//申请20字节
	
	buf[0] = 0x01;
	buf[1] = 0x43;
	buf[2] = addr1>>8 ;
	buf[3] = addr1;
	buf[4] = addr2>>8;
	buf[5] = addr2;
	crc16_data = ModBusCRC16(buf,6);
	buf[6] = crc16_data >> 8;
	buf[7] = crc16_data;
	
	RS485_SendMultibyte(USART_RS485, buf, 8);
	myfree(sramx,buf);
}

/*=============================================================================
*  函数名 ：Send_wr_all_mdrv_cmd
*  作   者：hrx
*  创建时间：2022年4月11日 
*  修改时间：
*  输   入：
*           
*  输   出：
*  说   明：写命令,只适用于M100自己开发的直流电机驱动器
*/
void Send_wr_all_mdrv_cmd(uint16_t addr1, uint16_t addr2,int16_t set_l,int16_t set_a, int16_t l_pwm, int16_t r_pwm)
{
  uint16_t crc16_data;
	uint8_t *buf;
	uint8_t sramx=0;					//默认为内部sram
  buf = mymalloc(sramx,20);	//申请2K字节
	
	buf[0] = 0x01;
	buf[1] = 0x45;
	buf[2] = addr1>>8 ;
	buf[3] = addr1;
	buf[4] = addr2>>8;
	buf[5] = addr2;
	buf[6] = set_l>>8 ;
	buf[7] = set_l;
	buf[8] = set_a>>8;
	buf[9] = set_a;
	buf[10] = l_pwm>>8 ;
	buf[11] = l_pwm;
	buf[12] = r_pwm>>8;
	buf[13] = r_pwm;
	
	crc16_data = ModBusCRC16(buf,14);
	buf[14] = crc16_data >> 8;
	buf[15] = crc16_data;
	
	RS485_SendMultibyte(USART_RS485, buf, 16);
	myfree(sramx,buf);
}


/*=============================================================================
*  函数名 ：Send_write_mdrv_cmd
*  作   者：hrx
*  创建时间：2022年4月11日 
*  修改时间：
*  输   入：
*           
*  输   出：
*  说   明：写数据命令
*/
void Send_write_mdrv_cmd(uint16_t addr1, uint16_t addr2, int16_t dat1, int16_t dat2)
{
  uint16_t crc16_data;
	uint8_t *buf;
	uint8_t sramx=0;					//默认为内部sram
  buf = mymalloc(sramx,20); //申请2K字节
	
	buf[0] = 0x01;
	buf[1] = 0x44;
	buf[2] = addr1>>8 ;
	buf[3] = addr1;
	buf[4] = addr2>>8;
	buf[5] = addr2;
	buf[6] = dat1>>8 ;
	buf[7] = dat1;
	buf[8] = dat2>>8;
	buf[9] = dat2;
	crc16_data = ModBusCRC16(buf,10);
	buf[10] = crc16_data >> 8;
	buf[11] = crc16_data;
	
	RS485_SendMultibyte(USART_RS485, buf, 12);
	//USARTx_SendMultibyte(USART1, buf, 12);
	myfree(sramx,buf);
}

/*=============================================================================
*  函数名 ：ModBusCRC16
*  作   者：hrx
*  创建时间：2022年4月11日 
*  修改时间：
*  输   入：
*           
*  输   出：
*  说   明：校验
*/
unsigned short ModBusCRC16(const void *s, int n) 
{
	unsigned short c = 0xffff, b;
	char i;
	int k;
	for(k=0; k<n; k++) 
	{  
		b=(((u8 *)s)[k]); 
		for(i=0; i<8; i++) 
		{  
			c = ((b^c)&1) ? (c>>1)^0xA001 : (c>>1); 
			b>>=1; 
		}  
	}  
	return (c<<8)|(c>>8);
}