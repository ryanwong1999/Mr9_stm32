#ifndef __APP_TASK_H__
#define __APP_TASK_H__

#include "delay.h"

#include "includes.h"					//ucos 使用	  


#define CAMMAND_TASK_OS  					1
//#define MOVE_SPEED_TASK_OS        1
#define ERR_TASK_OS 							1
#define POWEROFF_TASK_OS 					1
#define CHG_TASK_OS 							1
#define TEST_TASK_OS 							1
//#define PICC_TASK_OS  						1
#define PMU_TASK_OS 							1
//#define SEND_DMRV_TASK_OS   			1
#define DMRV_TASK_OS             	1
#define HEAD_CTRL_TASK_OS 				1
#define AUTOCHARGE_TASK_OS 				1
#define ULTRA_TASK_OS  						1
#define CRASH_TASK_OS 						1
//#define AGV_TASK_OS 							1
//#define AGV_GuideSta_TASK_OS 			1
//#define AGV_Guide_TASK_OS  				1
#define LIFTMOTO_TASK_OS 					1
#define ENVIRON_TASK_OS 					1
#define ENVIRON_QUERY_TASK_OS 		1 
//#define ESP8266_TASK_OS         	1
#define LED_TASK_OS 							1 


//任务优先级
#define START_TASK_PRIO		3
//任务堆栈大小	
#define START_STK_SIZE 		128

#define CHG_TASK    1
#define AGV_TASK    2
#define REMOTE_TASK 3


/*
extern OS_TCB StartTaskTCB; 
extern OS_TCB  ERR_TASKTCB;		
extern OS_TCB  POWEROFF_TASKTCB;				
extern OS_TCB  CHG_TASKTCB;
extern OS_TCB  CAMMAND_TASKTCB;
extern OS_TCB  COMM_TASKTCB;
extern OS_TCB  PMU_TASKTCB;  
extern OS_TCB  HEAD_CTRL_TASKTCB;    
extern OS_TCB  LED_TASKTCB;   
*/

extern OS_TCB StartTaskTCB;
extern CPU_STK START_TASK_STK[START_STK_SIZE];


extern OS_SEM	UsartRxFromPC_SEM;		  //定义一个信号量，用于USART1的任务同步
extern OS_SEM UsartEnviron_SEM;
extern OS_SEM  UsartToTest_SEM;
extern OS_SEM UsartToDrv_SEM;
//任务函数
void start_task(void *p_arg);

#endif

