
/*=============================================================================
*  Copyright (C) ".strftime("%Y")." All rights reserved.
*
*  �ļ�����"App_Task.c"
*  ���ߣ�hrx
*  ����ʱ�䣺2022��4��11�� 
*  �޸�ʱ�䣺
*  ˵�����������
*
==============================================================================*/


#include <string.h>

#include "includes.h"

#define OS_TICK_QTY  1

//�������ȼ�
#define START_TASK_PRIO		   		3 
#define CAMMAND_TASK_PRIO    		4
#define MOVE_SPEED_TASK_PRIO 		4
#define ERR_TASK_PRIO        		5
#define POWEROFF_TASK_PRIO   		6    
#define CHG_TASK_PRIO        		7
#define TEST_TASK_PRIO    			8
#define PMU_TASK_PRIO	     	 		3   
#define HEAD_CTRL_TASK_PRIO  		10
#define AUTOCHARGE_TASK_PRIO		11
#define ULTRA_TASK_PRIO      		12
#define SEND_MDRV_TASK_PRIO   	13
#define MDRV_TASK_PRIO    			14
#define LIFTMOTO_TASK_PRIO			17
#define ENVIRON_TASK_PRIO				8
#define ENVIRON_QUERY_TASK_PRIO	19 

#define LED_TASK_PRIO		     		30    


//�����ջ��С	
#define START_STK_SIZE 		   		128  
#define ERR_STK_SIZE 		     		128 
#define POWEROFF_STK_SIZE    		128  
#define CHG_STK_SIZE         		128
#define TEST_STK_SIZE        		128
#define CAMMAND_STK_SIZE     		128
#define SEND_MDRV_STK_SIZE    	128
#define MDRV_STK_SIZE         	128
#define MOVE_SPEED_STK_SIZE	 		128
#define PMU_STK_SIZE 		     		128 
#define HEAD_CTRL_STK_SIZE 	 		128 
#define ULTRA_STK_SIZE 	     		128 
#define AUTOCHARGE_STK_SIZE			128
#define LIFTMOTO_STK_SIZE    		128
#define LED_STK_SIZE 		     		128 
#define ENVIRON_STK_SIZE				128
#define ENVIRON_QUERY_STK_SIZE	128

//������ƿ�
OS_TCB StartTaskTCB; 
OS_TCB  ERR_TASKTCB;		
OS_TCB  POWEROFF_TASKTCB;				
OS_TCB  CHG_TASKTCB;
OS_TCB  TEST_TASKTCB;
OS_TCB  CAMMAND_TASKTCB;
OS_TCB  MOVE_SPEED_TASKTCB;
OS_TCB  SEND_MDRV_TASKTCB;
OS_TCB  MDRV_TASKTCB;
OS_TCB  PMU_TASKTCB;  
OS_TCB  HEAD_CTRL_TASKTCB;    
OS_TCB  ULTRA_TASKTCB; 
OS_TCB  LIFTMOTO_TASKTCB;
OS_TCB  AUTOCHARGE_TASKTCB;
OS_TCB  LED_TASKTCB; 
OS_TCB	ENVIRON_TASKTCB;
OS_TCB	ENVIRON_QUERY_TASKTCB;

//�����ջ	
CPU_STK START_TASK_STK[START_STK_SIZE]; 
CPU_STK ERR_TASK_STK[ERR_STK_SIZE]; 
CPU_STK POWEROFF_TASK_STK[POWEROFF_STK_SIZE];  
CPU_STK CHG_TASK_STK[CHG_STK_SIZE];
CPU_STK TEST_TASK_STK[TEST_STK_SIZE];
CPU_STK CAMMAND_TASK_STK[CAMMAND_STK_SIZE];
CPU_STK MOVE_SPEED_TASK_STK[MOVE_SPEED_STK_SIZE];
CPU_STK PMU_TASK_STK[PMU_STK_SIZE]; 
CPU_STK HEAD_CTRL_TASK_STK[HEAD_CTRL_STK_SIZE]; 
CPU_STK ULTRA_TASK_STK[ULTRA_STK_SIZE];  
CPU_STK SEND_MDRV_TASK_STK[SEND_MDRV_STK_SIZE]; 
CPU_STK MDRV_TASK_STK[MDRV_STK_SIZE]; 
CPU_STK AUTOCHARGE_TASK_STK[AUTOCHARGE_STK_SIZE];  
CPU_STK LIFTMOTO_STK[LIFTMOTO_STK_SIZE];
CPU_STK ENVIRON_TASK_STK[ENVIRON_STK_SIZE];
CPU_STK ENVIRON_QUERY_TASK_STK[ENVIRON_QUERY_STK_SIZE];

CPU_STK LED_TASK_STK[LED_STK_SIZE];  

//��������
void Err_Handle_task(void *p_arg);    //�쳣��������
void Poweroff_task(void *p_arg);    	//�ػ���������
void Chg_task(void *p_arg);    				//�����������
void Cammand_task(void *p_arg);    		//�������
void Move_Speed_task(void *p_arg);   	//�ٶȴ���
void Test_task(void *p_arg);
void Head_Ctrl_task(void *p_arg);     //�ֶ�ͷ������
void Pmu_task(void *p_arg);     			//��Դ������
void Ultrasonic_task(void *p_arg);    //��������
void LiftMoto_task(void *p_arg);
void AutoCharge_task(void *p_arg);    //��������
void Environ_task(void *p_arg);
void Environ_Query_task(void *p_arg);
void Send_Mdrv_task(void *p_arg);
void Mdrv_task(void *p_arg);
void LED_task(void *p_arg);     			//led���Ժ���

OS_TMR 	tmr1;		//��ʱ��1
void tmr1_callback(void *p_tmr, void *p_arg); 	//��ʱ��1�ص�����

OS_SEM UsartRxFromPC_SEM;		//����һ���ź���������USART1������ͬ��
OS_SEM UsartToTest_SEM;
OS_SEM UsartEnviron_SEM;
OS_SEM UsartToDrv_SEM;

OS_MUTEX CMD_MUTEX;		//����һ�������ź���


//��ʼ������
void start_task(void *p_arg)
{
	OS_ERR err;
	CPU_SR_ALLOC();
	p_arg = p_arg;       

	CPU_Init();
	
	#if OS_CFG_STAT_TASK_EN > 0u                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                     
  OSStatTaskCPUUsageInit(&err);  		//ͳ������                
	#endif

	#ifdef CPU_CFG_INT_DIS_MEAS_EN		//���ʹ���˲����жϹر�ʱ��
  CPU_IntDisMeasMaxCurReset();	
	#endif

	#if	OS_CFG_SCHED_ROUND_ROBIN_EN  	//��ʹ��ʱ��Ƭ��ת��ʱ��
	//ʹ��ʱ��Ƭ��ת���ȹ���,ʱ��Ƭ����Ϊ1��ϵͳʱ�ӽ��ģ���1*5=5ms
	OSSchedRoundRobinCfg(DEF_ENABLED, 1, &err);  
	#endif	
	
/*******************************************************************************
*	������: OSTmrCreate
*	��  ��: ������ʱ��
* ��  ��: ��
*	��  ��: ��
*/	
	OSTmrCreate((OS_TMR*  )&tmr1,										//��ʱ��1
              (CPU_CHAR*)"tmr1",									//��ʱ������
              (OS_TICK	)10,			 								//10*10=100ms��������ʱʱ��
              (OS_TICK	)10,          						//10*10=100ms��ʱʱ��
              (OS_OPT		)OS_OPT_TMR_PERIODIC, 		//����ģʽ
              (OS_TMR_CALLBACK_PTR)tmr1_callback,	//��ʱ��1�ص�����
              (void*    )0,												//����Ϊ0
              (OS_ERR*  )&err);										//���صĴ�����
								
	OS_CRITICAL_ENTER();	//�����ٽ���
								
/*******************************************************************************
*	������: OSSemCreate
*	��  ��: �����ź���,���ڹ���ͬ��
* ��  ��: ��
*	��  ��: ��
*/				

	OSSemCreate((OS_SEM*)&UsartRxFromPC_SEM, (CPU_CHAR*)"FromPC_SEM", (OS_SEM_CTR)0, (OS_ERR*)&err);							
	OSSemCreate((OS_SEM*)&UsartEnviron_SEM, (CPU_CHAR*)"UsartEnviron_SEM", (OS_SEM_CTR)0, (OS_ERR*)&err);	
 	OSSemCreate((OS_SEM*)&UsartToTest_SEM, (CPU_CHAR*)"UsartToTest_SEM", (OS_SEM_CTR)0, (OS_ERR*)&err);	
 	OSSemCreate((OS_SEM*)&UsartToDrv_SEM, (CPU_CHAR*)"UsartToDrv_SEM", (OS_SEM_CTR)0, (OS_ERR*)&err);		
							
/*******************************************************************************
*	������: OSSemCreate
*	��  ��: ���������ź���,���ڹ���ͬ��
* ��  ��: ��
*	��  ��: ��
*/		
//	����һ�������ź���
	OSMutexCreate((OS_MUTEX*)&CMD_MUTEX,
				        (CPU_CHAR*)"TEST_MUTEX",
                (OS_ERR*	)&err);			
								
/*******************************************************************************
*	������: OSTaskCreate
*	��  ��: ���񴴽�����
* ��  ��: ��
*	��  ��: ��
*/			
	#ifdef ERR_TASK_OS	 	   

	OSTaskCreate((OS_TCB*     )&ERR_TASKTCB,       
	             (CPU_CHAR*   )"Err_Handle_task",
							 (OS_TASK_PTR )Err_Handle_task,
							 (void*       )0,
							 (OS_PRIO     )ERR_TASK_PRIO,
							 (CPU_STK*    )&ERR_TASK_STK[0],
							 (CPU_STK_SIZE)ERR_STK_SIZE/10,
							 (CPU_STK_SIZE)ERR_STK_SIZE,
							 (OS_MSG_QTY  )0,
							 (OS_TICK     )OS_TICK_QTY,
							 (void*       )0,
							 (OS_OPT      )OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR,
	             (OS_ERR*     )&err);
	#endif 		
			
	#ifdef POWEROFF_TASK_OS									
//	power off task  
	OSTaskCreate((OS_TCB*		  )&POWEROFF_TASKTCB,       
	             (CPU_CHAR*   )"Poweroff_task",
							 (OS_TASK_PTR )Poweroff_task,
							 (void*       )0,
							 (OS_PRIO     )POWEROFF_TASK_PRIO,
							 (CPU_STK*    )&POWEROFF_TASK_STK[0],
							 (CPU_STK_SIZE)POWEROFF_STK_SIZE/10,
							 (CPU_STK_SIZE)POWEROFF_STK_SIZE,
							 (OS_MSG_QTY  )0,
							 (OS_TICK     )OS_TICK_QTY,
							 (void*       )0,
							 (OS_OPT      )OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR,
	             (OS_ERR*     )&err);
	#endif 		

	#ifdef CHG_TASK_OS											
// charging  task	 						
	OSTaskCreate((OS_TCB*     )&CHG_TASKTCB,
							 (CPU_CHAR*   )"Chg_task",
							 (OS_TASK_PTR )Chg_task,
							 (void*       )0,
							 (OS_PRIO     )CHG_TASK_PRIO,
               (CPU_STK*    )&CHG_TASK_STK[0],
               (CPU_STK_SIZE)CHG_STK_SIZE/10,
							 (CPU_STK_SIZE)CHG_STK_SIZE,
							 (OS_MSG_QTY  )0,
							 (OS_TICK     )OS_TICK_QTY,
							 (void*       )0,
							 (OS_OPT      )OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR,
							 (OS_ERR*     )&err);
	
	#endif
								
	#ifdef TEST_TASK_OS											
// charging  task	 						
	OSTaskCreate((OS_TCB*     )&TEST_TASKTCB,
							 (CPU_CHAR*   )"Test_task",
							 (OS_TASK_PTR )Test_task,
							 (void*       )0,
							 (OS_PRIO     )TEST_TASK_PRIO,
               (CPU_STK*    )&TEST_TASK_STK[0],
               (CPU_STK_SIZE)TEST_STK_SIZE/10,
							 (CPU_STK_SIZE)TEST_STK_SIZE,
							 (OS_MSG_QTY  )0,
							 (OS_TICK     )OS_TICK_QTY,
							 (void*       )0,
							 (OS_OPT      )OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR,
							 (OS_ERR*     )&err);
	
	#endif
								
	#ifdef CAMMAND_TASK_OS							
// command handle task	 							
	OSTaskCreate((OS_TCB*     )&CAMMAND_TASKTCB,
							 (CPU_CHAR*   )"Cammand_task",
							 (OS_TASK_PTR )Cammand_task,
							 (void*       )0,
							 (OS_PRIO     )CAMMAND_TASK_PRIO,
               (CPU_STK*    )&CAMMAND_TASK_STK[0],
               (CPU_STK_SIZE)CAMMAND_STK_SIZE/10,
					 		 (CPU_STK_SIZE)CAMMAND_STK_SIZE,
						   (OS_MSG_QTY  )0,
							 (OS_TICK     )OS_TICK_QTY,
							 (void*       )0,
							 (OS_OPT      )OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR,
							 (OS_ERR*     )&err);
	#endif 	

	#ifdef SEND_DMRV_TASK_OS							
// command handle task	 							
	OSTaskCreate((OS_TCB*     )&SEND_MDRV_TASKTCB,
							 (CPU_CHAR*   )"Send_Mdrv_task",
							 (OS_TASK_PTR )Send_Mdrv_task,
							 (void*       )0,
							 (OS_PRIO     )SEND_MDRV_TASK_PRIO,
               (CPU_STK*    )&SEND_MDRV_TASK_STK[0],
               (CPU_STK_SIZE)SEND_MDRV_STK_SIZE/10,
							 (CPU_STK_SIZE)SEND_MDRV_STK_SIZE,
							 (OS_MSG_QTY  )0,
							 (OS_TICK     )OS_TICK_QTY,
							 (void*       )0,
							 (OS_OPT      )OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR,
							 (OS_ERR*     )&err);
	#endif 
								
	#ifdef DMRV_TASK_OS							
// command handle task	 							
	OSTaskCreate((OS_TCB*     )&MDRV_TASKTCB,
							 (CPU_CHAR*   )"Mdrv_task",
							 (OS_TASK_PTR )Mdrv_task,
							 (void*       )0,
							 (OS_PRIO     )MDRV_TASK_PRIO,
               (CPU_STK*    )&MDRV_TASK_STK[0],
               (CPU_STK_SIZE)MDRV_STK_SIZE/10,
							 (CPU_STK_SIZE)MDRV_STK_SIZE,
							 (OS_MSG_QTY  )0,
							 (OS_TICK     )OS_TICK_QTY,
							 (void*       )0,
							 (OS_OPT      )OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR,
							 (OS_ERR*     )&err);
	#endif 
								
	#ifdef MOVE_SPEED_TASK_OS							
// command handle task	 							
	OSTaskCreate((OS_TCB*     )&MOVE_SPEED_TASKTCB,
							 (CPU_CHAR*   )"Move_Speed_task",
							 (OS_TASK_PTR )Move_Speed_task,
							 (void*       )0,
							 (OS_PRIO     )MOVE_SPEED_TASK_PRIO,
               (CPU_STK*    )&MOVE_SPEED_TASK_STK[0],
               (CPU_STK_SIZE)MOVE_SPEED_STK_SIZE/10,
							 (CPU_STK_SIZE)MOVE_SPEED_STK_SIZE,
							 (OS_MSG_QTY  )0,
							 (OS_TICK     )OS_TICK_QTY,
							 (void*       )0,
							 (OS_OPT      )OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR,
							 (OS_ERR*     )&err);
	#endif 	

	#ifdef PMU_TASK_OS								
// PMU handle task	 					
	OSTaskCreate((OS_TCB*     )&PMU_TASKTCB,
							 (CPU_CHAR*   )"Pmu_task",
							 (OS_TASK_PTR )Pmu_task,
							 (void*       )0,
							 (OS_PRIO     )PMU_TASK_PRIO,
               (CPU_STK*    )&PMU_TASK_STK[0],
               (CPU_STK_SIZE)PMU_STK_SIZE/10,
							 (CPU_STK_SIZE)PMU_STK_SIZE,
							 (OS_MSG_QTY  )0,
							 (OS_TICK     )OS_TICK_QTY,
							 (void*       )0,
							 (OS_OPT      )OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR,
							 (OS_ERR*     )&err);	
	#endif
								
	#ifdef HEAD_CTRL_TASK_OS
// head contol handle  task	
	OSTaskCreate((OS_TCB*     )&HEAD_CTRL_TASKTCB,
							 (CPU_CHAR*   )"Head_Ctrl_task",
							 (OS_TASK_PTR )Head_Ctrl_task,
							 (void*       )0,
							 (OS_PRIO     )HEAD_CTRL_TASK_PRIO,
               (CPU_STK*    )&HEAD_CTRL_TASK_STK[0],
               (CPU_STK_SIZE)HEAD_CTRL_STK_SIZE/10,
							 (CPU_STK_SIZE)HEAD_CTRL_STK_SIZE,
							 (OS_MSG_QTY  )0,
							 (OS_TICK     )OS_TICK_QTY,
							 (void*       )0,
							 (OS_OPT      )OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR,
							 (OS_ERR*     )&err);		
	#endif
							
	#ifdef ULTRA_TASK_OS
// ultra handle  task					
	OSTaskCreate((OS_TCB*     )&ULTRA_TASKTCB,
							 (CPU_CHAR*   )"Ultrasonic_task",
							 (OS_TASK_PTR )Ultrasonic_task,
							 (void*       )0,
							 (OS_PRIO     )ULTRA_TASK_PRIO,
               (CPU_STK*    )&ULTRA_TASK_STK[0],
               (CPU_STK_SIZE)ULTRA_STK_SIZE/10,
							 (CPU_STK_SIZE)ULTRA_STK_SIZE,
							 (OS_MSG_QTY  )0,
							 (OS_TICK     )OS_TICK_QTY,
							 (void*       )0,
							 (OS_OPT      )OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR,
							 (OS_ERR*     )&err);

	#endif
		
	#ifdef AUTOCHARGE_TASK_OS
//AutoCharge task						
	OSTaskCreate((OS_TCB*     )&AUTOCHARGE_TASKTCB,
							 (CPU_CHAR*   )"AutoCharge_task",
							 (OS_TASK_PTR )AutoCharge_task,
							 (void*       )0,
							 (OS_PRIO     ) AUTOCHARGE_TASK_PRIO,
               (CPU_STK*    )&AUTOCHARGE_TASK_STK[0],
               (CPU_STK_SIZE)AUTOCHARGE_STK_SIZE/10,
							 (CPU_STK_SIZE)AUTOCHARGE_STK_SIZE,
							 (OS_MSG_QTY  )0,
							 (OS_TICK     )OS_TICK_QTY,
							 (void*       )0,
							 (OS_OPT      )OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR,
							 (OS_ERR*     )&err);
	#endif

	#ifdef LIFTMOTO_TASK_OS
//  LED test task	
	OSTaskCreate((OS_TCB*     )&LIFTMOTO_TASKTCB,       
	             (CPU_CHAR*   )"LiftMoto_task",
							 (OS_TASK_PTR )LiftMoto_task,
							 (void*       )0,
							 (OS_PRIO     )LIFTMOTO_TASK_PRIO,
							 (CPU_STK*    )&LIFTMOTO_STK[0],
							 (CPU_STK_SIZE)LIFTMOTO_STK_SIZE/10,
							 (CPU_STK_SIZE)LIFTMOTO_STK_SIZE,
							 (OS_MSG_QTY  )0,
							 (OS_TICK     )OS_TICK_QTY,
							 (void*       )0,
							 (OS_OPT      )OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR,
	             (OS_ERR*     )&err);		
	#endif
								
	#ifdef ENVIRON_TASK_OS								
// environmental task	
	OSTaskCreate((OS_TCB*     )&ENVIRON_TASKTCB,       
	             (CPU_CHAR*   )"Environ_task",
							 (OS_TASK_PTR )Environ_task,
							 (void*       )0,
							 (OS_PRIO     )ENVIRON_TASK_PRIO,
							 (CPU_STK*    )&ENVIRON_TASK_STK[0],
							 (CPU_STK_SIZE)ENVIRON_STK_SIZE/10,
							 (CPU_STK_SIZE)ENVIRON_STK_SIZE,
							 (OS_MSG_QTY  )0,
							 (OS_TICK     )OS_TICK_QTY,
							 (void*       )0,
							 (OS_OPT      )OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR,
	             (OS_ERR*     )&err);
	#endif

	#ifdef ENVIRON_QUERY_TASK_OS								
// environmental task	
	OSTaskCreate((OS_TCB*     )&ENVIRON_QUERY_TASKTCB,       
	             (CPU_CHAR*   )"Environ_Query_task",
							 (OS_TASK_PTR )Environ_Query_task,
							 (void*       )0,
							 (OS_PRIO     )ENVIRON_QUERY_TASK_PRIO,
							 (CPU_STK*    )&ENVIRON_QUERY_TASK_STK[0],
							 (CPU_STK_SIZE)ENVIRON_QUERY_STK_SIZE/10,
							 (CPU_STK_SIZE)ENVIRON_QUERY_STK_SIZE,
							 (OS_MSG_QTY  )0,
							 (OS_TICK     )OS_TICK_QTY,
							 (void*       )0,
							 (OS_OPT      )OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR,
	             (OS_ERR*     )&err);		
	#endif

	#ifdef LED_TASK_OS								
// LED test task						
	OSTaskCreate((OS_TCB*     )&LED_TASKTCB,
	             (CPU_CHAR*   )"LED_task",
							 (OS_TASK_PTR )LED_task,
							 (void*       )0,
							 (OS_PRIO     )LED_TASK_PRIO,
							 (CPU_STK*    )&LED_TASK_STK[0],
							 (CPU_STK_SIZE)LED_STK_SIZE/10,
	             (CPU_STK_SIZE)LED_STK_SIZE,
							 (OS_MSG_QTY  )0,
							 (OS_TICK     )OS_TICK_QTY,
							 (void*       )0,                                     
							 (OS_OPT      )OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR,
							 (OS_ERR*     )&err);
	#endif
								
	OS_CRITICAL_EXIT();						//�˳��ٽ���
	OSTaskDel((OS_TCB*)0, &err);	//ɾ��start_task��������
}

/*=============================================================================
*  ������ ��Err_Handle_task
*  ��   �ߣ�hrx
*  ����ʱ�䣺2022��4��11�� 
*  �޸�ʱ�䣺
*  ��   �룺
*           
*  ��   ����
*  ˵   �����쳣��������
*/
void Err_Handle_task(void *p_arg)
{
	
	OS_ERR err;
	static uint8_t drv_flag = 0;
	static uint16_t drv_cnt = 0;
	static uint8_t oc_cnt = 0;
	p_arg = p_arg;
	while(1)
	{
		 IWDG_Feed();		//ι��
		
//		printf("Disconnect_flag : %d , %d,  %d\r\n", UsartToPC.Disconnect_flag, UsartToDrv.Disconnect_flag, Robot_Sys.Comm_break_flag);
//    printf("timeout:  %d, %d\r\n", Robot_Sys.Speed_Timeout_cnt, Robot_Sys.Odom_Timeout_cnt);
//		printf("\r\n");
		
		//�͹��ػ�ͨ��0.5Sû���յ�������ֹͣ
		if(UsartToPC.Comm_TimeOut >= 1000)    	
		{
			UsartToPC.Comm_TimeOut = 1000;
			UsartToPC.Disconnect_flag = 1;
		}
		//��������ͨ��0.5Sû���յ�������ֹͣ
		if(UsartToDrv.Comm_TimeOut >= 1000)			
		{
			UsartToDrv.Comm_TimeOut = 1000;
			UsartToDrv.Disconnect_flag = 1;
		}
		
		if(Robot_Sys.Speed_Timeout_cnt >= 5000 || Robot_Sys.Odom_Timeout_cnt >= 5000)
		{
			Robot_Sys.Speed_Timeout_cnt = 5000;
			Robot_Sys.Odom_Timeout_cnt = 5000;
			Robot_Sys.Comm_break_flag = 1;
		}
		else if(Robot_Sys.Speed_Timeout_cnt <= 1000 && Robot_Sys.Odom_Timeout_cnt <= 1000)
		{
			Robot_Sys.Comm_break_flag = 0;
		}
		//�޷�ͨѶ���ó���
		if(UsartToPC.Disconnect_flag == 1 || Robot_Sys.Comm_break_flag == 1)		
		{
			//��ͣ״̬����
			if(Robot_Sys.Mergency_Stop_flag == true)		
			{
				if(Moto.en_sta == 0)
				{
					Send_mdrv_en_set(1,1);
				}
				else
				{
					Moto.set_lear = 0;
					Moto.set_angle = 0;
					Send_speed_set(Moto.set_lear, Moto.set_angle);
				}
			}
			else
			{
				if(Moto.stop_sta == 1)
				{
					if(Moto.en_sta == 1)
					{
						Send_mdrv_en_set(0,0);	 
					}
					else
					{
						Moto.set_lear = 0;
						Moto.set_angle = 0;
						Send_speed_set(Moto.set_lear, Moto.set_angle);
					}
				}
				else
				{
					Moto.set_lear = 0;
					Moto.set_angle = 0;
					Send_speed_set(Moto.set_lear, Moto.set_angle);
				}
			}
		}
		
		if(drv_flag == 0)
		{
			if(UsartToDrv.Disconnect_flag == 1)
			{
				EN_MDRV_DISABLE;
				drv_flag = 1;
				drv_cnt = 0;
			}
		}
		else
		{
			drv_cnt ++;
			if(drv_cnt > 100)
			{
				drv_cnt = 0;
				EN_MDRV_ENABLE;
				Moto.en_sta = 0;
			}
			if(UsartToDrv.Disconnect_flag == 0) drv_flag = 0;
		}
		
		if(Moto.over_cur_flag == true)		//��������
		{
			if(Pms.Moto_Cur > 15000)
			{
				oc_cnt ++;
				if(oc_cnt > 10)
				{
					oc_cnt = 0;
					EN_MDRV_DISABLE;
				}
			}
		}
		else oc_cnt = 0;

		OSTimeDlyHMSM(0, 0, 0, 100, OS_OPT_TIME_HMSM_STRICT, &err); //��ʱ300ms
	}
}

/*=============================================================================
*  ������ ��Poweroff_task
*  ��   �ߣ�hrx
*  ����ʱ�䣺2022��4��11�� 
*  �޸�ʱ�䣺
*  ��   �룺
*           
*  ��   ����
*  ˵   �����ػ�����
*/
void Poweroff_task(void *p_arg)
{
	static uint16_t key_cnt = 0;
	static uint16_t poweroff_cnt = 0;
	OS_ERR err;
	p_arg = p_arg;
  
	while(1)
	{
		if(Robot_Sys.PowerOff_flag == 0)
		{
			if(POWERKEY_IN == 1)		//�����ػ�
			{
				key_cnt ++;
				if(key_cnt > 15)
				{
					key_cnt = 0;
					Robot_Sys.PowerOff_flag =1;
					Robot_Sys.mBeepStatus.BeepMode = 3;
				}
			}
			else
			{
				key_cnt = 0;
				Robot_Sys.PowerOff_flag =0;
			}
		}
		else
		{
			key_cnt ++;
			if((key_cnt > 50 && Pms.Bat_Current > -1000) || key_cnt > 200)
			{
			  poweroff_cnt ++;
				if(poweroff_cnt > 10)
				{
					poweroff_cnt = 0;
					EN24_DISABLE;
					EN_MDRV_DISABLE;
					OSTimeDlyHMSM(0, 0, 0,500, OS_OPT_TIME_HMSM_STRICT, &err); //��ʱ300ms
					EN_OUT_DISABLE;
				}
			}
		}
		OSTimeDlyHMSM(0, 0, 0, 200, OS_OPT_TIME_HMSM_STRICT, &err); //��ʱ300ms
	}
}

/*=============================================================================
*  ������ ��Chg_task
*  ��   �ߣ�hrx
*  ����ʱ�䣺2022��4��11�� 
*  �޸�ʱ�䣺
*  ��   �룺
*           
*  ��   ����
*  ˵   �����������
*/
void Chg_task(void *p_arg)    //��紦����
{
	static uint8_t cha_sta = 0;
	static uint8_t check_cnt = 0;
//	static uint8_t last_sta = 0;
	OS_ERR err;
	p_arg = p_arg;
	while(1)
	{
		if((cha_sta & 0x10) == 0){
			//�����ѹ����26V 
			if(ADC_Filter_Value[2] >= 2600 && AutoCharge.ReChg_flag == 0)
			{
				delay_ms(50);
//				Moto.cmd_addr1 = 0x2610;
//				Moto.cmd_addr2 = 0x3610;
//				Moto.set_left_pwm = -60;
//			  Moto.set_right_pwm = -60;
				AutoCharge.set_lear = 0;
				AutoCharge.set_angle = 0;
				delay_ms(100);
				if(ADC_Filter_Value[2] >= 2600 && AutoCharge.ReChg_flag == 0)
				{
					cha_sta |= 0x10;   //charging flag
					AutoCharge.chg_flag = 1;
					AutoCharge.chg_fail = 0;
					AutoCharge.rechg_cnt = 0;
					EN_CHG_ENABLE;     //���ʹ��
					check_cnt = 0;
					//Pms.Bat_Sta |= 0x01;   //chargine
					Robot_Sys.AutoCharge_task_flag = false;
					AutoCharge.AutoChg_Cmd = false;
				}
			}
			else         				//δ���
			{
				Pms.Bat_Sta &= 0xfe;
				EN_CHG_DISABLE;		
				cha_sta = 0;
				if(Robot_Sys.Remote_flag == false)
				{
					if(AutoCharge.StartUp_Flag == 1)
					{
						Get_Charger_Code();
						delay_ms(200);
						AutoCharge.ReChg_flag = 1;
						AutoCharge.Chg_Sta = 0;
						Robot_Sys.AutoCharge_task_flag = true;
						Robot_Sys.Remote_flag = false;
						AutoCharge.NotFind_Flag = false;
						Robot_Sys.Last_Task = CHG_TASK;
						OSTaskResume((OS_TCB*)&AUTOCHARGE_TASKTCB, &err);	//��ָ�����				
					}
				}
				else
				{
					AutoCharge.ReChg_flag = 0;
					AutoCharge.StartUp_Flag = 0;
				}
			}			
		}
		else                 //charging
		{
			if((cha_sta & 0x20) == 0)
			{
				//currnet > -375A��-187.5ma�� ,charger is not connected
				if(ADC_Filter_Value[3] < 1990)
				{
				 	check_cnt ++;
				  if(check_cnt > 10)
					{
						check_cnt = 0;
						cha_sta |= 0x20;
						EN_CHG_DISABLE;
					}
			  }
				//��ѹ����26.5,ͬʱ����С��1A��993.75ma��
				else if(ADC_Filter_Value[3] < 2053 &&  ADC_Filter_Value[0] < 2650)
				{
				}
				else
				{
					Pms.Bat_Sta |= 0x01;   //chargine
					check_cnt = 0;
				}
			}
			else
			{
				//voltage < 12V ,  no charger 
				if(ADC_Filter_Value[2] <= 2600)
				{
					delay_ms(800);
					if(ADC_Filter_Value[2] <= 2600)
					{
						cha_sta = 0;
						EN_CHG_DISABLE;
						Pms.Bat_Sta &= 0xfe;   //no chargine
					}
				}
				else
				{
					check_cnt ++;
					//2 sec later ,if input > 28V, charging
					if(check_cnt > 5)
					{
						check_cnt = 0;
						cha_sta &= 0xdf;
						EN_CHG_ENABLE;
					}
				}
			}
		}
		OSTimeDlyHMSM(0, 0, 0, 100, OS_OPT_TIME_HMSM_STRICT, &err); //��ʱ300ms
	}
}

/*=============================================================================
*  ������ ��AutoCharge_task
*  ��   �ߣ�hrx
*  ����ʱ�䣺2022��4��11�� 
*  �޸�ʱ�䣺
*  ��   �룺
*           
*  ��   ����
*  ˵   �����Զ��������
*/
void AutoCharge_task(void *p_arg)
{
	OS_ERR err;
	p_arg = p_arg;
	while(1)
	{
		if(Robot_Sys.AutoCharge_task_flag == true)
		{
			if((Pms.Bat_Sta & 0x01)== 0)		//�ǳ����
			{
				if(AutoCharge.ReChg_flag == 1 && AutoCharge.chg_fail != 0x02)
				{
					AutoCharge.StartUp_Flag = 0;
					AutoCharge.set_lear = 200;
					AutoCharge.set_angle = 0;
					//---------------------�Լ�����������ʹ��
					Moto.set_left_pwm = 100;
					Moto.set_right_pwm = 100;
					Moto.cmd_addr1 = 0x2620;
					Moto.cmd_addr2 = 0x3620;
					//---------------------
					delay_ms(2000);
					AutoCharge.set_lear = 0;
					AutoCharge.set_angle = 0;
					//---------------------�Լ�����������ʹ��
					Moto.set_left_pwm = 0;
					Moto.set_right_pwm = 0;
					//---------------------
					delay_ms(200);
					AutoCharge.ReChg_flag = 0;
					AutoCharge.chg_flag = 0;
					AutoCharge.rechg_cnt++;
					if(AutoCharge.rechg_cnt >= 3)
					{
						AutoCharge.chg_fail = 0x02;		//0000 0010
						Robot_Sys.AutoCharge_task_flag = false;
						Robot_Sys.Last_Task = REMOTE_TASK;
						AutoCharge.rechg_cnt = 0;
						AutoCharge.chg_flag = 0;
					}
				}
				else
				{
					AutoCharge_Processing();		//�����Զ���紦����
					if(AutoCharge.NotFind_Flag == true)
					{
						Robot_Sys.AutoCharge_task_flag = false;
					}
				}
			}
			else
			{
				//---------------------�Լ�����������ʹ��
				Moto.cmd_addr1 = 0x2610;
				Moto.cmd_addr2 = 0x3610;
				Moto.set_left_pwm = -60;
				Moto.set_right_pwm = -60;
				//---------------------
				//Robot_Sys.AutoCharge_task_flag = false;
			}
		}
		else
		{
			AutoCharge.set_lear = 0;
			AutoCharge.set_angle = 0;
			//---------------------�Լ�����������ʹ��
			Moto.set_left_pwm = 0;
			Moto.set_right_pwm = 0;
			//---------------------
			OSTaskSuspend((OS_TCB*)&AUTOCHARGE_TASKTCB, &err);//��������
		}
		OSTimeDlyHMSM(0, 0, 0, 200, OS_OPT_TIME_HMSM_STRICT, &err); //��ʱ200ms
	}
}

/*=============================================================================
*  ������ ��Cammand_task
*  ��   �ߣ�hrx
*  ����ʱ�䣺2022��4��11�� 
*  �޸�ʱ�䣺
*  ��   �룺
*           
*  ��   ����
*  ˵   �������ػ������������
*/
void Cammand_task(void *p_arg)
{
	static uint16_t last_left_set = 0;
	static uint16_t last_right_set = 0;		
	int16_t lear_tmp;
	int16_t angular_tmp;
	int16_t real_lear;
	int16_t real_angle;
  OS_ERR err;
	p_arg = p_arg;
	
	while(1)
	{	
		#ifdef ROBOT_YZ01		
		OSSemPend(&UsartRxFromPC_SEM, 0, OS_OPT_PEND_BLOCKING, 0, &err); //�����ź���
		#endif
		AnalysisCMD();

		if(Head_Status.PSC_Dir != 0 && Robot_Sys.Psc_Task_flag == false)
		{
			Robot_Sys.Psc_Task_flag = true;
		  OSTaskResume((OS_TCB*)&HEAD_CTRL_TASKTCB, &err);	//��ָ�����
		}
		
		if((Lift_Moto.Cmd != LIFT_STOP || Lift_Moto.Set_Height != 0xffff) && Robot_Sys.Lift_Task_flag == false)
		{
			Robot_Sys.Lift_Task_flag = true;
			OSTaskResume((OS_TCB*)&LIFTMOTO_TASKTCB, &err);		//��ָ�����
		}
		
		if(AutoCharge.AutoChg_Cmd == true && Robot_Sys.AutoCharge_task_flag == false && Robot_Sys.Remote_flag == true)
		{
		  Robot_Sys.Last_Task = CHG_TASK;
			AutoCharge.Chg_Sta = 0;
			Robot_Sys.AutoCharge_task_flag = true;
			Robot_Sys.Remote_flag = false;
			AutoCharge.NotFind_Flag = false;
			OSTaskResume((OS_TCB*)&AUTOCHARGE_TASKTCB, &err);	//��ָ�����
		}	
		#ifndef ROBOT_YZ01		
		OSTimeDlyHMSM(0, 0, 0, 3, OS_OPT_TIME_HMSM_STRICT, &err); //��ʱ1s
		#endif
	}
}

/*=============================================================================
*  ������ ��Send_Mdrv_task
*  ��   �ߣ�hrx
*  ����ʱ�䣺2022��4��11�� 
*  �޸�ʱ�䣺
*  ��   �룺
*           
*  ��   ����
*  ˵   ��������������·�����
*/
void Send_Mdrv_task(void *p_arg)
{
	OS_ERR err;
	uint8_t i;
	static uint8_t cnt = 0;
	uint16_t addr1;
	uint16_t addr2;
	p_arg = p_arg;
	
	while(1)
	{
		if(Robot_Sys.AutoCharge_task_flag == true)
		{
			addr1 = 0x2610;
			addr2 = 0x3610;
		}
		else
		{
			addr1 = 0x2620;
			addr2 = 0x3620;
			Moto.set_left_pwm = 0;
			Moto.set_right_pwm = 0;
		}
			
		Send_wr_all_mdrv_cmd(addr1, addr2, Moto.set_lear, Moto.set_angle, Moto.set_left_pwm, Moto.set_right_pwm);
		OSTimeDlyHMSM(0, 0, 0, 25, OS_OPT_TIME_HMSM_STRICT, &err); //��ʱ1s
	}
}

/*=============================================================================
*  ������ ��Mdrv_task
*  ��   �ߣ�hrx
*  ����ʱ�䣺2022��4��11�� 
*  �޸�ʱ�䣺
*  ��   �룺
*           
*  ��   ����
*  ˵   ��������������������
*/
void Mdrv_task(void *p_arg)
{
	OS_ERR err;
	uint8_t i;
	p_arg = p_arg;
	
	while(1)
	{
		Moto_mdrv_analysis();
		OSTimeDlyHMSM(0, 0, 0, 5, OS_OPT_TIME_HMSM_STRICT, &err); //��ʱ1s
	}
}

/*=============================================================================
*  ������ ��Mdrv_task
*  ��   �ߣ�hrx
*  ����ʱ�䣺2022��4��11�� 
*  �޸�ʱ�䣺
*  ��   �룺
*           
*  ��   ����
*  ˵   ��������������������
*/
void Move_Speed_task(void *p_arg)
{
	static uint8_t last_flag = 0;
	static uint16_t cnt = 0;
	static uint8_t last_Mergency_Stop = false;
	OS_ERR err;
	p_arg = p_arg;
	
	while(1)
	{

	 OSTimeDlyHMSM(0, 0, 0, 500, OS_OPT_TIME_HMSM_STRICT, &err); //��ʱ1s
	}
}

/*=============================================================================
*  ������ ��Pmu_task
*  ��   �ߣ�hrx
*  ����ʱ�䣺2022��4��11�� 
*  �޸�ʱ�䣺
*  ��   �룺
*           
*  ��   ����
*  ˵   ������Դ��������
*/
void Pmu_task(void *p_arg)    //��Դ������
{
	OS_ERR err;
	uint8_t i;
	static uint8_t cnt;
	static uint16_t min_cnt = 0;
	p_arg = p_arg;
	
	while(1)
	{
	  Read_PMUData();
		Moto.over_cur_flag = Get_Drv_OverCur_Flag(Pms.Moto_Cur, Pms.Bat_Current);
		
		//10���Ӱѵ���дһ��
		min_cnt ++;
		if(min_cnt > 600)
		{
			min_cnt = 0;
			AT24CXX_WriteOneByte(4,(Pms.Capacity_mah>>8) & 0x00ff);	//��ǰ����
			AT24CXX_WriteOneByte(5,Pms.Capacity_mah & 0x00ff);
		}
		
//		if(last_lever_offset != Head_Status.Level_Offset)
//		{
//			printf("write level\r\n");
//			AT24CXX_WriteOneByte(6, Head_Status.Level_Offset);   //��ǰ����
//			OSTimeDlyHMSM(0,0,0,10,OS_OPT_TIME_HMSM_STRICT, &err); //��ʱ1s
//			//last_lever_offset = Head_Status.Level_Offset;
//			last_lever_offset = AT24CXX_ReadOneByte(6);
//		}
//		if(last_pitch_offset != Head_Status.Pitch_Offset)
//		{
//			//last_pitch_offset = Head_Status.Pitch_Offset;
//			printf("write pitch\r\n");
//		  AT24CXX_WriteOneByte(7,Head_Status.Pitch_Offset);
//			OSTimeDlyHMSM(0,0,0,10,OS_OPT_TIME_HMSM_STRICT,&err); //��ʱ1s
//			last_pitch_offset = AT24CXX_ReadOneByte(7);
//		}
//    printf("\r\n");
	
//		printf("ad_buf: ");
//		for(i = 0;i<6;i++){
//			printf(" %d ",ADC_Filter_Value[i]);
//		}
//		printf("\r\n");
//		printf("Bat_Vol:%d\r\n",Pms.Bat_Voltage);
//		printf("Chg_Vol:%d\r\n",Pms.Input_Voltage);
//		printf("Chg_Cur:%d, %d\r\n",ADC_Filter_Value[3],Pms.Bat_Current);
//		printf("moto_over: %d,%d\r\n",Pms.Moto_Cur,Moto.over_cur_flag);
//		printf("stop_vol:%d\r\n",Pms.Moto_Voltage);
//		printf("capacity:%d\r\n",Pms.Capacity);
//		printf("batt_sta = %02x\r\n",Pms.Bat_Sta );
//		printf("\r\n");
	
	  OSTimeDlyHMSM(0, 0, 1, 0, OS_OPT_TIME_HMSM_STRICT, &err); //��ʱ1s
	}
}

/*=============================================================================
*  ������ ��Head_Ctrl_task
*  ��   �ߣ�hrx
*  ����ʱ�䣺2022��4��11�� 
*  �޸�ʱ�䣺
*  ��   �룺
*           
*  ��   ����
*  ˵   ����ͷ����������
*/

void Head_Ctrl_task(void *p_arg)
{
	static uint16_t task_cnt = 0;
	OS_ERR err;
	p_arg = p_arg;

	while(1)
	{
		Head_Control_Done();	 //ͷ������
		//ͷ��ֹͣ
		if(Head_Status.PSC_Dir == 0 )
		{
			Robot_Sys.Psc_Task_flag = false;
			OSTaskSuspend((OS_TCB*)&HEAD_CTRL_TASKTCB, &err);	//����ͷ����������
		}
		OSTimeDlyHMSM(0, 0, 0, 20, OS_OPT_TIME_HMSM_STRICT, &err); //��ʱ1s
	}
}

/*=============================================================================
*  ������ ��Ultrasonic_task
*  ��   �ߣ�hrx
*  ����ʱ�䣺2022��4��11�� 
*  �޸�ʱ�䣺
*  ��   �룺
*           
*  ��   ����
*  ˵   ����������������
*/

void Ultrasonic_task(void *p_arg)
{
	OS_ERR err;
	static uint16_t charge_overtime = 0;
	static uint8_t chg_cnt = 0;
	static uint8_t cnt = 0;
	static uint8_t reset_cnt = 0;
	static uint8_t chg_oc_cnt = 0;
	p_arg = p_arg;
	
	while(1)
	{
		//�жϼ�ͣ���¸���ͣ��־λ
		if(STOP_KEY_READ == 1)
		{
			cnt ++;
			reset_cnt = 0;
			if(cnt > 2)
			{
				cnt = 0;
				Robot_Sys.Mergency_Stop_flag = true;
			}
		}
		else
		{
			cnt = 0;
			reset_cnt ++;
			if(reset_cnt > 5)
			{
				reset_cnt = 0;
				Robot_Sys.Mergency_Stop_flag = false;
			}
		}
		Robot_Sys.Crash_Flag = Get_Crash_Status();
    Ultra_OverTime_Process();
    Ultra_Process();
		Ultra_StartUp();    //�������еĳ���
			
		if(AutoCharge.chg_flag == 1 && (Pms.Bat_Sta & 0x01) == 0 && Robot_Sys.Last_Task == CHG_TASK)
		{
		  chg_cnt ++;
			if(chg_cnt > 8)
			{
				chg_cnt = 0;
				AutoCharge.StartUp_Flag = 1;
				AutoCharge.Chg_Sta = 0;
				charge_overtime = 0;
			  AutoCharge.Time_out = 0;
				AutoCharge.chg_flag = 0;
			}
		}
		else if(Pms.Moto_Cur > 10000 && (Pms.Bat_Sta & 0x01) == 0 && Robot_Sys.Last_Task == CHG_TASK)
		{
			chg_oc_cnt ++;
			if(chg_oc_cnt > 10)
			{
				chg_oc_cnt = 0;
				AutoCharge.StartUp_Flag = 1;
				AutoCharge.Chg_Sta = 0;
				charge_overtime = 0;
				AutoCharge.chg_flag = 0;
			}
		}
		else if((Ultra3.Distance < 300 || Ultra4.Distance < 300) && (Pms.Bat_Sta & 0x01) == 0 && Robot_Sys.Last_Task == CHG_TASK)
		{	
			chg_oc_cnt = 0;
			charge_overtime ++;
			if(charge_overtime > 100)
			{
				AutoCharge.StartUp_Flag = 1;
				AutoCharge.Chg_Sta = 0;
				charge_overtime = 0;
				AutoCharge.chg_flag = 0;
			}
		}
//		else if(AutoCharge.Time_out > 65000 && AutoCharge.NotFind_Flag != true && Robot_Sys.Last_Task == CHG_TASK){
//		 	AutoCharge.StartUp_Flag = 1;
//			AutoCharge.Chg_Sta = 0;
//			charge_overtime = 0;
//			 AutoCharge.Time_out = 0;
//			AutoCharge.chg_flag = 0;
//		}
		else
		{
		  charge_overtime = 0;
			chg_cnt = 0;
			chg_oc_cnt = 0;
		}
						
//		if(UsartToPC.Comm_TimeOut >= 500){    //0.5Sû���յ�������ֹͣ
//		  UsartToPC.Comm_TimeOut = 500;
//		  UsartToPC.Disconnect_flag = 1;
//		}
//		
//		if(UsartToDrv.Comm_TimeOut >= 1000){
//		  UsartToDrv.Comm_TimeOut = 1000;
//			UsartToDrv.Disconnect_flag = 1;
//		}
		
//		printf("----%d, %d, %d, %d\r\n\r\n", Ultra1.Distance, Ultra2.Distance, Ultra3.Distance, Ultra4.Distance);
		OSTimeDlyHMSM(0, 0, 0, 120, OS_OPT_TIME_HMSM_STRICT, &err); //��ʱ100ms
	}
}

/*=============================================================================
*  ������ ��LiftMoto_task
*  ��   �ߣ�hrx
*  ����ʱ�䣺2022��4��11�� 
*  �޸�ʱ�䣺
*  ��   �룺
*           
*  ��   ����
*  ˵   ����������������
*/
void LiftMoto_task(void *p_arg)
{
	OS_ERR err;
	p_arg = p_arg;
	while(1)
	{
		LiftMoto_Process();
		#ifdef LiftMoto_1
		if(Lift_Moto.Cmd == LIFT_STOP && Lift_Moto.Set_Height == 0xffff)
		{
			Robot_Sys.Lift_Task_flag = false;
			Lift_Moto.Cmd = LIFT_STOP;
			LiftMoto_Set(Lift_Moto.Cmd);
			OSTaskSuspend((OS_TCB*)&LIFTMOTO_TASKTCB, &err);			//�����������
		}
		OSTimeDlyHMSM(0, 0, 0, 10, OS_OPT_TIME_HMSM_STRICT, &err); //��ʱ10ms
		#elif LiftMoto_2
		OSTimeDlyHMSM(0, 0, 0, 100, OS_OPT_TIME_HMSM_STRICT, &err); //��ʱ100ms
		#endif
	}
}

/*=============================================================================
*  ������ ��Environ_Query_task
*  ��   �ߣ�hrx
*  ����ʱ�䣺2022��4��11�� 
*  �޸�ʱ�䣺
*  ��   �룺
*           
*  ��   ����
*  ˵   ����������������ѯ����
*/
void Environ_Query_task(void *p_arg)
{
	OS_ERR err;
	p_arg = p_arg;
	while(1)
	{
		UsartToVoice.Rx_Len = 0;
		UsartToVoice.Rx_Sta = 0;
		UsartToVoice.Usart_Rx_OK = false;
		Send_Voice_Query();
		OSTimeDlyHMSM(0, 0, 0, 500, OS_OPT_TIME_HMSM_STRICT, &err); //��ʱ20ms
	}
}

/*=============================================================================
*  ������ ��Environ_task
*  ��   �ߣ�hrx
*  ����ʱ�䣺2022��4��11�� 
*  �޸�ʱ�䣺
*  ��   �룺
*           
*  ��   ����
*  ˵   ����������������������������ȡ����
*/
void Environ_task(void *p_arg)
{
	//16 11 01 02 50 00 01 03 1A 03 2A 00 13 00 16 00 10 00 00 02
	//CO2: 600,VOC:1, Humi:782, Tempera: 816,PM2_5:18,PM10:20, PM1_0:15,state:0
	OS_ERR err;
	p_arg = p_arg;
	while(1)
	{
		//OSSemPend(&UsartEnviron_SEM,0,OS_OPT_PEND_BLOCKING,0,&err); //�����ź���
		Environmental_Process();
		Voice_Process();
		OSTimeDlyHMSM(0, 0, 0, 10, OS_OPT_TIME_HMSM_STRICT, &err); //��ʱ20ms
	}
}

/*=============================================================================
*  ������ ��Test_task
*  ��   �ߣ�hrx
*  ����ʱ�䣺2022��4��11�� 
*  �޸�ʱ�䣺
*  ��   �룺
*           
*  ��   ����
*  ˵   ������������
*/
void Test_task(void *p_arg)
{
	OS_ERR err;
	p_arg = p_arg;
	while(1)
	{
		OSSemPend(&UsartToTest_SEM, 0, OS_OPT_PEND_BLOCKING, 0, &err); //�����ź���
		Test_Cmd_Analysis();
	}
}

/*=============================================================================
*  ������ ��LED_task
*  ��   �ߣ�hrx
*  ����ʱ�䣺2022��4��11�� 
*  �޸�ʱ�䣺
*  ��   �룺
*           
*  ��   ����
*  ˵   ����LED����
*/
void LED_task(void *p_arg)
{
	static uint8_t code_cnt = 0;
  static uint8_t pc_power_cnt = 0;
	uint8_t i;
	OS_ERR err;  
	p_arg = p_arg;
	while(1)
	{
		//PCB�ϵ��̵���˸
		LED_TEST_TOGGLE;
		//�����
		if((Pms.Bat_Sta & 0x01) != 0)
		{
			if(Pms.Capacity >= 95)
			{
				LED_RED_OFF;
				LED_GREEN_ON;
			}
			else
			{
				LED_RED_ON;
				LED_GREEN_OFF;
			}
			pc_power_cnt = 0;
		}
		//���ػ�δ����
		else if((Pms.Bat_Sta & 0x01) == 0 && Pms.Bat_Current > -1000)
		{
			LED_RED_OFF;
			LED_GREEN_OFF;
			pc_power_cnt ++;
			if(pc_power_cnt > 20)
			{
				pc_power_cnt = 0;
				PC_Power_Cfg_Init(PC_STARTUP_ON);
				delay_ms(300);
				PC_Power_Cfg_Init(PC_STARTUP_OFF);
			}
		}
		//���ػ��Ͽ����Ӳ�����
		else if(UsartToPC.Disconnect_flag == 1)
		{
			LED_RED_OFF;
			LED_GREEN_OFF;
			pc_power_cnt = 0;
		}
		//������û������Ƴ���
		else if(UsartToDrv.Disconnect_flag == 1)
		{
			LED_RED_ON;
			LED_GREEN_OFF;
			pc_power_cnt = 0;
		}
		//�͵��������˸
		else if((Pms.Bat_Sta & 0x10) != 0)
		{
			LED_RED_TOGGLE;
			LED_GREEN_OFF;
			pc_power_cnt = 0;
		}
		//���������̵���˸
		else
		{
			LED_RED_OFF;
			LED_GREEN_TOGGLE;
			pc_power_cnt = 0;
		}

		#ifdef ROBOT_M100
		//��ʾ�ƿ���
		if(UsartToPC.Disconnect_flag == 1 || UsartToDrv.Disconnect_flag == 1)
		{
			LAMP_YELLOW_OFF;
			LAMP_RED_ON;
			LAMP_GREEN_OFF;
		}
		else if((Pms.Bat_Sta & 0x10) != 0)
		{
			LAMP_YELLOW_ON;
			LAMP_RED_OFF;
			LAMP_GREEN_OFF;
		}
		else
		{
			LAMP_YELLOW_OFF;
			LAMP_RED_OFF;
			LAMP_GREEN_ON;
		}
	
		if(Robot_Sys.Out_en_flag == 1) EN24_ENABLE;
		else EN24_DISABLE;
				 
		if(Robot_Sys.Beep_en_flag == 1) LAMP_BEEP_ON;
		else LAMP_BEEP_OFF;
		#elif ROBOT_MR9
		//���ƿ���
		if(Robot_Sys.CarLight_flag == 1) 
			LAMP_WHITE_ON;
		else if(Robot_Sys.CarLight_flag == 0) 
			LAMP_WHITE_OFF;
		#endif

//		printf("CH1_DAT1=%02X CH1_DAT2=%02X CH1_DAT3=%02X  CH1_DAT4=%02X \r\n", AutoCharge.CH1_IrDa_Dat[0],  
//		AutoCharge.CH1_IrDa_Dat[1],AutoCharge.CH1_IrDa_Dat[2], AutoCharge.CH1_IrDa_Dat[3] );
//		printf("CH3_DAT1=%02X CH3_DAT2=%02X CH3_DAT3=%02X CH3_DAT4=%02X \r\n", AutoCharge.CH3_IrDa_Dat[0],   
//		AutoCharge.CH3_IrDa_Dat[1], AutoCharge.CH3_IrDa_Dat[2], AutoCharge.CH3_IrDa_Dat[3] );
//		printf("Move_Sta = %d,%d\r\n", AutoCharge.Position, AutoCharge.Move_Sta);
//		printf("\r\n");
//		memset(AutoCharge.CH1_IrDa_Dat, 0x00, 4);  
//		memset(AutoCharge.CH3_IrDa_Dat, 0x00, 4);
		OSTimeDlyHMSM(0, 0, 0, 500, OS_OPT_TIME_HMSM_STRICT, &err); //��ʱ500ms

	}
}

//��ʱ��1�Ļص�����
void tmr1_callback(void *p_tmr, void *p_arg)
{
	static u8 tmr1_num = 0;
	tmr1_num++;		//��ʱ��1ִ�д�����1
	if(tmr1_num < 4)
	{
    //LED_TEST_TOGGLE;
		tmr1_num = 0;
	}
}

