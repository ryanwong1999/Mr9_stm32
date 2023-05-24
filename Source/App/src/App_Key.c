/*=============================================================================
*  Copyright (C) ".strftime("%Y")." All rights reserved.
*
*  �ļ�����"App_Key.c"
*  ���ߣ�zz  (zhangzhen123good@163.com)
*  ����ʱ�䣺2016��07��25�� 
*  �޸�ʱ�䣺
*  ˵����
*
==============================================================================*/
/* Includes ------------------------------------------------------------------*/

//#include "includes.h"
#include "includes.h"

extern uint8_t    Key_Cnt_Flag ;
extern uint16_t   Key_Cnt_Plus ;

void  Read_StopKeyStatus_Task(void)
{
	if(READ_STOP_KEY == 0)       // ����
	{
		Key_Cnt_Flag = 1;
		if(Key_Cnt_Plus >=50)     // 100*1ms = 100ms 
		{
			if(READ_STOP_KEY == 0)   // �ٴζ�ȡ��ͣ����
			{
				// ����������
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




