/*=============================================================================
*  Copyright (C) ".2016 " All rights reserved.
*
*  文件名："App_AnalysisCMD.h"
*  作者hrx
*  创建时间：2016年07月25日 
*  修改时间：
*  说明：
*
==============================================================================*/


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __APP_ANALYSISCMD_H
#define __APP_ANALYSISCMD_H

#ifdef __cplusplus
 extern "C" {
#endif 

	 
/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include "stdbool.h"
#include  "inet.h"

	
//#define ROBOT_YZ01		1		//YZ01
//#define ROBOT_M100		1		//M100
#define ROBOT_MR9		1		//MR9

//#define MOTO_LOCK		1		//电机是否抱死

//#define LiftMoto_1		1		//竹节式升降杆
#define LiftMoto_2		1		//新升降杆

	 
#define SEND_PC_LEN  19
//#define patrol_speed    0.3
#define OPEN_BEEP_DEBUG

#define	FRAME_H   0x55
#define	FRAME_L   0xAA
	 
#define	END_H     0x0D
#define	END_L     0x0A	 
	 
	 
// buffer 缓冲区大小	 
#define	MAX_TX_LEN     64
#define	MAX_RX_LEN     64
//#define   CMD_LEN        15	 

// 数据帧寄存器
#define	FRH_REG   0x00   // 帧1
#define	FRL_REG   0x01   // 帧2
#define	FINX_REG  0x02   // index
#define	FLEN_REG  0x03   // Frame length
#define	S_ID_REG  0x04   // 源ID
#define	P_ID_REG  0x05	 // 目的ID
#define	CMD_REG   0x06   // 命令
#define	DUM_REG   0x07   // 数据包大小

#define	SPEED1_REG1  0x08

#define	SPEED2_REG1  0x0A


// 本地地址
#define	DEV_ID    0x01
#define	PSC_ID    0x02

// 命令
#define	CMD_QUERY_ODOM       0x01	   // 查询ODOM
#define	CMD_QUERY_PWR        0x02    // 查询电量
#define	CMD_QUERY_TEMPHUM    0x03    // 查询温度，湿度
#define	CMD_SET_SPEED        0x15	   // 设置转速
#define	CMD_STOP             0x05	   // 刹车
#define	CMD_HEAD             0x06    // 头部运动控制
#define	CMD_Ultrasonic       0x07    // 查询  超声的
#define	CMD_AUTOCHARGE       0x08    // 自动充电
#define	CMD_CHECK            0x19    // 上面发送数据下来校正直行
#define	CMD_QUERY_POWER      0x09    // 查询电量
#define	CMD_HEAD_POSE        0x10    // 查询  头部的姿态
#define	CMD_HEAD_ANGLE			 0x11    // 用角度控制头部
#define	CMD_Recall					 0x12			//这个是告诉ros是否有 充电器在附近

#define	CMD_AGV_Near			   0x20			

#define	CMD_QUERY_AGV				 0x60     //查询AGV信息(RFID坐标,到点标志)
#define	CMD_QUERY_LIFT       0x61     //查询升降电机信息
#define	CMD_QUERY_TEMP			 0x62     //查询环境参数
#define	CMD_QUERY_ENVIRON    0x63     //查询环境参数
#define	CMD_QUERY_OBS_EN     0x64     //查询障碍使能

#define	CMD_SET_LIFT				  0x71    	//设置升降电机高度
#define	CMD_SET_BAT_CAP       0x72      //设置电池的容量  
#define	CMD_SET_RESET_EEPOM   0x73      //复位EEPOM 
#define	CMD_SET_RFID          0x74      //
#define	CMD_SET_HEAD_OFFSET   0x75      //设置头部归正
//#define	CMD_SET_IR_EN					0x76      //设置红外使能
//#define	CMD_SET_ULTRA_EN      0x77      //设置超声使能
#define	CMD_SET_SENSOR_EN     0x77      //设置传感器使能

#define	CMD_LIFT_CON					0x81			//控制升降电机
#define	CMD_CON_MOVE					0x82


#define MOVE_STOP			0
#define MOVE_FRONT		1
#define MOVE_BACK			5
#define MOVE_RIGHT		3
#define MOVE_LEFT			7
	
#define AGV_STOP			MOVE_STOP
#define AGV_FRONT			MOVE_FRONT
#define AGV_BACK			MOVE_BACK
#define AGV_RIGHT			MOVE_RIGHT
#define AGV_LEFT			MOVE_LEFT

// USART 接收寄存器
// [7]:数据帧接收完成  0:接收数据帧未完成，1:接收数据帧完成
// [6]:接收到数据帧头  0:未接收到数据帧头，1:接收到数据帧头
// [0]:帧尾

//#define WHEEL_TRACK  0.31        //单位米 轮距 ,送餐0.31,加拿大0.28
#define WHEEL_TRACK  	0.38         //YZ01C 单位米 轮距 ,金属外壳0.31,加拿大0.28，塑胶底壳0.41
//#define WHEEL_TRACK  0.28         //YZ01B 单位米 轮距 ,金属外壳0.31,加拿大0.28，塑胶底壳0.41
#define WHEEL_RADIUS  0.0845        //单位米,轮子半径
#define PI  					3.1415926       
//#define PULSE_CYCLE   139     //脉冲周期
//#define PULSE_CYCLE		1000     //脉冲周期
#define PULSE_CYCLE   5600    //脉冲周期

#define PWM_STARTUP  	50
#define PWM_MIN   		50
#define PWM_MAX   		700
#define PWM_SET_MAX   1
#define LEFT_ADJUST   1
#define RIGHT_ADJUST  1


typedef struct
{
	uint8_t	Send_BUF[MAX_TX_LEN];
	uint8_t	Recv_BUF[MAX_RX_LEN];
	uint8_t	RX_STA;
	uint8_t	Recv_Length;
	bool		Recv_OK;
}PC_COM_type;	
	 

typedef struct
{
	int16_t Left_Value;
	int16_t Right_Value;
	int16_t Left_Value_Sum;
	int16_t Right_Value_Sum;
}Odom_Data_type;


extern Odom_Data_type  Moto_Odom; 

void  Send_OdomUpdata( uint8_t index, uint8_t paddr, Odom_Data_type odom_dat );
void  Send_HeadCtrlCmd( uint8_t index, uint8_t addr, uint8_t cmd_dat );
void  Send_HeadCtrlAngle(uint8_t index, uint8_t addr , uint8_t level,uint8_t );
void Send_Obs_EN_Mess(uint8_t index,uint8_t addr);
unsigned char CRC8_Table(unsigned char *p, char counter);
uint8_t  CRC8(uint8_t *pDate, uint8_t length);
void  AnalysisCMD(void);	 
unsigned char CRC8_Table(unsigned char *p, char counter);	 
void  SetMotoSpeed(void);
void Send_Obstacle_Sta(uint8_t index,uint8_t paddr,uint8_t obs_sta,uint8_t crach_sta);
void Send_Speed_reply(uint8_t index,uint8_t paddr,uint16_t linear, uint16_t angular);
void Send_Autocharge_reply(uint8_t index,uint8_t paddr,uint8_t dat);
void Send_HeadCtrl_reply(uint8_t index,uint8_t paddr,uint8_t cmd);
void Send_HeadAngle_reply(uint8_t index,uint8_t paddr,uint16_t set_level, uint16_t set_pitch);
void Send_SetLift_reply(uint8_t index,uint8_t paddr,uint16_t heitht);
void Send_LiftCtrl_reply(uint8_t index,uint8_t paddr,uint8_t cmd);
void Send_ultra_en_reply(uint8_t index,uint8_t paddr,uint8_t cmd);
void Send_angle_offset_reply(uint8_t index,uint8_t paddr,int8_t level_offset, int8_t pitch_offset);
#ifdef __cplusplus
}
#endif

#endif /*__BSP_SYSTICK_CFG_H */


/************************ (C) COPYRIGHT YzRobot  *****END OF FILE****/




