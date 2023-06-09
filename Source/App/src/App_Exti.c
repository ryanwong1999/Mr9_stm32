/*=============================================================================
*  Copyright (C) ".strftime("%Y")." All rights reserved.
*
*  �ļ�����"App_Exti.c"
*  ���ߣ�hrx
*  ����ʱ�䣺2022��4��11�� 
*  �޸�ʱ�䣺
*  ˵�����ⲿ�жϴ���
*
==============================================================================*/

/* Includes ------------------------------------------------------------------*/
#include "includes.h"


/*��һ·����*/
uint8_t  CH1_IrDa_STA = 0;    
uint16_t CH1_IrDa_VAL1 = 0;		// ͨ��1 �����ؼ���ֵ
uint16_t CH1_IrDa_VAL2 = 0;		// ͨ��1 �½��ؼ���ֵ
uint8_t  CH1_IrDa_Data = 0;		// ͨ��1 ���յ�������

/*����·����*/
uint8_t  CH3_IrDa_STA = 0;    
uint16_t CH3_IrDa_VAL1 = 0;		// ͨ��3 �����ؼ���ֵ
uint16_t CH3_IrDa_VAL2 = 0;		// ͨ��3 �½��ؼ���ֵ
uint8_t  CH3_IrDa_Data = 0;		// ͨ��3 ���յ�������

uint16_t BaseTimeCount = 0;		// 10us ʱ���׼���� 

/*=============================================================================
*  ������ ��IR_Code_Delay10us
*  ��   �ߣ�hrx
*  ����ʱ�䣺2022��4��11�� 
*  �޸�ʱ�䣺
*  ��   �룺 
*           
*  ��   ����
*  ˵   �����ⲿ�ж�����Ҫ�ļ�����
*/
void IR_Code_Delay10us(void)
{
	BaseTimeCount ++;
	if(BaseTimeCount >= 0xffff)
		BaseTimeCount = 0 ;
}


/*=============================================================================
*  ������ ��EXTI0_IRQHandler
*  ��   �ߣ�hrx
*  ����ʱ�䣺2022��4��11�� 
*  �޸�ʱ�䣺
*  ��   �룺 
*           
*  ��   ����
*  ˵   �����ⲿ�ж�, �����Զ�������ı���
*/
void  EXTI0_IRQHandler(void)
{
	OS_ERR err;	
	uint16_t tmp ;
	#ifdef SYSTEM_SUPPORT_OS	 	
	OSIntEnter();    
  #endif

	//Line6�����ж�
	if(EXTI_GetITStatus(EXTI_LINE_IRBL) != RESET)   	
	{
		//û�в���������	0100 0000
		if((CH1_IrDa_STA & 0x40) != 0x40)            	
		{
			//û�в���������	0010 0000
			if((CH1_IrDa_STA & 0x20) != 0x20)          	
			{
				//�ߵ�ƽ
				if(GPIO_ReadInputDataBit(IRCODE_PORT, IRCODE_BL_PIN) == SET)
				{
					CH1_IrDa_VAL1 = BaseTimeCount;
					//��ǲ���������	0010 0000	
				  CH1_IrDa_STA |= 0x20;               						
				}
			}
			else
			{
				//�͵�ƽ �½�����
				if(GPIO_ReadInputDataBit(IRCODE_PORT,IRCODE_BL_PIN) == RESET)
				{  
					CH1_IrDa_VAL2 = BaseTimeCount;
					//������������ر�ǣ�������һ��	1101 1111
				  CH1_IrDa_STA &= 0xDF;               			
					//�����
					if(CH1_IrDa_VAL2 < CH1_IrDa_VAL1)
					{
						tmp = 65535 + CH1_IrDa_VAL2 - CH1_IrDa_VAL1;
					}
					else
					{
						tmp = CH1_IrDa_VAL2 - CH1_IrDa_VAL1;
					}
					//450*10us=4.5ms  4ms  4.5ms    ������
					if(tmp >= 370 && tmp < 550)
					{
						//��ǲ���������	0100 0000
						CH1_IrDa_STA |= 0x40;       			
						CH1_IrDa_Data = 0;
					}
				}
			}
		}
		//���յ�������
		else
		{
			//û�в���������	0010 0000
			if((CH1_IrDa_STA & 0x20) != 0x20)         		
			{
				//�ߵ�ƽ
				if(GPIO_ReadInputDataBit(IRCODE_PORT,IRCODE_BL_PIN) == SET)
				{
					CH1_IrDa_VAL1 = BaseTimeCount;
					//��ǲ���������
					CH1_IrDa_STA |= 0x20;
				}
			}
			else
			{
				//�͵�ƽ �½�����
				if(GPIO_ReadInputDataBit(IRCODE_PORT,IRCODE_BL_PIN) == RESET)
				{  
					CH1_IrDa_VAL2 = BaseTimeCount;
					//������������ر�ǣ�������һ��	1101 1111
					CH1_IrDa_STA &= 0xDF;               			
					//�����
					if(CH1_IrDa_VAL2 < CH1_IrDa_VAL1) tmp = 65535 + CH1_IrDa_VAL2 - CH1_IrDa_VAL1;
					else tmp = CH1_IrDa_VAL2 - CH1_IrDa_VAL1;
					//56*10us=560us 
					if(tmp >= 20 && tmp < 90)
					{               
						CH1_IrDa_Data |= 0;	
					}
					//169*10us=1690us
					else if(tmp >= 100 && tmp < 210)
					{       	
						CH1_IrDa_Data |= 1;
					}
					//225*10us=2250us ����  δ����
					else if(tmp >= 220 && tmp < 270)
					{
						
					}
					else if(tmp >= 300)
					{
						//���״̬�Ĵ��������½���	
						CH1_IrDa_STA = 0;
					}
					//0001 1111
					if((CH1_IrDa_STA & 0x1F) >= 6)
					{
						CH1_IrDa_STA = 0;
						if(CH1_IrDa_Data == TXDAT_LEFT)
						{
							AutoCharge.CH1_IrDa_Dat[0] = TXDAT_LEFT;	
						}	
						else if(CH1_IrDa_Data == TXDAT_MIDL)
						{
							AutoCharge.CH1_IrDa_Dat[1] = TXDAT_MIDL;	
						}	
						else if(CH1_IrDa_Data == TXDAT_RIGHT)
						{
							AutoCharge.CH1_IrDa_Dat[2] = TXDAT_RIGHT;
						}
						else if(CH1_IrDa_Data == TXDAT_MIDR)
						{
							AutoCharge.CH1_IrDa_Dat[3] = TXDAT_MIDR;	
						}
					}
					else
					{
						CH1_IrDa_STA ++ ;
						CH1_IrDa_Data <<= 1;
					}
				}
			}
		}
		//���Lin6�жϱ�־λ
		EXTI_ClearITPendingBit(EXTI_LINE_IRBL);
	}	
	#ifdef SYSTEM_SUPPORT_OS	 
	OSIntExit();  											 
  #endif
}

/*=============================================================================
*  ������ ��EXTI1_IRQHandler
*  ��   �ߣ�hrx
*  ����ʱ�䣺2022��4��11�� 
*  �޸�ʱ�䣺
*  ��   �룺 
*           
*  ��   ����
*  ˵   �����ⲿ�ж�, �����Զ�������ı���
*/
void EXTI1_IRQHandler(void)
{
	OS_ERR err;	
  uint16_t tmp;
	#ifdef SYSTEM_SUPPORT_OS	 	
	OSIntEnter();    
  #endif
	
	//Line6�����ж� EXTI_LINE_IRBR :EXTI_Line10
	if(EXTI_GetITStatus(EXTI_LINE_IRBR) != RESET)   	
	{
		//û�в���������
		if((CH3_IrDa_STA & 0x40) != 0x40)
		{
			//û�в���������    	
			if((CH3_IrDa_STA & 0x20) != 0x20)
			{          	
				//�ߵ�ƽ
				if(GPIO_ReadInputDataBit(IRCODE_PORT, IRCODE_BR_PIN) == SET)
				{  
					CH3_IrDa_VAL1 = BaseTimeCount;
					//��ǲ���������
				  CH3_IrDa_STA |= 0x20;
				}
			}
			else
			{
				//�͵�ƽ �½�����
				if(GPIO_ReadInputDataBit(IRCODE_PORT, IRCODE_BR_PIN) == RESET)
				{
					CH3_IrDa_VAL2 = BaseTimeCount;
					//������������ر�ǣ�������һ��
				  CH3_IrDa_STA &= 0xDF;
					//�����
					if(CH3_IrDa_VAL2 < CH3_IrDa_VAL1) tmp = 65535 + CH3_IrDa_VAL2 - CH3_IrDa_VAL1;
					else tmp = CH3_IrDa_VAL2 - CH3_IrDa_VAL1;
					
					if(tmp >= 370 && tmp < 550)
					{
						//��ǲ���������
						CH3_IrDa_STA |= 0x40 ;            			
						CH3_IrDa_Data = 0;
					}
				}
			}
		}
		//���յ�������
		else
		{
			//û�в���������
			if((CH3_IrDa_STA & 0x20) != 0x20)
			{
				//�ߵ�ƽ
				if(GPIO_ReadInputDataBit(IRCODE_PORT, IRCODE_BR_PIN) == SET)
				{
					CH3_IrDa_VAL1 = BaseTimeCount;
					//��ǲ���������
					CH3_IrDa_STA |= 0x20;
				}
			}
			else
			{
				//�͵�ƽ �½�����
				if(GPIO_ReadInputDataBit(IRCODE_PORT,IRCODE_BR_PIN) == RESET)
				{
					CH3_IrDa_VAL2 = BaseTimeCount;
					//������������ر�ǣ�������һ��
					CH3_IrDa_STA &= 0xDF;               			
					//�����
					if(CH3_IrDa_VAL2 < CH3_IrDa_VAL1) tmp = 65535 + CH3_IrDa_VAL2 - CH3_IrDa_VAL1;
					else tmp = CH3_IrDa_VAL2 - CH3_IrDa_VAL1;
					//56*10us=560us
					if(tmp >= 20 && tmp < 100)
					{               
						CH3_IrDa_Data |= 0;
					}
					//169*10us=1690us
					else if(tmp >= 100 && tmp < 220)
					{       	
						CH3_IrDa_Data |= 1;
					}
					//225*10us=2250us ����  δ����
					else if(tmp >= 230 && tmp < 270)
					{	
					}
					else if(tmp >= 300)
					{
						//���״̬�Ĵ��������½���	
						CH3_IrDa_STA = 0;
					}
					if((CH3_IrDa_STA & 0x1F) >= 6)
					{
						//����ͷ�������һ�ν���
						CH3_IrDa_STA = 0;
						if(CH3_IrDa_Data == TXDAT_LEFT)
						{
							AutoCharge.CH3_IrDa_Dat[0] = TXDAT_LEFT;	
						}
						else if(CH3_IrDa_Data == TXDAT_MIDL)
						{
							AutoCharge.CH3_IrDa_Dat[1] = TXDAT_MIDL;	
						}
						else if(CH3_IrDa_Data == TXDAT_RIGHT)
						{
							AutoCharge.CH3_IrDa_Dat[2] = TXDAT_RIGHT;	
						}
						else if(CH3_IrDa_Data == TXDAT_MIDR)
						{
							AutoCharge.CH3_IrDa_Dat[3] = TXDAT_MIDR;
						}
					}
					else
					{
						CH3_IrDa_STA ++ ;
						CH3_IrDa_Data <<= 1;
					}
				}
			}
		}
		//���Lin6�жϱ�־λ EXTI_LINE_IRBR :EXTI_Line10
		EXTI_ClearITPendingBit(EXTI_LINE_IRBR);
	}
	#ifdef SYSTEM_SUPPORT_OS	 
	OSIntExit();  											 
  #endif
}

/*=============================================================================
*  ������ ��EXTI2_IRQHandler
*  ��   �ߣ�hrx
*  ����ʱ�䣺2022��4��11�� 
*  �޸�ʱ�䣺
*  ��   �룺 
*           
*  ��   ����
*  ˵   �����ⲿ�ж�,
*/
void EXTI2_IRQHandler(void)
{
	OS_ERR err;	

	#ifdef SYSTEM_SUPPORT_OS	 	
	OSIntEnter();    
  #endif
	
	//Line12�����ж�
	if(EXTI_GetITStatus(EXTI_Line2) != RESET)   
	{
		//���Lin6�жϱ�־λ
		EXTI_ClearITPendingBit(EXTI_Line2);       
	}
	
	#ifdef SYSTEM_SUPPORT_OS	 
	OSIntExit();  											 
  #endif
}

/*=============================================================================
*  ������ ��EXTI4_IRQHandler
*  ��   �ߣ�hrx
*  ����ʱ�䣺2022��4��11�� 
*  �޸�ʱ�䣺
*  ��   �룺 
*           
*  ��   ����
*  ˵   �����ⲿ�ж�,
*/
void EXTI4_IRQHandler(void)
{
  OS_ERR err;	
	uint16_t tmp;
  static uint16_t ultra3_val1 = 0;
	static uint16_t ultra3_val2 = 0;
	#ifdef SYSTEM_SUPPORT_OS	 	
	OSIntEnter();    
  #endif
	
	//Line12�����ж�
	if(EXTI_GetITStatus(EXTI_Line4) != RESET)   
	{
		//���Lin6�жϱ�־λ
		EXTI_ClearITPendingBit(EXTI_Line4);
	}
	
	#ifdef SYSTEM_SUPPORT_OS	 
	OSIntExit();  											 
  #endif
}

/*=============================================================================
*  ������ ��EXTI9_5_IRQHandler
*  ��   �ߣ�hrx
*  ����ʱ�䣺2022��4��11�� 
*  �޸�ʱ�䣺
*  ��   �룺 
*           
*  ��   ����
*  ˵   �����ⲿ�ж�,��ȡ����Ultra1��Ultra2��Ultra3�ľ���
*/
void EXTI9_5_IRQHandler(void)
{
	OS_ERR err;	
	uint16_t tmp;
	
	static uint16_t ultra1_val1 = 0;
	static uint16_t ultra1_val2 = 0;
	static uint16_t ultra2_val1 = 0;
	static uint16_t ultra2_val2 = 0;
	static uint16_t ultra3_val1 = 0;
	static uint16_t ultra3_val2 = 0;
	
	#ifdef SYSTEM_SUPPORT_OS	 	
	OSIntEnter();    
  #endif
	
	//Line12�����ж�
	if(EXTI_GetITStatus(EXTI_Line5) != RESET)   
	{
		//�Ѿ�ʹ�ܳ��������ݲ���
		if((Ultra2.Uswave_Sta & 0x20) == 0x20)
		{
			//����û�н������
			if((Ultra2.Uswave_Sta & 0x80) != 0x80)
			{
				//û�в���������
				if((Ultra2.Uswave_Sta & 0x40) != 0x40)
				{
					//�ߵ�ƽ
					if(ECHO_READ_2 == SET)
					{
						ultra2_val1 =BaseTimeCount;
						//��ǲ���������
						Ultra2.Uswave_Sta |= 0x40;
					}
				}
				//�����������ˣ��жϲ����½���
				else
				{
					//�͵�ƽ �½�����
					if(ECHO_READ_2 == RESET)
					{
						ultra2_val2 = BaseTimeCount;
						//���������
						if(ultra2_val2 < ultra2_val1)
						{
							tmp = 65535 + ultra2_val2 - ultra2_val1;
						}
						else
						{
							tmp = ultra2_val2 - ultra2_val1;
						}
						//�ϰ������  mm
						Ultra2.Distance = tmp*50/29;
						if(Ultra2.Distance > 2000) Ultra2.Distance = 2000;
						//���ʹ�ܽ�������
						Ultra2.Uswave_Sta &= 0x0F;
						//���ݽ������
						Ultra2.Uswave_Sta |=0x80;
					  Ultra2.Out_time = 0;
					}
				}
			}
		}
		//���Lin6�жϱ�־λ
		EXTI_ClearITPendingBit(EXTI_Line5);
	}
	
	//Line12�����ж�
	if(EXTI_GetITStatus(EXTI_Line7) != RESET)
	{
		//�Ѿ�ʹ�ܳ��������ݲ���
		if((Ultra1.Uswave_Sta & 0x20) == 0x20)
		{
			//����û�н������
			if((Ultra1.Uswave_Sta & 0x80) != 0x80)
			{
				//û�в���������
				if((Ultra1.Uswave_Sta & 0x40) != 0x40)
				{
					//�ߵ�ƽ
					if(ECHO_READ_1 == SET)
					{
						ultra1_val1 =BaseTimeCount;
						//��ǲ���������
						Ultra1.Uswave_Sta |= 0x40;	
					}
				}
				//�����������ˣ��жϲ����½���
				else
				{
					//�͵�ƽ �½�����
					if(ECHO_READ_1 == RESET)
					{
						ultra1_val2 = BaseTimeCount;
						//���������
						if(ultra1_val2 < ultra1_val1)
						{
							tmp= 65535 + ultra1_val2 - ultra1_val1;
						}
						else
						{
							tmp= ultra1_val2 - ultra1_val1;
						}
						//�ϰ������  mm
						Ultra1.Distance = tmp*50/29;
						if(Ultra1.Distance>2000) Ultra1.Distance = 2000;
						//���ʹ�ܽ�������
						Ultra1.Uswave_Sta &= 0x0F;
						//���ݽ������
						Ultra1.Uswave_Sta |=0x80;
					  Ultra1.Out_time = 0;
					}
				}
			}
		}
		//���Lin6�жϱ�־λ
		EXTI_ClearITPendingBit(EXTI_Line7);
	}
	
	//Line12�����ж�
	if(EXTI_GetITStatus(EXTI_Line9) != RESET)
	{
	 //�Ѿ�ʹ�ܳ��������ݲ���
   if((Ultra3.Uswave_Sta & 0x20) == 0x20)
		{
			//����û�н������
			if((Ultra3.Uswave_Sta & 0x80) != 0x80)
			{
				//û�в���������
				if((Ultra3.Uswave_Sta & 0x40) != 0x40)
				{
					//�ߵ�ƽ
					if(ECHO_READ_3 == SET)
					{
						ultra3_val1 = BaseTimeCount;
						//��ǲ���������	
						Ultra3.Uswave_Sta |= 0x40;
					}
				}
				//�����������ˣ��жϲ����½���
				else
				{
					//�͵�ƽ �½�����
					if(ECHO_READ_3 == RESET)
					{
						ultra3_val2 = BaseTimeCount;
						//���������
						if(ultra3_val2 < ultra3_val1) tmp = 65535 + ultra3_val2 - ultra3_val1;
						else tmp = ultra3_val2 - ultra3_val1;
						//�ϰ������  mm
						Ultra3.Distance = tmp*50/29;
						//printf("Ultra3.Distance = %d\r\n",Ultra3.Distance);
						if(Ultra3.Distance > 2000) Ultra3.Distance = 2000;
						//���ʹ�ܽ�������
						Ultra3.Uswave_Sta &= 0x0F;
						//���ݽ������
						Ultra3.Uswave_Sta |=0x80;
					  Ultra3.Out_time = 0;
					}
				}
			}
		}
		//���Lin6�жϱ�־λ
		EXTI_ClearITPendingBit(EXTI_Line9);
	}
	#ifdef SYSTEM_SUPPORT_OS	 
	OSIntExit();  											 
  #endif
}


/*=============================================================================
*  ������ ��EXTI15_10_IRQHandler
*  ��   �ߣ�hrx
*  ����ʱ�䣺2022��4��11�� 
*  �޸�ʱ�䣺
*  ��   �룺 
*           
*  ��   ����
*  ˵   �����ⲿ�ж�,��ȡ����Ultra4�ľ���
*/
void EXTI15_10_IRQHandler(void)
{
	OS_ERR err;	
	uint16_t tmp;
	
	static uint16_t ultra4_val1 = 0;
	static uint16_t ultra4_val2 = 0;
	static uint16_t ultra5_val1 = 0;
	static uint16_t ultra5_val2 = 0;
	static uint16_t ultra6_val1 = 0;
	static uint16_t ultra6_val2 = 0;
	
	#ifdef SYSTEM_SUPPORT_OS	 	
	OSIntEnter();    
  #endif
	//Line12�����ж�
  if(EXTI_GetITStatus(EXTI_Line11) != RESET)
	{
		//�Ѿ�ʹ�ܳ��������ݲ���
		if((Ultra4.Uswave_Sta &0x20) == 0x20)
		{
			//����û�н������
			if((Ultra4.Uswave_Sta &0x80) != 0x80)
			{
				//û�в���������
				if((Ultra4.Uswave_Sta &0x40) != 0x40)
				{
					//�ߵ�ƽ
					if(ECHO_READ_4 == SET)
					{
						ultra4_val1 = BaseTimeCount;
						//��ǲ���������	
						Ultra4.Uswave_Sta |= 0x40;
					}
				}
				//�����������ˣ��жϲ����½���
				else
				{
					//�͵�ƽ �½�����
					if(ECHO_READ_4 == RESET)
					{
						ultra4_val2 = BaseTimeCount;
						//���������
						if(ultra4_val2 < ultra4_val1) tmp = 65535 + ultra4_val2 - ultra4_val1;
						else tmp = ultra4_val2 - ultra4_val1;
						
						//�ϰ������  mm
						Ultra4.Distance = tmp*50/29;
						if(Ultra4.Distance>2000) Ultra4.Distance = 2000;
						//���ʹ�ܽ�������
						Ultra4.Uswave_Sta &= 0x0F;
						//���ݽ������
						Ultra4.Uswave_Sta |=0x80;
					  Ultra4.Out_time = 0;
					}
				}
			}
		}
		//���Lin6�жϱ�־λ
		EXTI_ClearITPendingBit(EXTI_Line11);
	}

	#ifdef SYSTEM_SUPPORT_OS	 
	OSIntExit();  											 
  #endif
}

/* CopyRight : Copyright(c) 1998 - 2015 YZRobot. All Rights Reserved */





