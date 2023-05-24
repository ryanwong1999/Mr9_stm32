/*=============================================================================
*  Copyright (C) ".strftime("%Y")." All rights reserved.
*
*  文件名："App_AnalysisCMD.c"
*  作者：hrx
*  创建时间：2022年4月11日 
*  修改时间：
*  说明：工控机命令解析
*
==============================================================================*/

/* Includes ------------------------------------------------------------------*/

#include "includes.h"

Sys_Type   			Robot_Sys;
Odom_Data_type  Moto_Odom;    // 里程数据
Moto_Typedef  	Robot_Moto;

void Send_Head_Pose(uint8_t index, uint8_t addr, HeadPose_Type mHead_Pose, bool stop_key);
void Send_PowerDataUpdata(uint8_t index, uint8_t addr, Power_Type mPower);
void Send_LiftMoto_Mess(uint8_t index, uint8_t addr,LiftMoto_Type *_liftmoto);

uint8_t gUpdateCnt = 0;
uint8_t gPscCnt = 0;


/*=============================================================================
*  函数名 ：AnalysisCMD
*  作   者：hrx
*  创建时间：2022年4月11日 
*  修改时间：
*  输   入：
*  输   出：
*  说   明：工控机命令解析
*/ 
void AnalysisCMD(void)
{
	int level;
	int pitch;
	uint8_t i=0;
	int16_t lear_tmp;
	int16_t angular_tmp;
	int16_t real_lear;
	int16_t real_angle;
	float pulse_width_r;
	float pulse_width_l;
	static float left_speed_tmp = 0;
	static float right_speed_tmp = 0;
	static uint16_t left_inc_pwm = 0;
	static uint16_t right_inc_pwm = 0;
	static uint16_t peroid_tmp = 0;
	static uint8_t period_set_sta = 0;
	float  period_inc_tmp;
	uint16_t addr1;
	uint16_t addr2;
	uint8_t charge_tmp = 0;
	uint8_t cmd_tmp ;
	uint8_t Sdev_tmp ;  // 源地址
	uint8_t Pdev_tmp ;  // 目地址	
	float l_speed;
	float r_speed;
	float angle_div;
	int16_t rx_angle;
	int16_t rx_lear;
	int16_t chg_dis;
	int16_t chg_angle;
		
	if(UsartToPC.Usart_Rx_OK == true)
	{
		UsartToPC.Usart_Rx_OK = false ;					//清空等待下一次接收完成
		UsartToPC.Comm_TimeOut = 0;
		UsartToPC.Disconnect_flag = 0;	
		Sdev_tmp = UsartToPC.Rx_Buf[S_ID_REG];	//取源地址
		Pdev_tmp = UsartToPC.Rx_Buf[P_ID_REG];  //取目的地址,与本地地址匹配
			
		if(Pdev_tmp == DEV_ID){
			cmd_tmp = UsartToPC.Rx_Buf[CMD_REG];
			switch(cmd_tmp)
			{
				case CMD_QUERY_ODOM:   //查询ODOM
					Robot_Sys.Odom_Timeout_cnt = 0;
					if(Robot_Sys.Mergency_Stop_flag == true){
						Moto_Odom.Left_Value = 0;
						Moto_Odom.Right_Value = 0;
						Send_OdomUpdata(1, 0xff, Moto_Odom);
					}else{
						Send_code_disk_query();
					}
					break;

        case CMD_SET_SPEED:   //设置速度	
					Robot_Sys.Speed_Timeout_cnt = 0;
				  memcpy(&lear_tmp, &UsartToPC.Rx_Buf[SPEED1_REG1], sizeof(int16_t));
					memcpy(&angular_tmp, &UsartToPC.Rx_Buf[SPEED2_REG1], sizeof(int16_t));
					rx_lear = (short int)t_ntohs(lear_tmp);
					rx_angle = (short int)t_ntohs(angular_tmp);
				 
					if(rx_lear > 1500){
						rx_lear = 1500;
					}else if(rx_lear < -1500){
						rx_lear = -1500;
					}
					
					if(rx_lear != 0 || rx_angle != 0){
						Robot_Sys.AutoCharge_task_flag = false;
						AutoCharge.AutoChg_Cmd = false;
						Robot_Sys.Remote_flag = true;
						Robot_Sys.Set_PWM_Task_Flag = false;
						Robot_Sys.Last_Task = REMOTE_TASK;
						AutoCharge.chg_flag = 0;
						AutoCharge.chg_fail = 0;
					}
					
					if(Robot_Sys.Mergency_Stop_flag == true || Moto.over_cur_flag == true){
						if(Moto.en_sta == 1){
							Moto.set_lear = 0;
							Moto.set_angle = 0;
							Send_speed_set(Moto.set_lear, Moto.set_angle);
						}else{
							Send_mdrv_en_set(1, 1);
						}
					}else if(Robot_Sys.Mergency_Stop_flag != true){
						if(Robot_Sys.AutoCharge_task_flag == false){
							if(((Robot_Sys.Ultra_sta & 0x03) != 0 || ((Robot_Sys.Crash_Flag & 0x01) != 0)) && rx_lear > 0){
								Moto.set_lear = 0;
								Moto.set_angle = 0;
							}else if(((Robot_Sys.Ultra_sta & 0x30) != 0 || ((Robot_Sys.Crash_Flag & 0x02) != 0)) && rx_lear < 0){
								Moto.set_lear = 0;
								Moto.set_angle = 0;
							}else{
								Moto.set_lear = rx_lear;
								Moto.set_angle = rx_angle;
							}
						}else{
							Moto.set_lear = AutoCharge.set_lear;
							Moto.set_angle = AutoCharge.set_angle;
						}
						
						#ifdef MOTO_LOCK	 	
							//电机不动时可以用手推动，适用于室外
							if(Moto.en_sta == 0){
								 Send_mdrv_en_set(1, 1);
							}else{
								 Send_speed_set(Moto.set_lear, Moto.set_angle);
							}
						#else
							//电机不动时可以用手推动，适用于室内
							if(Moto.set_lear == 0 && Moto.set_angle == 0 && (Pms.Bat_Sta & 0x01) == 0){
								if(Moto.stop_sta == 1 && Moto.en_sta == 1){
									Send_mdrv_en_set(0, 0);
								}else{
									Send_speed_set(Moto.set_lear, Moto.set_angle);
								}
							}else{
								if(Moto.en_sta == 0){
									Send_mdrv_en_set(1, 1);
								}else{
									Send_speed_set(Moto.set_lear, Moto.set_angle);
								}
							}
						#endif
					}
					break;

				case CMD_QUERY_POWER:		//查询电量及充电状态
					Robot_Sys.mSysPower.charger = Pms.Bat_Sta | AutoCharge.chg_fail;
				  Robot_Sys.mSysPower.power = Pms.Capacity;
					Send_PowerDataUpdata(gPscCnt++, Sdev_tmp, Robot_Sys.mSysPower);
					break;
				
				case CMD_Ultrasonic:    //障碍状态
					Send_Obstacle_Sta(gUpdateCnt, Sdev_tmp, Robot_Sys.Ultra_sta, Robot_Sys.Crash_Flag);
					Robot_Sys.StopDisable_flag = UsartToPC.Rx_Buf[9];		//解除过流停止
					break;
				
				case CMD_AUTOCHARGE:		//自动充电
					if((Pms.Bat_Sta & 0x01) == 0){				
						charge_tmp = UsartToPC.Rx_Buf[8];
						if(	Robot_Sys.Last_Task != CHG_TASK){
							Robot_Sys.Last_Task = CHG_TASK;
							Robot_Sys.mBeepStatus.BeepMode = 0x01;
							AutoCharge.AutoChg_Cmd = true;
							Robot_Sys.AutoCharge_task_flag = false;
							Robot_Sys.Remote_flag = true;
						}
					}
					#ifndef ROBOT_YZ01	
						Send_Autocharge_reply(gUpdateCnt, Sdev_tmp, charge_tmp);
					#endif
					break;
								
				case CMD_HEAD_POSE:     //查询头部角度
					//delay_ms(2);
					Robot_Sys.mHeadPose.Level = Head_Status.PSC_Level_Pos * 10;
					Robot_Sys.mHeadPose.Pitch = Head_Status.PSC_UD_Pos * 10;
				  Send_Head_Pose(gPscCnt ++, Sdev_tmp, Robot_Sys.mHeadPose, 0);
					break;
				
				case CMD_HEAD_ANGLE:    //设置头部角度
					level = ((UsartToPC.Rx_Buf[8] & 0xFF) << 8) + (UsartToPC.Rx_Buf[9] & 0xFF);
					pitch = ((UsartToPC.Rx_Buf[10] & 0xFF) << 8) + (UsartToPC.Rx_Buf[11] & 0xFF);
				  Head_Angle_Control(level, pitch);
				
				  #ifndef ROBOT_YZ01	
						Send_HeadAngle_reply(gPscCnt ++, Sdev_tmp, level, pitch);
					#endif
					break;
				
				case CMD_HEAD:  				//控制头部电机
					//55 AA 11 13 FF 01 06 00 05 00 00 00 00 00 00 00 3A 0D 0A		左
					//55 AA 11 13 FF 01 06 00 07 00 00 00 00 00 00 00 3A 0D 0A		右
					//55 AA 11 13 FF 01 06 00 06 00 00 00 00 00 00 00 3A 0D 0A		仰
					//55 AA 11 13 FF 01 06 00 08 00 00 00 00 00 00 00 3A 0D 0A		右
					//55 AA 11 13 FF 01 06 00 00 00 00 00 00 00 00 00 3A 0D 0A		俯
					switch(UsartToPC.Rx_Buf[8])
					{
						case 0x05:    //05 左
							Head_Status.PSC_Dir |= 1<<(5-1);
						  Head_Status.PSC_Dir &= ~(1<<(7-1));
							break; 
						case 0x06:    //06 仰
							Head_Status.PSC_Dir |= 1<<(6-1);
					    Head_Status.PSC_Dir &= ~(1<<(8-1));
							break;
						case 0x07:    //07 右
							Head_Status.PSC_Dir |= 1<<(7-1);
						  Head_Status.PSC_Dir &= ~(1<<(5-1));
						  break;
						case 0x08:    //08 俯
							Head_Status.PSC_Dir |= 1<<(8-1);
						  Head_Status.PSC_Dir &= ~(1<<(6-1));
						  break;
						case 0x09:
							Head_Back();
							break;
						default :
							Head_Status.PSC_Dir = 0;
						  break;
					}	
					#ifndef ROBOT_YZ01	
						Send_HeadCtrl_reply(gUpdateCnt++, Sdev_tmp, UsartToPC.Rx_Buf[8]);		
          #endif					
					break;
					
				case CMD_QUERY_LIFT:			//查询升降高度
					Robot_Sys.Out_en_flag = UsartToPC.Rx_Buf[8];
					Robot_Sys.Beep_en_flag = UsartToPC.Rx_Buf[9];
					Send_LiftMoto_Mess(gUpdateCnt++, Sdev_tmp, &Lift_Moto);
					break;
				
				case CMD_QUERY_TEMP:			//查询温湿度,CO2
					Send_TempHumMess(gUpdateCnt++, Sdev_tmp, &Environ);
					break;
				
				case CMD_QUERY_ENVIRON:		//查询PM值
					Send_EnvironMess(gUpdateCnt++, Sdev_tmp, &Environ);
					break;	
				
        case CMD_QUERY_OBS_EN:		//障碍传感器使能
					Send_Obs_EN_Mess(gUpdateCnt++, Sdev_tmp);
					break;
				
				case CMD_SET_LIFT:				//设置升降高度	
					Lift_Moto.Set_Height = (uint16_t)UsartToPC.Rx_Buf[8]<<8 | UsartToPC.Rx_Buf[9];
					Lift_Moto.Lift_OK_flag = false;
					#ifndef ROBOT_YZ01	
						Send_SetLift_reply(gUpdateCnt++, Sdev_tmp, Lift_Moto.Set_Height);
					#endif
					break;
	
				case CMD_SET_HEAD_OFFSET:	//设置头部偏差角度
					Head_Status.Pitch_Offset = Head_Status.Pitch_Offset +  Head_Status.PSC_UD_Pos - 90;
					Head_Status.Level_Offset = Head_Status.Level_Offset + 90 - Head_Status.PSC_Level_Pos;
				  Head_Status.PSC_UD_Pos = PSC_UD_DEFAULT;
				  Head_Status.PSC_Level_Pos = PSC_LEVEL_DEFAULT;
					SetHeadLevelPosition(PSC_LEVEL_DEFAULT, Head_Status.Level_Offset);
					SetHeadPitchPosition(PSC_UD_DEFAULT, Head_Status.Pitch_Offset);
		      //AT24CXX_WriteOneByte(6, Head_Status.Level_Offset);   //当前容量
		      //AT24CXX_WriteOneByte(7, Head_Status.Pitch_Offset);
					Send_angle_offset_reply(gUpdateCnt++, Sdev_tmp, Head_Status.Level_Offset, Head_Status.Pitch_Offset);
					break;
				
//				case CMD_SET_IR_EN:				//设置红外使能
//					Robot_Sys.IR_Front_Disable_Flag = UsartToPC.Rx_Buf[8];
//					Robot_Sys.IR_Bottom_Disable_Flag = UsartToPC.Rx_Buf[9];
//					break;
//				
//				case CMD_SET_ULTRA_EN:		//设置超声使能
//					Robot_Sys.Ultra_Disable_Flag = UsartToPC.Rx_Buf[8];
//				  //AT24CXX_WriteOneByte(8,Robot_Sys.Ultra_Disable_Flag);
//				  Send_ultra_en_reply(gUpdateCnt++, Sdev_tmp,Robot_Sys.Ultra_Disable_Flag);
//					break;
				
				case CMD_SET_SENSOR_EN:		//设置传感器使能
					Robot_Sys.Ultra_Disable_Flag = UsartToPC.Rx_Buf[8];
				  //AT24CXX_WriteOneByte(8,Robot_Sys.Ultra_Disable_Flag);
					Robot_Sys.IR_Front_Disable_Flag = UsartToPC.Rx_Buf[9];
					Robot_Sys.IR_Bottom_Disable_Flag = UsartToPC.Rx_Buf[10];
				  Send_ultra_en_reply(gUpdateCnt++, Sdev_tmp, Robot_Sys.Ultra_Disable_Flag);
					Robot_Sys.CarLight_flag = UsartToPC.Rx_Buf[11];
					break;
				
				case CMD_LIFT_CON:				//升降控制命令
					Lift_Moto.Cmd = UsartToPC.Rx_Buf[8];
					Lift_Moto.Set_Height = 0xffff;
					Robot_Moto.Robot_Dir = UsartToPC.Rx_Buf[9];
				  Robot_Moto.Slow_Flag = UsartToPC.Rx_Buf[10];
					Robot_Moto.Turn_Angle = UsartToPC.Rx_Buf[11];
					if(Robot_Moto.Robot_Dir != MOVE_STOP){
						Robot_Sys.Remote_flag = false;
						Robot_Sys.Last_Task = AGV_TASK;
					}else{
					}
					#ifndef ROBOT_YZ01	
						Send_LiftCtrl_reply(gUpdateCnt++, Sdev_tmp, Lift_Moto.Cmd);
					#endif
					break;
					
				case CMD_CON_MOVE:				//控制运动状态		
					break;  
				
				default:break;

			}
		}	
	}
}


/*=============================================================================
*  函数名 ：Send_OdomUpdata
*  作   者：hrx
*  创建时间：2022年4月11日 
*  修改时间：
*  输   入：
*  输   出：
*  说   明：发送Odom信息
*/ 
void  Send_OdomUpdata(uint8_t index, uint8_t addr, Odom_Data_type odom_dat)
{
	int16_t tmpYaw;
	int16_t tmpAngular_Rate;
	static uint8_t ultra[4];
	
	uint8_t *buf;
	uint8_t sramx=0;					//默认为内部sram
  buf = mymalloc(sramx,20);	//申请20字节
	
	ultra[0] = Ultra1.Distance/10;
	ultra[1] = Ultra2.Distance/10;
	ultra[2] = Ultra3.Distance/10;
	ultra[3] = Ultra4.Distance/10;
	
	buf[0] = 0x55;
	buf[1] = 0xAA;
	buf[2] = index;
	buf[3] = 0x13;
	buf[4] = 0x01;   //本机ID
	buf[5] = addr;   //目的ID
	buf[6] = 0x01;   //功能码
	buf[7] = 0x08;   //数据包个数
	buf[8] = (odom_dat.Left_Value >> 8) & 0x00ff;
	buf[9] = odom_dat.Left_Value & 0x00ff; 
	buf[10] = (odom_dat.Right_Value>>8) & 0x00ff; 
	buf[11] = odom_dat.Right_Value & 0x00ff; 
	buf[12]	= ultra[0];
	buf[13]	= ultra[1];
	buf[14]	= ultra[2];
	buf[15]	= ultra[3];
	buf[16]	= CRC8_Table(buf, 16);
	buf[17]	= 0x0D;
	buf[18]	= 0x0A;
	
	USARTx_SendMultibyte(USART_PC, buf, SEND_PC_LEN);
	//USARTx_SendMultibyte(USART1, buf, SEND_PC_LEN);
	myfree(sramx,buf);
}


/*=============================================================================
*  函数名 ：Send_Head_Pose
*  作   者：hrx
*  创建时间：2022年4月11日 
*  修改时间：
*  输   入：
*  输   出：
*  说   明：发送头部状态信息
*/ 
void  Send_Head_Pose(uint8_t index, uint8_t addr, HeadPose_Type mHead_Pose, bool stop_key)
{
	int16_t   level;
	int16_t   pitch;
	uint8_t 	*buf;
	uint8_t 	sramx=0;				//默认为内部sram
  buf = mymalloc(sramx,20);	//申请20字节
	
	buf[0] = 0x55;
	buf[1] = 0xAA;
	buf[2] = index;
	buf[3] = 0x13;
	buf[4] = 0x01;
	buf[5] = addr;
	buf[6] = 0x10;
	buf[7] = 0x08;
	buf[8] = (mHead_Pose.Level>>8)&0x00FF;
	buf[9] = (mHead_Pose.Level)&0x00FF;
	buf[10] = (mHead_Pose.Pitch>>8)&0x00FF;
	buf[11] = (mHead_Pose.Pitch)&0x00FF;

	buf[12] = 0x00;
	buf[13] = Robot_Sys.Mergency_Stop_flag;
	buf[14] = (uint8_t)stop_key;
	buf[15] = 0x00;
	buf[16]	= CRC8_Table(buf, 16);
	buf[17]	= 0x0D;
	buf[18]	= 0x0A;
	
	USARTx_SendMultibyte(USART_PC, buf, SEND_PC_LEN);
	myfree(sramx, buf);
}


/*=============================================================================
*  函数名 ：Send_LiftMoto_Mess
*  作   者：hrx
*  创建时间：2022年4月11日 
*  修改时间：
*  输   入：
*  输   出：
*  说   明：发送升降状态信息
*/ 
void Send_LiftMoto_Mess(uint8_t index, uint8_t addr,LiftMoto_Type *_liftmoto)
{
	uint8_t *buf;
	uint8_t sramx=0;					//默认为内部sram
  buf = mymalloc(sramx,20);	//申请20字节
	
	buf[0] = 0x55;
	buf[1] = 0xAA;
	buf[2] = index;
	buf[3] = 0x13;
	buf[4] = 0x01;
	buf[5] = addr;
	buf[6] = 0x61;
	buf[7] = 0x08;
	buf[8] = (_liftmoto->Height>>8)&0x00FF;
	buf[9] = _liftmoto->Height&0x00FF;
	buf[10] = _liftmoto->Limit_Switch_Flag | _liftmoto->OverCurrent_Flag;
	buf[11] = _liftmoto->Lift_OK_flag;
	buf[12] = _liftmoto->OverCurrent_Flag;
	buf[13] = 0;
	buf[14] = 0;
	buf[15] = 0;
	buf[16]	= CRC8_Table(buf, 16);
	buf[17]	= 0x0D;
	buf[18]	= 0x0A;
	USARTx_SendMultibyte(USART_PC, buf, SEND_PC_LEN);
	myfree(sramx, buf);
}


/*=============================================================================
*  函数名 ：Send_PowerDataUpdata
*  作   者：hrx
*  创建时间：2022年4月11日 
*  修改时间：
*  输   入：
*  输   出：
*  说   明：发送电源状态信息
*/ 
void Send_PowerDataUpdata(uint8_t index, uint8_t addr, Power_Type mPower)
{
	uint32_t tmpCharger;
	uint32_t tmpPower;
	
	uint8_t *buf;
	uint8_t sramx=0;					//默认为内部sram
  buf = mymalloc(sramx,20);	//申请2K字节
	
  buf[0] =0x55;
	buf[1] =0xAA;
	buf[2] =index;
	buf[3] =0x13;
	buf[4] =0x01;   //本机ID
	buf[5] =addr;   //目的ID
	buf[6] =0x09;   //功能码
	buf[7] =0x08;		//数据包个数

	tmpCharger = (uint32_t) mPower.charger;	//以整形发送
	tmpPower = (uint32_t) mPower.power;			//以整形发送	
	
	buf[8] = Robot_Sys.Mergency_Stop_flag;	//急停开关标志
	buf[9] = Robot_Sys.PowerOff_flag;				//关机命令
	buf[10]	= Robot_Sys.Voice_flag;					//语音使能标志
	buf[11]	= (uint8_t)tmpCharger;	

	buf[12]	= (Environ.voice >> 8) & 0x00ff;
	buf[13]	= Environ.voice  & 0x00ff;	
	buf[14]	= Moto.en_sta;	
	buf[15]	= (uint8_t)tmpPower;	

	buf[16]	= CRC8_Table(buf, 16);
	buf[17]	= 0x0D;
	buf[18]	= 0x0A;
	
	USARTx_SendMultibyte(USART_PC, buf, SEND_PC_LEN); 
	//USARTx_SendMultibyte(USART1, buf, SEND_PC_LEN); 
	myfree(sramx, buf);	//释放内存
}


/*=============================================================================
*  函数名 ：Send_PowerDataUpdata
*  作   者：hrx
*  创建时间：2022年4月11日 
*  修改时间：
*  输   入：
*  输   出：
*  说   明：发送电源状态信息
*/ 
void Send_HeadCtrlCmd(uint8_t index, uint8_t addr, uint8_t cmd_dat)
{
	int16_t tmpTemp;
	int16_t tmpHum;
	
	uint8_t *buf;
	uint8_t sramx=0;					//默认为内部sram
  buf = mymalloc(sramx,20);	//申请20字节
	
	buf[0] =0x55;
	buf[1] =0xAA;
	buf[2] =index;
	buf[3] =0x13;
	buf[4] =0x01;   //本机ID
	buf[5] =addr;   //目的ID
	buf[6] =0x32;   //功能码
	buf[7] =0x08;   //数据包个数

	buf[8] = cmd_dat;
	
	memset(buf+9,0,7);	
		
	buf[16] = CRC8_Table(buf, 16);
	buf[17]	= 0x0D;
	buf[18]	= 0x0A;
	
	USARTx_SendMultibyte(USART_PC, buf, SEND_PC_LEN); 
	myfree(sramx, buf);
}


/*=============================================================================
*  函数名 ：Send_Obs_EN_Mess
*  作   者：hrx
*  创建时间：2022年4月11日 
*  修改时间：
*  输   入：
*  输   出：
*  说   明：发送障碍传感器使能状态信息
*/ 
void Send_Obs_EN_Mess(uint8_t index, uint8_t addr)
{
	uint8_t *buf;
	uint8_t sramx = 0;				//默认为内部sram
  buf = mymalloc(sramx,20);	//申请20字节
	
	buf[0] = 0x55;
	buf[1] = 0xAA;
	buf[2] = index;
	buf[3] = 0x13;
	buf[4] = 0x01;
	buf[5] = addr;
	buf[6] = 0x64;
	buf[7] = 0x08;
	buf[8] = Robot_Sys.Ultra_Disable_Flag;		//超声失能标志
	buf[9] = Robot_Sys.IR_Bottom_Disable_Flag;
	buf[10] = Robot_Sys.IR_Front_Disable_Flag;
	buf[11] = 0;
	buf[12] = 0;
	buf[13] = 0;
	buf[14] = 0;
	buf[15] = 0;
	buf[16]	= CRC8_Table(buf, 16);
	buf[17]	= 0x0D;
	buf[18]	= 0x0A;
	
	USARTx_SendMultibyte(USART_PC, buf, SEND_PC_LEN);
	myfree(sramx, buf);
}

/*=============================================================================
*  函数名 ：Send_Obstacle_Sta
*  作   者：hrx
*  创建时间：2022年4月11日 
*  修改时间：
*  输   入：
*  输   出：
*  说   明：发送障碍传感器状态信息
*/ 
void Send_Obstacle_Sta(uint8_t index, uint8_t paddr, uint8_t obs_sta, uint8_t crach_sta)
{
	uint8_t *buf;
	uint8_t sramx=0;					//默认为内部sram
	int zf_flag = 0;					//判断正负
	
	int32_t Moto_Current_Send = Pms.Moto_Cur;
  buf = mymalloc(sramx,20);	//申请20字节
	
	if(Pms.Moto_Cur<0){
		Moto_Current_Send = Moto_Current_Send * -1;
		zf_flag = 1;
	}else{
		zf_flag = 0;
	}
	
	buf[0] = 0x55;
	buf[1] = 0xAA;
	buf[2] = index;
	buf[3] = 0x13;
	buf[4] = 0x01;			//本机ID
	buf[5] = paddr;			//目的ID
	buf[6] = 0x07;			//功能码
	buf[7] = 0x08;			//数据包个数8

	buf[8] = obs_sta;		//下左
	buf[9] = crach_sta;	//下右
	buf[10] = zf_flag;				
	buf[11] = (Moto_Current_Send >> 8) & 0x00ff;		//把驱动器电流发上去
	buf[12] = Moto_Current_Send  & 0x00ff;	
	buf[13] = Robot_Sys.Stop_flag;
	buf[14] = (Pms.Bat_Voltage >> 8) & 0x00ff;  		//把电池电压发上去
	buf[15] = Pms.Bat_Voltage  & 0x00ff;	
		
	buf[16]	= CRC8_Table(buf, 16);
	buf[17]	= 0x0D;
	buf[18]	= 0x0A;
	
	USARTx_SendMultibyte(USART_PC, buf, SEND_PC_LEN);
	myfree(sramx, buf);
} 


/*=============================================================================
*  函数名 ：Send_Speed_reply
*  作   者：hrx
*  创建时间：2022年4月11日 
*  修改时间：
*  输   入：
*  输   出：
*  说   明：发送设置速度应答
*/ 
void Send_Speed_reply(uint8_t index, uint8_t paddr, uint16_t linear, uint16_t angular){

	uint8_t *buf;
	uint8_t sramx=0;					//默认为内部sram
  buf = mymalloc(sramx,20);//申请20字节
	
	buf[0] = 0x55;
	buf[1] = 0xAA;
	buf[2] = index;
	buf[3] = 0x13;
	buf[4] = 0x01;	//本机ID
	buf[5] = paddr;	//目的ID6
	buf[6] = 0x15;
	buf[7] = 0x08;	//数据包个数

	buf[8] = linear>>8;
	buf[9] = linear;
	buf[10] = angular>>8;
	buf[11] = angular;
	buf[12] = AutoCharge.NotFind_Flag;
	buf[13] = 0;
	buf[14] = 0;
	buf[15] = 0;
		
	buf[16] = CRC8_Table(buf, 16);
	buf[17] = 0x0D;
	buf[18] = 0x0A;
	USARTx_SendMultibyte(USART_PC, buf, SEND_PC_LEN);
	myfree(sramx, buf);
}


/*=============================================================================
*  函数名 ：Send_Autocharge_reply
*  作   者：hrx
*  创建时间：2022年4月11日 
*  修改时间：
*  输   入：
*  输   出：
*  说   明：发送自动充电命令应答
*/ 
void Send_Autocharge_reply(uint8_t index, uint8_t paddr, uint8_t dat){

	uint8_t *buf;
	uint8_t sramx=0;					//默认为内部sram
  buf = mymalloc(sramx,20);	//申请20字节
	
	buf[0] = 0x55;
	buf[1] = 0xAA;
	buf[2] = index;
	buf[3] = 0x13;
	buf[4] = 0x01;	//本机ID
	buf[5] = paddr;	//目的ID
	buf[6] = 0x08;	//自动充电
	buf[7] = 0x08;	//数据包个数

	buf[8] = 0;
	buf[9] = 0;
	buf[10] = 0;
	buf[11] = 0;
	buf[12] = 0;
	buf[13] = 0;
	buf[14] = 0;
	buf[15] = 0;

	buf[16] = CRC8_Table(buf, 16);
	buf[17] = 0x0D;
	buf[18] = 0x0A;
	
	USARTx_SendMultibyte(USART_PC, buf, SEND_PC_LEN);
	myfree(sramx, buf);
} 


/*=============================================================================
*  函数名 ：Send_HeadAngle_reply
*  作   者：hrx
*  创建时间：2022年4月11日 
*  修改时间：
*  输   入：
*  输   出：
*  说   明：发送设置头部角度命令应答
*/ 
void Send_HeadAngle_reply(uint8_t index, uint8_t paddr, uint16_t set_level, uint16_t set_pitch){

	uint8_t *buf;
	uint8_t sramx=0;					//默认为内部sram
  buf = mymalloc(sramx,20);	//申请20字节
	
	buf[0] = 0x55;
	buf[1] = 0xAA;
	buf[2] = index;
	buf[3] = 0x13;
	buf[4] = 0x01;	//本机ID
	buf[5] = paddr;	//目的ID
	buf[6] = 0x11;	//头部角度设置
	buf[7] = 0x08;	//数据包个数

	buf[8] = set_level>>8;
	buf[9] = set_level;
	buf[10] = set_pitch>>8;
	buf[11] = set_pitch;
	buf[12] = 0;
	buf[13] = 0;
	buf[14] = 0;
	buf[15] = 0;
		
	buf[16] = CRC8_Table(buf, 16);
	buf[17] = 0x0D;
	buf[18] = 0x0A;
	
	USARTx_SendMultibyte(USART_PC, buf, SEND_PC_LEN);
	myfree(sramx, buf);
}


/*=============================================================================
*  函数名 ：Send_HeadAngle_reply
*  作   者：hrx
*  创建时间：2022年4月11日 
*  修改时间：
*  输   入：
*  输   出：
*  说   明：发送头部控制命令应答
*/ 
void Send_HeadCtrl_reply(uint8_t index, uint8_t paddr, uint8_t cmd){

	uint8_t *buf;
	uint8_t sramx=0;					//默认为内部sram
  buf = mymalloc(sramx,20);	//申请20字节
	
	buf[0] = 0x55;
	buf[1] = 0xAA;
	buf[2] = index;
	buf[3] = 0x13;
	buf[4] = 0x01;	//本机ID
	buf[5] = paddr;	//目的ID
	buf[6] = 0x06;	//头部角度设置
	buf[7] = 0x08;	//数据包个数

	buf[8] = cmd;
	buf[9] = 0;
	buf[10] = 0;
	buf[11] = 0;
	buf[12] = 0;
	buf[13] = 0;
	buf[14] = 0;
	buf[15] = 0;
	
	buf[16] = CRC8_Table(buf, 16);
	buf[17] = 0x0D;
	buf[18] = 0x0A;
	
	USARTx_SendMultibyte(USART_PC, buf, SEND_PC_LEN);
	myfree(sramx, buf);
}


/*=============================================================================
*  函数名 ：Send_SetLift_reply
*  作   者：hrx
*  创建时间：2022年4月11日 
*  修改时间：
*  输   入：
*  输   出：
*  说   明：发送设置升降高度命令应答
*/ 
void Send_SetLift_reply(uint8_t index, uint8_t paddr, uint16_t heitht){

	uint8_t *buf;
	uint8_t sramx=0;					//默认为内部sram
  buf = mymalloc(sramx,20);	//申请20字节
	
	buf[0] = 0x55;
	buf[1] = 0xAA;
	buf[2] = index;
	buf[3] = 0x13;
	buf[4] = 0x01;	//本机ID
	buf[5] = paddr;	//目的ID
	buf[6] = 0x71;	//头部角度设置
	buf[7] = 0x08;	//数据包个数

	buf[8] = heitht>>8;
	buf[9] = heitht;
	buf[10] = 0;
	buf[11] = 0;
	buf[12] = 0;
	buf[13] = 0;
	buf[14] = 0;
	buf[15] = 0;

	buf[16]= CRC8_Table(buf, 16);
	buf[17]= 0x0D;
	buf[18]= 0x0A;
	
	USARTx_SendMultibyte(USART_PC, buf, SEND_PC_LEN);
	myfree(sramx, buf);
}


/*=============================================================================
*  函数名 ：Send_LiftCtrl_reply
*  作   者：hrx
*  创建时间：2022年4月11日 
*  修改时间：
*  输   入：
*  输   出：
*  说   明：发送升降控制命令应答
*/ 
void Send_LiftCtrl_reply(uint8_t index, uint8_t paddr, uint8_t cmd){

	uint8_t *buf;
	uint8_t sramx=0;					//默认为内部sram
  buf = mymalloc(sramx,20);	//申请20字节
	
	buf[0] = 0x55;
	buf[1] = 0xAA;
	buf[2] = index;
	buf[3] = 0x13;
	buf[4] = 0x01;	//本机ID
	buf[5] = paddr;	//目的ID
	buf[6] = 0x81;	//头部角度设置
	buf[7] = 0x08;	//数据包个数

	buf[8] = cmd;
	buf[9] = 0;
	buf[10] = 0;
	buf[11] = 0;
	buf[12] = 0;
	buf[13] = 0;
	buf[14] = 0;
	buf[15] = 0;
	
	buf[16] = CRC8_Table(buf, 16);
	buf[17] = 0x0D;
	buf[18] = 0x0A;
	
	USARTx_SendMultibyte(USART_PC, buf, SEND_PC_LEN);
	myfree(sramx, buf);
}


/*=============================================================================
*  函数名 ：Send_LiftCtrl_reply
*  作   者：hrx
*  创建时间：2022年4月11日 
*  修改时间：
*  输   入：
*  输   出：
*  说   明：发送升降控制命令应答
*/ 
void Send_ultra_en_reply(uint8_t index, uint8_t paddr, uint8_t cmd){

	uint8_t *buf;
	uint8_t sramx=0;					//默认为内部sram
  buf = mymalloc(sramx,20);	//申请20字节
	
	buf[0] = 0x55;
	buf[1] = 0xAA;
	buf[2] = index;
	buf[3] = 0x13;
	buf[4] = 0x01;	//本机ID
	buf[5] = paddr;	//目的ID
	buf[6] = 0x77;	//头部角度设置
	buf[7] = 0x08;	//数据包个数

	buf[8] = cmd;
	buf[9] = 0;
	buf[10] = 0;
	buf[11] = 0;
	buf[12] = 0;
	buf[13] = 0;
	buf[14] = 0;
	buf[15] = 0;
	
	buf[16] = CRC8_Table(buf, 16);
	buf[17] = 0x0D;
	buf[18] = 0x0A;
	
	USARTx_SendMultibyte(USART_PC, buf, SEND_PC_LEN);
	myfree(sramx, buf);
}

/*=============================================================================
*  函数名 ：Send_angle_offset_reply
*  作   者：hrx
*  创建时间：2022年4月11日 
*  修改时间：
*  输   入：
*  输   出：
*  说   明：设置头部云台偏移量
*/ 
void Send_angle_offset_reply(uint8_t index, uint8_t paddr, int8_t level_offset, int8_t pitch_offset)
{

	uint8_t *buf;
	uint8_t sramx=0;					//默认为内部sram
  buf = mymalloc(sramx,20);	//申请20字节
	
	buf[0] = 0x55;
	buf[1] = 0xAA;
	buf[2] = index;
	buf[3] = 0x13;
	buf[4] = 0x01;	//本机ID
	buf[5] = paddr;	//目的ID
	buf[6] = 0x75;	//头部角度设置
	buf[7] = 0x08;	//数据包个数

	buf[8] = level_offset;
	buf[9] = pitch_offset;
	buf[10] = 0;
	buf[11] = 0;
	buf[12] = 0;
	buf[13] = 0;
	buf[14] = 0;
	buf[15] = 0;
	
	buf[16] = CRC8_Table(buf, 16);
	buf[17] = 0x0D;
	buf[18] = 0x0A;
	
	USARTx_SendMultibyte(USART_PC, buf, SEND_PC_LEN);
	myfree(sramx, buf);
}


/*=============================================================================
*  函数名 ：CRC8Table
*  作   者：hrx
*  创建时间：2022年4月11日 
*  修改时间：
*  输   入：
*  输   出：
*  说   明：校验数组
*/ 
const char CRC8Table[]={
  0, 94, 188, 226, 97, 63, 221, 131, 194, 156, 126, 32, 163, 253, 31, 65,
  157, 195, 33, 127, 252, 162, 64, 30, 95, 1, 227, 189, 62, 96, 130, 220,
  35, 125, 159, 193, 66, 28, 254, 160, 225, 191, 93, 3, 128, 222, 60, 98,
  190, 224, 2, 92, 223, 129, 99, 61, 124, 34, 192, 158, 29, 67, 161, 255,
  70, 24, 250, 164, 39, 121, 155, 197, 132, 218, 56, 102, 229, 187, 89, 7,
  219, 133, 103, 57, 186, 228, 6, 88, 25, 71, 165, 251, 120, 38, 196, 154,
  101, 59, 217, 135, 4, 90, 184, 230, 167, 249, 27, 69, 198, 152, 122, 36,
  248, 166, 68, 26, 153, 199, 37, 123, 58, 100, 134, 216, 91, 5, 231, 185,
  140, 210, 48, 110, 237, 179, 81, 15, 78, 16, 242, 172, 47, 113, 147, 205,
  17, 79, 173, 243, 112, 46, 204, 146, 211, 141, 111, 49, 178, 236, 14, 80,
  175, 241, 19, 77, 206, 144, 114, 44, 109, 51, 209, 143, 12, 82, 176, 238,
  50, 108, 142, 208, 83, 13, 239, 177, 240, 174, 76, 18, 145, 207, 45, 115,
  202, 148, 118, 40, 171, 245, 23, 73, 8, 86, 180, 234, 105, 55, 213, 139,
  87, 9, 235, 181, 54, 104, 138, 212, 149, 203, 41, 119, 244, 170, 72, 22,
  233, 183, 85, 11, 136, 214, 52, 106, 43, 117, 151, 201, 74, 20, 246, 168,
  116, 42, 200, 150, 21, 75, 169, 247, 182, 232, 10, 84, 215, 137, 107, 53
};


/*=============================================================================
*  函数名 ：CRC8_Table
*  作   者：hrx
*  创建时间：2022年4月11日 
*  修改时间：
*  输   入：
*  输   出：
*  说   明：校验
*/ 
unsigned char CRC8_Table(unsigned char *p, char counter)
{
    unsigned char crc8 = 0;

    for( ; counter > 0; counter--){
        crc8 = CRC8Table[crc8^*p];
        p++;
    }
    return(crc8);
}


/*=============================================================================
*  函数名 ：CRC8
*  作   者：hrx
*  创建时间：2022年4月11日 
*  修改时间：
*  输   入：
*  输   出：
*  说   明：校验
*/ 
uint8_t  CRC8(uint8_t *pDate, uint8_t length)
{
	uint8_t crc=0;
	uint8_t i=0;
	while(length--)
	{
		crc ^= *pDate++;
		for(i = 0;i < 8;i++)
		{
			if(crc & 0x01)
			{
				crc = (crc >> 1) ^ 0x8C;
			}else crc >>= 1;
		}
	}
    return crc;	
}




