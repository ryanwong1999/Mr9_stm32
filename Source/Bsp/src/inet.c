/*=============================================================================
*  Copyright (C) ".strftime("%Y")." All rights reserved.
*
*  �ļ�����"App_AnalysisCMD.c"
*  ���ߣ�zz  (zhangzhen123good@163.com)
*  ����ʱ�䣺2016��07��25�� 
*  �޸�ʱ�䣺
*  ˵����
*
==============================================================================*/
/* Includes ------------------------------------------------------------------*/
#include  <stdio.h>
#include  <string.h>
#include  "inet.h"


// ������˷���1��С�˷���0
int checkCPUendian(void)  
{  
	union{  
		unsigned long int i;  
		unsigned char s[4];  
	}c;    
	c.i = 0x12345678;  
	return (0x12 == c.s[0]);  
}  
  
// ģ��htonl�����������ֽ���ת�����ֽ���
unsigned long int t_htonl(unsigned long int h)  
{  
	// ������Ϊ��ˣ��������ֽ���ͬ��ֱ�ӷ���
	// ������ΪС�ˣ�ת���ɴ���ٷ���
	return checkCPUendian() ? h : BigLittleSwap32(h);  
}  
  
// ģ��ntohl�����������ֽ���ת�����ֽ���
unsigned long int t_ntohl(unsigned long int n)  
{  
	// ������Ϊ��ˣ��������ֽ���ͬ��ֱ�ӷ���
	// ������ΪС�ˣ���������ת����С���ٷ��� 
	return checkCPUendian() ? n : BigLittleSwap32(n);  
}  

// ģ��htons�����������ֽ���ת�����ֽ���
unsigned short int t_htons(unsigned short int h)
{
	// ������Ϊ��ˣ��������ֽ���ͬ��ֱ�ӷ���
	// ������ΪС�ˣ�ת���ɴ���ٷ���
	return checkCPUendian() ? h : BigLittleSwap16(h);
}
 
// ģ��ntohs�����������ֽ���ת�����ֽ���
unsigned short int t_ntohs(unsigned short int n)
{
	// ������Ϊ��ˣ��������ֽ���ͬ��ֱ�ӷ���
	// ������ΪС�ˣ���������ת����С���ٷ���
	return checkCPUendian() ? n : BigLittleSwap16(n);
}


