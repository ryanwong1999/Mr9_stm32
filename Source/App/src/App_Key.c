/*=============================================================================
*  Copyright (C) ".strftime("%Y")." All rights reserved.
*
*  文件名："App_Key.c"
*  作者：zz  (zhangzhen123good@163.com)
*  创建时间：2016年07月25日 
*  修改时间：
*  说明：
*
==============================================================================*/
/* Includes ------------------------------------------------------------------*/

//#include "includes.h"
#include "includes.h"

extern uint8_t    Key_Cnt_Flag ;
extern uint16_t   Key_Cnt_Plus ;

void  Read_StopKeyStatus_Task(void)
{
	if(READ_STOP_KEY == 0)       // 按下
	{
		Key_Cnt_Flag = 1;
		if(Key_Cnt_Plus >=50)     // 100*1ms = 100ms 
		{
			if(READ_STOP_KEY == 0)   // 再次读取急停按键
			{
				// 按键被按下
				Mr7_Sys.mKey.bStop_Key = true;
				
			}else
			{
				Mr7_Sys.mKey.bStop_Key = false;
				Key_Cnt_Flag = 0;
				Key_Cnt_Plus = 0;
			}
		}
	}else
	{
		Key_Cnt_Flag = 0;
		Key_Cnt_Plus = 0;
		Mr7_Sys.mKey.bStop_Key = false;
	}

}




