/*=============================================================================
*  Copyright (C) ".strftime("%Y")." All rights reserved.
*
*  �ļ�����"App_24cxx.c"
*  ���ߣ�hrx
*  ����ʱ�䣺2022��4��11�� 
*  �޸�ʱ�䣺
*  ˵����AT24Cxx�Ķ�ȡ��д��
*
==============================================================================*/


#include "App_24cxx.h" 
#include "includes.h" 				 


#define AT24CXX_FIRST_STA  0x51
#define FIRST_STA_ADDR     0xff

//��ʼ��IIC�ӿ�

void AT24CXX_Init(void)
{
	IIC_Init();//IIC��ʼ��
}


//��AT24CXXָ����ַ����һ������
//ReadAddr:��ʼ�����ĵ�ַ  
//����ֵ  :����������
u8 AT24CXX_ReadOneByte(u16 ReadAddr)
{				  
	u8 temp=0;		  	    																 
  IIC_Start();  
	if(EE_TYPE>AT24C16)
	{
		IIC_Send_Byte(0XA0);	   			//����д����
		IIC_Wait_Ack();
		IIC_Send_Byte(ReadAddr>>8);		//���͸ߵ�ַ	    
	}
	else IIC_Send_Byte(0XA0+((ReadAddr/256)<<1));   //����������ַ0XA0,д���� 
	IIC_Wait_Ack(); 
  IIC_Send_Byte(ReadAddr%256);   	//���͵͵�ַ
	IIC_Wait_Ack();	    
	IIC_Start();  	 	   
	IIC_Send_Byte(0XA1);           	//�������ģʽ			   
	IIC_Wait_Ack();	 
  temp=IIC_Read_Byte(0);		   
  IIC_Stop();											//����һ��ֹͣ����	    
	return temp;
}

//��AT24CXXָ����ַд��һ������
//WriteAddr  :д�����ݵ�Ŀ�ĵ�ַ    
//DataToWrite:Ҫд�������
void AT24CXX_WriteOneByte(u16 WriteAddr,u8 DataToWrite)
{				   	  	    																 
  IIC_Start();  
	if(EE_TYPE>AT24C16)
	{
		IIC_Send_Byte(0XA0);	    		//����д����
		IIC_Wait_Ack();
		IIC_Send_Byte(WriteAddr>>8);	//���͸ߵ�ַ	  
	}
	else IIC_Send_Byte(0XA0+((WriteAddr/256)<<1));   //����������ַ0XA0,д���� 	 
	IIC_Wait_Ack();	   
  IIC_Send_Byte(WriteAddr%256);   //���͵͵�ַ
	IIC_Wait_Ack(); 	 										  		   
	IIC_Send_Byte(DataToWrite);     //�����ֽ�							   
	IIC_Wait_Ack();  		    	   
  IIC_Stop();											//����һ��ֹͣ���� 
	delay_ms(50);	 
}

//��AT24CXX�����ָ����ַ��ʼд�볤��ΪLen������
//�ú�������д��16bit����32bit������.
//WriteAddr  :��ʼд��ĵ�ַ  
//DataToWrite:���������׵�ַ
//Len        :Ҫд�����ݵĳ���2,4
void AT24CXX_WriteLenByte(u16 WriteAddr,u32 DataToWrite,u8 Len)
{  	
	u8 t;
	for(t=0;t<Len;t++)
	{
		AT24CXX_WriteOneByte(WriteAddr+t,(DataToWrite>>(8*t))&0xff);
	}												    
}

//��AT24CXX�����ָ����ַ��ʼ��������ΪLen������
//�ú������ڶ���16bit����32bit������.
//ReadAddr   :��ʼ�����ĵ�ַ 
//����ֵ     :����
//Len        :Ҫ�������ݵĳ���2,4
u32 AT24CXX_ReadLenByte(u16 ReadAddr,u8 Len)
{  	
	u8 t;
	u32 temp=0;
	for(t=0;t<Len;t++)
	{
		temp<<=8;
		temp+=AT24CXX_ReadOneByte(ReadAddr+Len-t-1); 	 				   
	}
	return temp;												    
}


//��AT24CXX�����ָ����ַ��ʼ����ָ������������
//ReadAddr :��ʼ�����ĵ�ַ ��24c02Ϊ0~255
//pBuffer  :���������׵�ַ
//NumToRead:Ҫ�������ݵĸ���
void AT24CXX_Read(u16 ReadAddr,u8 *pBuffer,u16 NumToRead)
{
	while(NumToRead)
	{
		*pBuffer++=AT24CXX_ReadOneByte(ReadAddr++);	
		NumToRead--;
	}
}  


//��AT24CXX�����ָ����ַ��ʼд��ָ������������
//WriteAddr :��ʼд��ĵ�ַ ��24c02Ϊ0~255
//pBuffer   :���������׵�ַ
//NumToWrite:Ҫд�����ݵĸ���
void AT24CXX_Write(u16 WriteAddr,u8 *pBuffer,u16 NumToWrite)
{
	while(NumToWrite--)
	{
		AT24CXX_WriteOneByte(WriteAddr,*pBuffer);
		WriteAddr++;
		pBuffer++;
	}
}

/*=============================================================================
*  ������ ��AT24xx_Check
*  ��   �ߣ�hrx
*  ����ʱ�䣺2022��4��11�� 
*  �޸�ʱ�䣺
*  ��   �룺
*           
*  ��   ���������Ƿ��⵽оƬ��־
*  ˵   �������AT24Cxx�Ƿ���ڣ������״μ��ʱд��Ĭ��ֵ
*/
uint8_t AT24xx_Check(void)
{
	u8 temp;
	temp = AT24CXX_ReadOneByte(FIRST_STA_ADDR);//����ÿ�ο�����дAT24CXX			  
	if(temp == AT24CXX_FIRST_STA)
	{
		printf("Read AT24xx OK!\r\n");
		Robot_Sys.mBeepStatus.BeepMode = 1;
		return 0;	
	}
	else
	{		//�ų���һ�γ�ʼ�������
		Pms.Capacity_Set = FULL_CAP;
		Capacity_Init();
		
		Pms.Capacity_mah =  Pms.Capacity * (Pms.Capacity_Set/100); 
		
		Robot_Sys.Ultra_Disable_Flag = 0;
		Robot_Sys.IR_Bottom_Disable_Flag = 0;
	  Robot_Sys.IR_Front_Disable_Flag = 0;
		
		printf("Firse Write AT24xx !\r\n");
		
		AT24CXX_WriteOneByte(2,(Pms.Capacity_Set>>8) & 0x00ff);	//��������
		AT24CXX_WriteOneByte(3,Pms.Capacity_Set& 0x00ff);
		
		AT24CXX_WriteOneByte(4,(Pms.Capacity_mah>>8) & 0x00ff);	//��ǰ����
		AT24CXX_WriteOneByte(5,Pms.Capacity_mah& 0x00ff);
			
		//ͷ��ˮƽ��������ƫ����
		Head_Status.Level_Offset = 70;
		Head_Status.Pitch_Offset = 60;
		AT24CXX_WriteOneByte(6,Head_Status.Level_Offset);   		//��ǰ����
		AT24CXX_WriteOneByte(7,Head_Status.Pitch_Offset);
	
		AT24CXX_WriteOneByte(8,Robot_Sys.Ultra_Disable_Flag);   //��ǰ����
		AT24CXX_WriteOneByte(9,Robot_Sys.IR_Bottom_Disable_Flag);
		AT24CXX_WriteOneByte(10,Robot_Sys.IR_Front_Disable_Flag);
		
		AT24CXX_WriteOneByte(FIRST_STA_ADDR,AT24CXX_FIRST_STA);
	  temp=AT24CXX_ReadOneByte(FIRST_STA_ADDR);	 
	
    delay_ms(2);
    printf("write first sta : %02x\r\n",temp);		
		if(temp == AT24CXX_FIRST_STA)
		{
			Robot_Sys.mBeepStatus.BeepMode = 2;
			printf("Firse Read AT24xx OK!\r\n");
			return 0;
		}
	}
	return 1;											  
}


/*=============================================================================
*  ������ ��Read_PowerOn_AT24xx
*  ��   �ߣ�hrx
*  ����ʱ�䣺2022��4��11�� 
*  �޸�ʱ�䣺
*  ��   �룺
*           
*  ��   ����
*  ˵   ����������ȡ�洢����
*/
void Read_PowerOn_AT24xx(void)
{
	uint16_t temp = 0, temp_1 = 0;

	temp = AT24CXX_ReadOneByte(2);
	Pms.Capacity_Set = (temp<<8) + AT24CXX_ReadOneByte(3);
	
	temp_1 = AT24CXX_ReadOneByte(4);
	Pms.Capacity_mah = (temp_1<<8) + AT24CXX_ReadOneByte(5);
	
	Head_Status.Level_Offset = AT24CXX_ReadOneByte(6);
	Head_Status.Pitch_Offset = AT24CXX_ReadOneByte(7);
	
	Robot_Sys.Ultra_Disable_Flag = AT24CXX_ReadOneByte(8);
	Robot_Sys.IR_Bottom_Disable_Flag = AT24CXX_ReadOneByte(9);
	Robot_Sys.IR_Front_Disable_Flag = AT24CXX_ReadOneByte(10);
}







