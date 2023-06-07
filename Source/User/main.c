
/*=============================================================================
*  Copyright (C) ".strftime("%Y")." All rights reserved.
*
*  �ļ�����"main.c"
*  ���ߣ�hrx
*  ����ʱ�䣺2022��4��11�� 
*  �޸�ʱ�䣺
*  ˵�������������
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
	
	System_Board_Init();  	//��ʼ��ϵͳӲ����Դ

	NVIC_Configuration();  	//�ж����ȼ�����

	delay_ms(200);
	EN_OUT_ENABLE;					//Ӳ�����ʹ��VCC-OUT
	System_StartTips();			//ϵͳ������������ʾ
	
	Head_Status.PSC_UD_Default_Pos = PSC_UD_DEFAULT;         //80   �м�Ĭ��ֵ
	Head_Status.PSC_UD_Pos = PSC_UD_DEFAULT;                 //80   ��ǰλ��
	Head_Status.PSC_UD_MAX_Pos = PSC_UD_MAX;                 //125	max_pos=80+45
	Head_Status.PSC_UD_MIN_Pos = PSC_UD_MIN;             		 //35		min_pos=80-45
	
	Head_Status.PSC_Level_Default_Pos = PSC_LEVEL_DEFAULT;   //80   �м�Ĭ��ֵ
	Head_Status.PSC_Level_Pos = PSC_LEVEL_DEFAULT;           //80   ��ǰλ��
	Head_Status.PSC_Level_MAX_Pos = PSC_LEVEL_MAX;           //125	max_pos=80+45
	Head_Status.PSC_Level_MIN_Pos = PSC_LEVEL_MIN;           //35		min_pos=80-45
	

	//����ͨ�������жϹ��ػ��Ƿ��Ѿ�����
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
	
//	//ֱ�Ӹ������ź�
//	PC_Power_Cfg_Init(PC_STARTUP_ON);
//	delay_ms(300);
//	PC_Power_Cfg_Init(PC_STARTUP_OFF);
  
	Robot_Moto.Left_Real_Dir = 0;
	Robot_Moto.Right_Real_Dir = 0;
	
//	Pms.Capacity_Set = FULL_CAP;
//	AT24CXX_WriteOneByte(2,(Pms.Capacity_Set>>8) & 0x00ff);	//��������
//	AT24CXX_WriteOneByte(3,Pms.Capacity_Set& 0x00ff);
//	Pms.Capacity_mah = 10000;
//	AT24CXX_WriteOneByte(4,(Pms.Capacity_mah>>8) & 0x00ff);	//��ǰ����
//	AT24CXX_WriteOneByte(5,Pms.Capacity_mah& 0x00ff);
	if(AT24xx_Check() == 0){
		Read_PowerOn_AT24xx();		//������ȡ�洢����
	}else{
		printf("Read AT24xx false!\r\n");
		Robot_Sys.mBeepStatus.BeepMode = 3;		//��ȡʧ�ܷ�������
	}
  Pms.Capacity = Pms.Capacity_mah/(Pms.Capacity_Set/100);
	printf("Capacity_Set: %d, Capacity_mah = %d,Capacity: %d\r\n", Pms.Capacity_Set, Pms.Capacity_mah, Pms.Capacity);
	printf("Level_Offset: %d, Pitch_Offset: %d\r\n", Head_Status.Level_Offset, Head_Status.Pitch_Offset);
	printf("Ultra_Disable_Flag: %d, IR_Bottom_Disable_Flag: %d\r\n", Robot_Sys.Ultra_Disable_Flag, Robot_Sys.IR_Bottom_Disable_Flag);

	SetHeadPitchPosition(PSC_UD_DEFAULT, Head_Status.Pitch_Offset);
  delay_ms(10);
	SetHeadLevelPosition(PSC_LEVEL_DEFAULT, Head_Status.Level_Offset);
	delay_ms(100);
	
	//�������˳�ʼ������һֱ����
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
	IWDG_Init(4, 500); 		//���Ƶ��Ϊ64,����ֵΪ500,���ʱ��Ϊ1s	
	
	OSInit(&err);		    	//��ʼ��UCOSIII
	OS_CRITICAL_ENTER();	//�����ٽ���			  
	
	//������ʼ����
	#ifdef ROBOT_YZ01	
	printf(" ROBOT_YZ01 Sys starting!!\r\n"); 
	#endif
	#ifdef ROBOT_M100	
	printf(" ROBOT_M100 Sys starting!!\r\n"); 
	#endif
	#ifdef ROBOT_MR9	
	printf(" ROBOT_MR9 Sys starting!!\r\n"); 
	#endif
	
	DMA_ITConfig(DMA2_Stream0, DMA_IT_TC, ENABLE);	//ʹ���ж�
	OSTaskCreate((OS_TCB 		* )&StartTaskTCB,				//������ƿ�
							 (CPU_CHAR	* )"start task", 				//��������
               (OS_TASK_PTR )start_task, 					//������
               (void		  * )0,										//���ݸ��������Ĳ���
               (OS_PRIO	    )START_TASK_PRIO,			//�������ȼ�
               (CPU_STK   * )&START_TASK_STK[0],	//�����ջ����ַ
               (CPU_STK_SIZE)START_STK_SIZE/10,		//�����ջ�����λ
               (CPU_STK_SIZE)START_STK_SIZE,			//�����ջ��С
               (OS_MSG_QTY  )0,										//�����ڲ���Ϣ�����ܹ����յ������Ϣ��Ŀ,Ϊ0ʱ��ֹ������Ϣ
               (OS_TICK	    )0,										//��ʹ��ʱ��Ƭ��תʱ��ʱ��Ƭ���ȣ�Ϊ0ʱΪĬ�ϳ��ȣ�
               (void   	  * )0,										//�û�����Ĵ洢��
               (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,	//����ѡ��
               (OS_ERR 	  * )&err);								//��Ÿú�������ʱ�ķ���ֵ
								 
	OS_CRITICAL_EXIT();						//�˳��ٽ���	 
	OSStart(&err);      					//����UCOSIIId
}





