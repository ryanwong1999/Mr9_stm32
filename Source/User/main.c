
/*=============================================================================
*  Copyright (C) ".strftime("%Y")." All rights reserved.
*
*  文件名："main.c"
*  作者：hrx
*  创建时间：2022年4月11日 
*  修改时间：
*  说明：主函数入口
*
==============================================================================*/

/* Includes ------------------------------------------------------------------*/
#include "includes.h"

int main(void)
{
	static uint8_t pc_test = 0;
	static uint8_t test_cnt = 0;
	uint16_t i = 0;
	OS_ERR err;
	CPU_SR_ALLOC();
	
	System_Board_Init();  	//初始化系统硬件资源

	NVIC_Configuration();  	//中断优先级配置

	delay_ms(200);
	EN_OUT_ENABLE;					//硬开软关使能VCC-OUT
	System_StartTips();			//系统启动蜂鸣器提示
	
	Head_Status.PSC_UD_Default_Pos = PSC_UD_DEFAULT;         //80   中间默认值
	Head_Status.PSC_UD_Pos = PSC_UD_DEFAULT;                 //80   当前位置
	Head_Status.PSC_UD_MAX_Pos = PSC_UD_MAX;                 //125	max_pos=80+45
	Head_Status.PSC_UD_MIN_Pos = PSC_UD_MIN;             		 //35		min_pos=80-45
	
	Head_Status.PSC_Level_Default_Pos = PSC_LEVEL_DEFAULT;   //80   中间默认值
	Head_Status.PSC_Level_Pos = PSC_LEVEL_DEFAULT;           //80   当前位置
	Head_Status.PSC_Level_MAX_Pos = PSC_LEVEL_MAX;           //125	max_pos=80+45
	Head_Status.PSC_Level_MIN_Pos = PSC_LEVEL_MIN;           //35		min_pos=80-45
	

	//开机通过电流判断工控机是否已经起来
	pc_test = false;
	while(pc_test != true){
	  printf("-----------%d\r\n", ADC_Filter_Value[3]);
		delay_ms(20);
		if(ADC_Filter_Value[3] < 1970){			//-562.5
			test_cnt ++;
			if(test_cnt > 5){
				pc_test = true;
			}
			delay_ms(10);
		}else{
			test_cnt = 0;
			delay_ms(100);
			if(ADC_Filter_Value[3] >= 1950){		//-937.5
				PC_Power_Cfg_Init(PC_STARTUP_ON);
				BEEP_ON;
				delay_ms(300);
				PC_Power_Cfg_Init(PC_STARTUP_OFF);
				BEEP_OFF;
				pc_test = true;
			}
		}
	}
	
//	//直接给开机信号
//	PC_Power_Cfg_Init(PC_STARTUP_ON);
//	delay_ms(300);
//	PC_Power_Cfg_Init(PC_STARTUP_OFF);
  
	Robot_Moto.Left_Real_Dir = 0;
	Robot_Moto.Right_Real_Dir = 0;
	
//	Pms.Capacity_Set = FULL_CAP;
//	AT24CXX_WriteOneByte(2,(Pms.Capacity_Set>>8) & 0x00ff);	//设置容量
//	AT24CXX_WriteOneByte(3,Pms.Capacity_Set& 0x00ff);
//	Pms.Capacity_mah = 10000;
//	AT24CXX_WriteOneByte(4,(Pms.Capacity_mah>>8) & 0x00ff);	//当前容量
//	AT24CXX_WriteOneByte(5,Pms.Capacity_mah& 0x00ff);
	if(AT24xx_Check() == 0){
		Read_PowerOn_AT24xx();		//开机读取存储数据
	}else{
		printf("Read AT24xx false!\r\n");
		Robot_Sys.mBeepStatus.BeepMode = 3;		//读取失败蜂鸣器响
	}
  Pms.Capacity = Pms.Capacity_mah/(Pms.Capacity_Set/100);
	printf("Capacity_Set: %d, Capacity_mah = %d,Capacity: %d\r\n", Pms.Capacity_Set, Pms.Capacity_mah, Pms.Capacity);
	printf("Level_Offset: %d, Pitch_Offset: %d\r\n", Head_Status.Level_Offset, Head_Status.Pitch_Offset);
	printf("Ultra_Disable_Flag: %d, IR_Bottom_Disable_Flag: %d\r\n", Robot_Sys.Ultra_Disable_Flag, Robot_Sys.IR_Bottom_Disable_Flag);

	SetHeadPitchPosition(PSC_UD_DEFAULT, Head_Status.Pitch_Offset);
  delay_ms(10);
	SetHeadLevelPosition(PSC_LEVEL_DEFAULT, Head_Status.Level_Offset);
	delay_ms(100);
	
	//新升降杆初始化让它一直往下
	#ifdef LiftMoto_2
	Lift_Moto.Cmd = LIFT_DOWN;
	#endif

	Moto_mdrv_init();	
	Robot_Sys.Remote_flag = true;
	UsartToDrv.Usart_Rx_OK = false;
	EN_MDRV_ENABLE;

	delay_ms(500);
 	UsartToEnviro.Comm_TimeOut = 0;
	EN24_ENABLE;
	AutoCharge.chg_fail = 0;
	AutoCharge.rechg_cnt = 0;
	Pms.Reset_Flag = 0;
	Robot_Sys.CarLight_flag = 0;
//	while(1)
//	{
//		USARTx_SendOneByte(USART3, 0x33);
//		USARTx_SendOneByte(USART1, 0x33);
//		//Send_PowerDataUpdata(gPscCnt++, Sdev_tmp, Robot_Sys.mSysPower);
//		delay_ms(500);
//	}
	IWDG_Init(4, 500); 		//与分频数为64,重载值为500,溢出时间为1s	
	
	OSInit(&err);		    	//初始化UCOSIII
	OS_CRITICAL_ENTER();	//进入临界区			  
	
	//创建开始任务
	#ifdef ROBOT_YZ01	
	printf(" ROBOT_YZ01 Sys starting!!\r\n"); 
	#endif
	#ifdef ROBOT_M100	
	printf(" ROBOT_M100 Sys starting!!\r\n"); 
	#endif
	#ifdef ROBOT_MR9	
	printf(" ROBOT_MR9 Sys starting!!\r\n"); 
	#endif
	
	DMA_ITConfig(DMA2_Stream0, DMA_IT_TC, ENABLE);	//使能中断
	OSTaskCreate((OS_TCB 		* )&StartTaskTCB,				//任务控制块
							 (CPU_CHAR	* )"start task", 				//任务名字
               (OS_TASK_PTR )start_task, 					//任务函数
               (void		  * )0,										//传递给任务函数的参数
               (OS_PRIO	    )START_TASK_PRIO,			//任务优先级
               (CPU_STK   * )&START_TASK_STK[0],	//任务堆栈基地址
               (CPU_STK_SIZE)START_STK_SIZE/10,		//任务堆栈深度限位
               (CPU_STK_SIZE)START_STK_SIZE,			//任务堆栈大小
               (OS_MSG_QTY  )0,										//任务内部消息队列能够接收的最大消息数目,为0时禁止接收消息
               (OS_TICK	    )0,										//当使能时间片轮转时的时间片长度，为0时为默认长度，
               (void   	  * )0,										//用户补充的存储区
               (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,	//任务选项
               (OS_ERR 	  * )&err);								//存放该函数错误时的返回值
								 
	OS_CRITICAL_EXIT();						//退出临界区	 
	OSStart(&err);      					//开启UCOSIIId
}





