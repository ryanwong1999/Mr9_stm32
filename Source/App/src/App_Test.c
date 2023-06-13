/*=============================================================================
*  Copyright (C) ".strftime("%Y")." All rights reserved.
*
*  文件名："Test.c"
*  作者：hrx
*  创建时间：2021年03月8日 
*  修改时间：
*  说明：测试
*
==============================================================================*/

/* Includes ------------------------------------------------------------------*/

#include "includes.h"


/*=============================================================================
*  函数名 ：Test_Cmd_Analysis
*  创建时间：2016年07月25日
*  修改时间：
*  输   入：
*  输   出：
*  说   明：解析命令
*  命令举例(16进制):  55 AA 01 11 10 08 00 00 00 00 00 00 00 00 3A 0D 0A
     帧头(固定): 55 AA
		 本机地址(固定): 01
		 长度(可变): 11
	   命令(可变): 10
		 数据长度(可变): 08
		 数据(可变): 00 00 00 00 00 00 00 00 
		 校验(可变): 3A
		 帧尾(固定): 0D 0A

*/
void Test_Cmd_Analysis(void)
{
	uint8_t rx_cmd;
	if(UsartToTest.Usart_Rx_OK == true)
	{
		UsartToTest.Usart_Rx_OK = false;
		rx_cmd = UsartToTest.Rx_Buf[4];
		switch(rx_cmd)
		{
			case QUERY_FLASH :
				Send_Test_Flash();
				break;
			
			case QUERY_PMS:
				Send_Test_PMS();
				break;
			
			case QUERY_OBS:
				Send_Test_OBS();
				break;
			
			case QUERY_ODOM:
				Send_Test_Odom();
				break;
			
			case QUERY_ENVIRON:
				Send_Test_Enviro();
				break;
			case QUERY_HEAD:
				Send_Test_Head();
				break;

			default:
				break;
		}
	}
}

void Send_Test_Flash(void)
{
	uint8_t *buf;
	uint8_t sramx=0;						// 默认为内部sram
  buf = mymalloc(sramx, 20);	// 申请2K字节
	
	buf[0] = 0x55;  	// head
	buf[1] = 0xAA;  	// head
	buf[2] = 0x01;  	// id
	buf[3] = 0x11;  	// len
	buf[4] = 0x10;		// cmd
	buf[5] = 0x08;		// data len
	buf[6] = (Pms.Capacity_Set >> 8) & 0x00ff;
	buf[7] = Pms.Capacity_Set & 0x00ff;  
	
	buf[8] =(Pms.Capacity_mah >> 8) & 0x00ff;
	buf[9] = Pms.Capacity_mah & 0x00ff; 
	buf[10] = Robot_Sys.Ultra_Disable_Flag; 
	buf[11] = Robot_Sys.IR_Bottom_Disable_Flag; 
	
	buf[12] = Head_Status.Level_Offset;
	buf[13] = Head_Status.Pitch_Offset;
	
	buf[14] = CRC8_Table(buf, 14);   // check
	buf[15] = 0x0D;		// end
	buf[16] = 0x0A;		// end
	
	USARTx_SendMultibyte(USART1, buf, buf[3]);
	myfree(sramx, buf);
}


void Send_Test_PMS(void)
{
	uint8_t *buf;
	uint8_t sramx=0;						// 默认为内部sram
  buf = mymalloc(sramx, 24);	// 申请2K字节
	
	buf[0] = 0x55;    // head
	buf[1] = 0xAA;    // head
	buf[2] = 0x01;    // id
	buf[3] = 0x15;    // len
	buf[4] = 0x11;   	// cmd
	buf[5] = 0x0C;		// data len
	buf[6] = (Pms.Bat_Voltage >> 8) & 0x00ff;
	buf[7] = Pms.Bat_Voltage & 0x00ff;
	
	buf[8] = (Pms.Bat_Current >> 8) & 0x00ff;
	buf[9] = Pms.Bat_Current & 0x00ff; 
	
	buf[10] = (Pms.Input_Voltage >> 8) & 0x00ff;
	buf[11] =	Pms.Input_Voltage & 0x00ff;
	
	buf[12]	= (Pms.Moto_Voltage >> 8) & 0x00ff;
	buf[13]	= Pms.Moto_Voltage  & 0x00ff;
	
	buf[14]	= (Pms.Capacity_mah >> 8) & 0x00ff;
	buf[15]	= Pms.Capacity_mah& 0x00ff;
	buf[16]	= Pms.Capacity;
	buf[17]	= Pms.Bat_Sta;

	buf[18]	= CRC8_Table(buf, 18);   // check
	buf[19]	= 0x0D;   // end
	buf[20]	= 0x0A;		// end
	
	USARTx_SendMultibyte(USART1, buf, buf[3]);
  myfree(sramx, buf);
}


void Send_Test_OBS(void)
{
	static uint8_t ultra[4];
	uint8_t *buf;
	uint8_t sramx=0;						// 默认为内部sram
  buf = mymalloc(sramx, 20);	// 申请2K字节
	
	ultra[0] = Ultra1.Distance/10;
	ultra[1] = Ultra2.Distance/10;
	ultra[2] = Ultra3.Distance/10;
	ultra[3] = Ultra4.Distance/10;

	buf[0] = 0x55;		// head
	buf[1] = 0xAA;		// head
	buf[2] = 0x01;		// id
	buf[3] = 0x13;		// len
	buf[4] = 0x12;		// cmd
	buf[5] = 0x0c;		// data len
	
	buf[6] = ultra[0];
	buf[7] = ultra[1];
	
	buf[8] = ultra[2];
	buf[9] = ultra[3]; 
	
	buf[10] = 0;
	buf[11] = 0; 
	
	buf[12] = Robot_Sys.Ultra_sta;
	buf[13] = Robot_Sys.Crash_Flag;
	
	buf[14] = 0;
	buf[15] = 0;

	buf[16] = CRC8_Table(buf, 18);   // check
	buf[17] = 0x0D;   // end
	buf[18] = 0x0A;		// end
	
	USARTx_SendMultibyte(USART1, buf, buf[3]);
	myfree(sramx, buf);
}


void Send_Test_Odom(void)
{
	int16_t lear_set;
	int16_t angle_set;
	uint8_t *buf;
	uint8_t sramx = 0;					// 默认为内部sram
  buf = mymalloc(sramx, 30);	// 申请2K字节
	
	lear_set = 	Robot_Moto.Lear * 1000;
	angle_set = Robot_Moto.Angular * 1000;
	
	buf[0] = 0x55;		// head
	buf[1] = 0xAA;		// head
	buf[2] = 0x01;		// id
	buf[3] = 0x1D;		// len
	buf[4] = 0x13;   	// cmd
	buf[5] = 0x14;		// data len
	
	buf[6] = (lear_set>> 8) & 0x00ff;
	buf[7] = lear_set & 0x00ff;
	
	buf[8] =(angle_set >> 8) & 0x00ff;
	buf[9] = angle_set & 0x00ff; 
	
	buf[10] = (Robot_Moto.Real_Lear >> 8) & 0x00ff;
	buf[11] = Robot_Moto.Real_Lear & 0x00ff;
	
	buf[12] = (Robot_Moto.Real_Angular>> 8) & 0x00ff;
	buf[13] = Robot_Moto.Real_Angular  & 0x00ff;
	
	buf[14] = (Robot_Moto.Left_Period_Set>> 8) & 0x00ff;
	buf[15] =  Robot_Moto.Left_Period_Set & 0x00ff;
	buf[16] = (Robot_Moto.Right_Period_Set>> 8) & 0x00ff;
	buf[17] = Robot_Moto.Right_Period_Set & 0x00ff;

	buf[18] = (Robot_Moto.Left_Period>> 8) & 0x00ff;
	buf[19] =  Robot_Moto.Left_Period & 0x00ff;
	buf[20] = (Robot_Moto.Right_Period>> 8) & 0x00ff;
	buf[21] = Robot_Moto.Right_Period & 0x00ff;
	
	buf[22] = (Moto_Odom.Left_Value>> 8) & 0x00ff;
	buf[23] =  Moto_Odom.Left_Value & 0x00ff;
	buf[24] = (Moto_Odom.Right_Value>> 8) & 0x00ff;
	buf[25] = Moto_Odom.Right_Value & 0x00ff;
	
	buf[26] = CRC8_Table(buf, 22);   // check
	buf[27] = 0x0D;		// end
	buf[28] = 0x0A;   // end
	
	USARTx_SendMultibyte(USART1, buf, buf[3]);
  myfree(sramx, buf);
}


void Send_Test_Enviro(void)
{
	uint8_t *buf;
	uint8_t sramx = 0;					// 默认为内部sram
  buf = mymalloc(sramx, 30);	// 申请2K字节
	
	buf[0] = 0x55;		// head
	buf[1] = 0xAA;		// head
	buf[2] = 0x01;		// id
	buf[3] = 0x1B;		// len
	buf[4] = 0x14;   	// cmd
	buf[5] = 0x12;		// data len
	buf[6] = (Environ.CO2>> 8) & 0x00ff;
	buf[7] = Environ.CO2 & 0x00ff;
	
	buf[8] = (Environ.VOC >> 8) & 0x00ff;
	buf[9] = Environ.VOC & 0x00ff; 
	
	buf[10] = (Environ.Humi >> 8) & 0x00ff;
	buf[11] = Environ.Humi & 0x00ff;
	
	buf[12] = (Environ.Tempera>> 8) & 0x00ff;
	buf[13] = Environ.Tempera  & 0x00ff;
	
	buf[14] = (Environ.PM2_5>> 8) & 0x00ff;
	buf[15] = Environ.PM2_5 & 0x00ff;
	buf[16] = (Environ.PM10>> 8) & 0x00ff;
	buf[17] = Environ.PM10 & 0x00ff;

	buf[18] = (Environ.PM1_0>> 8) & 0x00ff;
	buf[19] =  Environ.PM1_0 & 0x00ff;
	buf[20] = (Environ.state>> 8) & 0x00ff;
	buf[21] = Environ.state & 0x00ff;
	buf[22] = (Environ.voice>> 8) & 0x00ff;
	buf[23] = Environ.voice & 0x00ff;
	buf[24] = CRC8_Table(buf, 24);   // check
	buf[25] = 0x0D;   // end
	buf[26] = 0x0A;   // end
	
	USARTx_SendMultibyte(USART1, buf, buf[3]);
  myfree(sramx,buf);
}


void Send_Test_Head(void)
{
	int16_t level_tmp;
	int16_t pitch_tmp;
	uint8_t *buf;
	uint8_t sramx = 0;					// 默认为内部sram
  buf = mymalloc(sramx, 20);	// 申请2K字节
	
	level_tmp = Head_Status.PSC_Level_Pos * 10;
	pitch_tmp = Head_Status.PSC_UD_Pos * 10;
	
	buf[0] = 0x55;		// head
	buf[1] = 0xAA;		// head
	buf[2] = 0x01;		// id
	buf[3] = 0x0F;		// len
	buf[4] = 0x11;   	// cmd
	buf[5] = 0x04;		// data len
	buf[6] = (level_tmp>> 8) & 0x00ff;
	buf[7] = 	level_tmp & 0x00ff;
	
	buf[8] =(pitch_tmp >> 8) & 0x00ff;
	buf[9] = pitch_tmp & 0x00ff; 
	
	buf[10] = Head_Status.PSC_Limit_Flag;
	buf[11] = Head_Status.PSC_Pitch_OK_Flag; 
	
	buf[12] = CRC8_Table(buf, 14);   // check
	buf[13] = 0x0D;   // end
	buf[14] = 0x0A;   // end
	
	USARTx_SendMultibyte(USART1, buf, buf[3]);
	myfree(sramx, buf);
}