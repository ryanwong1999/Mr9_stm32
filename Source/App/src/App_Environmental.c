/*=============================================================================
*  Copyright (C) ".strftime("%Y")." All rights reserved.
*
*  �ļ�����"App_Environmental.c"
*  ���ߣ�hrx
*  ����ʱ�䣺2022��4��11�� 
*  �޸�ʱ�䣺
*  ˵������������������������������
*
==============================================================================*/

/* Includes ------------------------------------------------------------------*/


#include "includes.h"


Environmental_Typedef Environ;


/*=============================================================================
*  ������ ��Send_Environ_Query
*  ��   �ߣ�hrx
*  ����ʱ�䣺2022��4��11�� 
*  �޸�ʱ�䣺
*  ��   �룺 
*           
*  ��   ����
*  ˵   �������ͻ�����������ѯ����
*/
void Send_Environ_Query(void)
{

	uint8_t *buf;
	uint8_t sramx=0;					//Ĭ��Ϊ�ڲ�sram
  buf = mymalloc(sramx,10);//����20�ֽ�

	buf[0] = 0x11;
	buf[1] = 0x02;
	buf[2] = 0x01;
	buf[3] = 0x00;
	buf[4] = CRC_Sum(buf,4);
	
	USARTx_SendMultibyte(USART_ENVIRON, buf, 5);
	myfree(sramx,buf);
}


/*=============================================================================
*  ������ ��Send_Voice_Query
*  ��   �ߣ�hrx
*  ����ʱ�䣺2022��4��11�� 
*  �޸�ʱ�䣺
*  ��   �룺 
*           
*  ��   ����
*  ˵   ��������������������ѯ����
*/
void Send_Voice_Query(void)
{

	uint8_t *buf;
	uint8_t sramx=0;					//Ĭ��Ϊ�ڲ�sram
  buf = mymalloc(sramx,10);//����20�ֽ�

	buf[0] = 0x01;
	buf[1] = 0x03;
	buf[2] = 0x00;
	buf[3] = 0x00;
	buf[4] = 0x00;
	buf[5] = 0x01;
	buf[6] = 0x84;
	buf[7] = 0x0a;
	USARTx_SendMultibyte(USART_VOICE, buf, 8);
	myfree(sramx,buf);
}


/*=============================================================================
*  ������ ��Environmental_Process
*  ��   �ߣ�hrx
*  ����ʱ�䣺2022��4��11�� 
*  �޸�ʱ�䣺
*  ��   �룺 
*           
*  ��   ����
*  ˵   ������ȡ��������������
*/
void Environmental_Process(void)
{
	uint16_t i;
	float hum_tmp;
	float tem_tmp;
	uint8_t len;
	uint16_t rx_crc;
	uint16_t cal_crc;
	if(UsartToEnviro.Usart_Rx_OK == true){
		len = UsartToEnviro.Rx_Len;
		rx_crc = (uint16_t)UsartToEnviro.Rx_Buf[ len - 1] << 8 | UsartToEnviro.Rx_Buf[ len - 2];
		cal_crc =  CRC_Compute (UsartToEnviro.Rx_Buf, len - 2);

		if(rx_crc == cal_crc){
			memcpy(&Environ.CO2,  &UsartToEnviro.Rx_Buf[ 3 ],sizeof(int16_t));
			Environ.CO2 = (short int)t_ntohs(Environ.CO2);

			memcpy(&Environ.VOC, &UsartToEnviro.Rx_Buf[ 5 ],sizeof(int16_t));
			Environ.VOC = (short int) t_ntohs(Environ.VOC);
			
			memcpy(&Environ.Humi, &UsartToEnviro.Rx_Buf[ 7 ],sizeof(int16_t));
			Environ.CH2O = (short int) t_ntohs(Environ.Humi);	
			
			memcpy(&Environ.PM2_5, &UsartToEnviro.Rx_Buf[ 9 ],sizeof(int16_t));
			Environ.PM2_5 = (short int) t_ntohs(Environ.PM2_5);

			memcpy(&Environ.Humi, &UsartToEnviro.Rx_Buf[ 11 ],sizeof(int16_t));
			hum_tmp = (float) t_ntohs(Environ.Humi);
			Environ.Humi = (uint16_t)hum_tmp;

			memcpy(&Environ.Tempera, &UsartToEnviro.Rx_Buf[ 13 ],sizeof(int16_t));

			tem_tmp = (float) t_ntohs(Environ.Tempera);
			Environ.Tempera = (int16_t)tem_tmp;

			memcpy(&Environ.PM10, &UsartToEnviro.Rx_Buf[ 15 ],sizeof(int16_t));
			Environ.PM10 = (short int) t_ntohs(Environ.PM10);
		}
		
	UsartToEnviro.Rx_Len = 0;
	UsartToEnviro.Rx_Sta = 0;
  UsartToEnviro.Comm_TimeOut = 0;
	UsartToEnviro.Usart_Rx_OK = false;

	}
}

/*=============================================================================
*  ������ ��Voice_Process
*  ��   �ߣ�hrx
*  ����ʱ�䣺2022��4��11�� 
*  �޸�ʱ�䣺
*  ��   �룺 
*           
*  ��   ����
*  ˵   ������ȡ��������������
*/
void Voice_Process(void)
{
	uint8_t i;
  uint16_t cal_crc;
	uint16_t rx_crc;
	if( UsartToVoice.Usart_Rx_OK == true){
		if(UsartToVoice.Rx_Buf[0] == 0x01 && UsartToVoice.Rx_Buf[1] == 0x03){
			rx_crc = (uint16_t)UsartToVoice.Rx_Buf[UsartToVoice.Rx_Len-1] << 8 | UsartToVoice.Rx_Buf[UsartToVoice.Rx_Len-2];
			cal_crc = CRC_Compute ( UsartToVoice.Rx_Buf, UsartToVoice.Rx_Len-2);
			if(rx_crc == cal_crc){
		 		memcpy(&Environ.voice, &UsartToVoice.Rx_Buf[ 3 ],sizeof(int16_t));
				Environ.voice = (short int) t_ntohs(Environ.voice);
				//printf("voice: %d\r\n",Environ.voice);
			}
		}
		UsartToVoice.Usart_Rx_OK = false;
	}
}

/*=============================================================================
*  ������ ��Send_TempHumMess
*  ��   �ߣ�hrx
*  ����ʱ�䣺2022��4��11�� 
*  �޸�ʱ�䣺
*  ��   �룺 
*           
*  ��   ����
*  ˵   ����������������������
*/
void Send_TempHumMess(uint8_t index,uint8_t addr,Environmental_Typedef *_environ)
{
	uint8_t *buf;
	uint8_t sramx=0;					//Ĭ��Ϊ�ڲ�sram
  buf = mymalloc(sramx,20);//����20�ֽ�
	
	buf[0] = 0x55;
	buf[1] = 0xAA;
	buf[2] = index;
	buf[3] = 0x13;
	buf[4] = 0x01;
	buf[5] = addr;
	buf[6] = 0x62;
	buf[7] = 0x08;
	buf[8] = (_environ->CO2>>8)&0x00FF;
	buf[9] = _environ->CO2&0x00FF;
	buf[10] = (_environ->VOC>>8)&0x00FF;
	buf[11] = _environ->VOC&0x00FF;
	buf[12] = (_environ->Humi>>8)&0x00FF;
	buf[13] = _environ->Humi&0x00FF;;
	buf[14] = (_environ->Tempera>>8)&0x00FF;;
	buf[15] = _environ->Tempera&0x00FF;;;
	buf[16]= CRC8_Table(buf, 16);
	buf[17]= 0x0D;
	buf[18]= 0x0A;
	
	USARTx_SendMultibyte(USART_PC, buf, SEND_PC_LEN);
	myfree(sramx,buf);
}

/*=============================================================================
*  ������ ��Send_EnvironMess
*  ��   �ߣ�hrx
*  ����ʱ�䣺2022��4��11�� 
*  �޸�ʱ�䣺
*  ��   �룺 
*           
*  ��   ����
*  ˵   �������ͻ�������������
*/
void Send_EnvironMess(uint8_t index,uint8_t addr,Environmental_Typedef *_environ)
{
	uint8_t *buf;
	uint8_t sramx=0;					//Ĭ��Ϊ�ڲ�sram
  buf = mymalloc(sramx,20);//����20�ֽ�
	
	buf[0] = 0x55;
	buf[1] = 0xAA;
	buf[2] = index;
	buf[3] = 0x13;
	buf[4] = 0x01;
	buf[5] = addr;
	buf[6] = 0x63;
	buf[7] = 0x08;
	buf[8] = (_environ->PM2_5>>8)&0x00FF;
	buf[9] = _environ->PM2_5&0x00FF;
	buf[10] = (_environ->PM10>>8)&0x00FF;
	buf[11] = _environ->PM10&0x00FF;;
	buf[12] = (_environ->PM1_0>>8)&0x00FF;
	buf[13] = _environ->PM1_0&0x00FF;;
	buf[14] = (_environ->state>>8)&0x00FF;;
	buf[15] = _environ->state&0x00FF;
	buf[16]= CRC8_Table(buf, 16);
	buf[17]= 0x0D;
	buf[18]= 0x0A;
	USARTx_SendMultibyte(USART_PC, buf, SEND_PC_LEN);
	myfree(sramx,buf);
}

/*=============================================================================
*  ������ ��CRC_Sum
*  ��   �ߣ�hrx
*  ����ʱ�䣺2022��4��11�� 
*  �޸�ʱ�䣺
*  ��   �룺 
*           
*  ��   ����
*  ˵   ���������
*/
uint8_t CRC_Sum(uint8_t *buf,uint8_t num)
{
	uint8_t i = 0;
	uint8_t sum = 0;
	for(i = 0;i < num;i++){
		sum += buf[i];
	}
	return (uint8_t)(256 - sum);
}

/*=============================================================================
*  ������ ��CRC_Compute
*  ��   �ߣ�hrx
*  ����ʱ�䣺2022��4��11�� 
*  �޸�ʱ�䣺
*  ��   �룺 
*           
*  ��   ����
*  ˵   ��������
*/
unsigned int CRC_Compute ( unsigned char *arr_buff, unsigned char len)
{
	unsigned int crc=0xFFFF;
	unsigned char i, j;
	for ( j=0; j <len;j++){
		crc=crc ^*arr_buff++;
		for ( i=0; i<8; i++){
			if( ( crc&0x0001) >0){
				crc=crc>>1;
				crc=crc^ 0xa001;
			}
			else
			crc=crc>>1;
		}
	}
	return ( crc);
}