/*=============================================================================
*  Copyright (C) ".strftime("%Y")." All rights reserved.
*
*  文件名："App_24cxx.c"
*  作者：hrx
*  创建时间：2022年4月11日 
*  修改时间：
*  说明：AT24Cxx的读取和写入
*
==============================================================================*/


#include "App_24cxx.h" 
#include "includes.h" 				 


#define AT24CXX_FIRST_STA  0x51
#define FIRST_STA_ADDR     0xff

//初始化IIC接口

void AT24CXX_Init(void)
{
	IIC_Init();//IIC初始化
}


//在AT24CXX指定地址读出一个数据
//ReadAddr:开始读数的地址  
//返回值  :读到的数据
u8 AT24CXX_ReadOneByte(u16 ReadAddr)
{				  
	u8 temp=0;		  	    																 
  IIC_Start();  
	if(EE_TYPE>AT24C16)
	{
		IIC_Send_Byte(0XA0);	   			//发送写命令
		IIC_Wait_Ack();
		IIC_Send_Byte(ReadAddr>>8);		//发送高地址	    
	}
	else IIC_Send_Byte(0XA0+((ReadAddr/256)<<1));   //发送器件地址0XA0,写数据 
	IIC_Wait_Ack(); 
  IIC_Send_Byte(ReadAddr%256);   	//发送低地址
	IIC_Wait_Ack();	    
	IIC_Start();  	 	   
	IIC_Send_Byte(0XA1);           	//进入接收模式			   
	IIC_Wait_Ack();	 
  temp=IIC_Read_Byte(0);		   
  IIC_Stop();											//产生一个停止条件	    
	return temp;
}

//在AT24CXX指定地址写入一个数据
//WriteAddr  :写入数据的目的地址    
//DataToWrite:要写入的数据
void AT24CXX_WriteOneByte(u16 WriteAddr,u8 DataToWrite)
{				   	  	    																 
  IIC_Start();  
	if(EE_TYPE>AT24C16)
	{
		IIC_Send_Byte(0XA0);	    		//发送写命令
		IIC_Wait_Ack();
		IIC_Send_Byte(WriteAddr>>8);	//发送高地址	  
	}
	else IIC_Send_Byte(0XA0+((WriteAddr/256)<<1));   //发送器件地址0XA0,写数据 	 
	IIC_Wait_Ack();	   
  IIC_Send_Byte(WriteAddr%256);   //发送低地址
	IIC_Wait_Ack(); 	 										  		   
	IIC_Send_Byte(DataToWrite);     //发送字节							   
	IIC_Wait_Ack();  		    	   
  IIC_Stop();											//产生一个停止条件 
	delay_ms(50);	 
}

//在AT24CXX里面的指定地址开始写入长度为Len的数据
//该函数用于写入16bit或者32bit的数据.
//WriteAddr  :开始写入的地址  
//DataToWrite:数据数组首地址
//Len        :要写入数据的长度2,4
void AT24CXX_WriteLenByte(u16 WriteAddr,u32 DataToWrite,u8 Len)
{  	
	u8 t;
	for(t=0;t<Len;t++)
	{
		AT24CXX_WriteOneByte(WriteAddr+t,(DataToWrite>>(8*t))&0xff);
	}												    
}

//在AT24CXX里面的指定地址开始读出长度为Len的数据
//该函数用于读出16bit或者32bit的数据.
//ReadAddr   :开始读出的地址 
//返回值     :数据
//Len        :要读出数据的长度2,4
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


//在AT24CXX里面的指定地址开始读出指定个数的数据
//ReadAddr :开始读出的地址 对24c02为0~255
//pBuffer  :数据数组首地址
//NumToRead:要读出数据的个数
void AT24CXX_Read(u16 ReadAddr,u8 *pBuffer,u16 NumToRead)
{
	while(NumToRead)
	{
		*pBuffer++=AT24CXX_ReadOneByte(ReadAddr++);	
		NumToRead--;
	}
}  


//在AT24CXX里面的指定地址开始写入指定个数的数据
//WriteAddr :开始写入的地址 对24c02为0~255
//pBuffer   :数据数组首地址
//NumToWrite:要写入数据的个数
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
*  函数名 ：AT24xx_Check
*  作   者：hrx
*  创建时间：2022年4月11日 
*  修改时间：
*  输   入：
*           
*  输   出：返回是否检测到芯片标志
*  说   明：检测AT24Cxx是否存在，并在首次检测时写入默认值
*/
uint8_t AT24xx_Check(void)
{
	u8 temp;
	temp = AT24CXX_ReadOneByte(FIRST_STA_ADDR);//避免每次开机都写AT24CXX			  
	if(temp == AT24CXX_FIRST_STA)
	{
		printf("Read AT24xx OK!\r\n");
		Robot_Sys.mBeepStatus.BeepMode = 1;
		return 0;	
	}
	else
	{		//排除第一次初始化的情况
		Pms.Capacity_Set = FULL_CAP;
		Capacity_Init();
		
		Pms.Capacity_mah =  Pms.Capacity * (Pms.Capacity_Set/100); 
		
		Robot_Sys.Ultra_Disable_Flag = 0;
		Robot_Sys.IR_Bottom_Disable_Flag = 0;
	  Robot_Sys.IR_Front_Disable_Flag = 0;
		
		printf("Firse Write AT24xx !\r\n");
		
		AT24CXX_WriteOneByte(2,(Pms.Capacity_Set>>8) & 0x00ff);	//设置容量
		AT24CXX_WriteOneByte(3,Pms.Capacity_Set& 0x00ff);
		
		AT24CXX_WriteOneByte(4,(Pms.Capacity_mah>>8) & 0x00ff);	//当前容量
		AT24CXX_WriteOneByte(5,Pms.Capacity_mah& 0x00ff);
			
		//头部水平俯仰设置偏移量
		Head_Status.Level_Offset = 70;
		Head_Status.Pitch_Offset = 60;
		AT24CXX_WriteOneByte(6,Head_Status.Level_Offset);   		//当前容量
		AT24CXX_WriteOneByte(7,Head_Status.Pitch_Offset);
	
		AT24CXX_WriteOneByte(8,Robot_Sys.Ultra_Disable_Flag);   //当前容量
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
*  函数名 ：Read_PowerOn_AT24xx
*  作   者：hrx
*  创建时间：2022年4月11日 
*  修改时间：
*  输   入：
*           
*  输   出：
*  说   明：开机读取存储数据
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







