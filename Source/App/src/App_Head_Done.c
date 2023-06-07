/*=============================================================================
*  Copyright (C) ".strftime("%Y")." All rights reserved.
*
*  文件名："App_Head_Done.c"
*  作者：hrx
*  创建时间：2022年4月11日 
*  修改时间：
*  说明：头部控制处理
*
==============================================================================*/

/* Includes ------------------------------------------------------------------*/

#include "includes.h"

#define  UD_MAX_      180

#define  UD_MIN_      0


Head_Pos_Type Head_Status;
                                                                                                                                                  

uint8_t autoscan_flag ;

/*=============================================================================
*  函数名 ：Head_Angle_Control
*  作   者：hrx
*  创建时间：2022年4月11日 
*  修改时间：
*  输   入：
*           
*  输   出：
*  说   明：设定俯仰角度和水平旋转角度
*/
void Head_Angle_Control(int level, int pitch)
{
	 Head_UD_To_SomeWhere(pitch);    
	 Head_Level_To_SomeWhere(level);
}

/*=============================================================================
*  函数名 ：Moto_Pit
*  作   者：hrx
*  创建时间：2022年4月11日 
*  修改时间：
*  输   入：
*           
*  输   出：
*  说   明：俯仰控制
*/
void Moto_Pit(uint8_t Pit_dir)
{
	switch(Pit_dir)
	{
		case PSC_MOVE_UP:
			if(Head_Status.PSC_UD_Pos < Head_Status.PSC_UD_MAX_Pos)  {
				Head_Status.PSC_UD_Pos += 0.5;
			}else{
			}
			SetHeadPitchPosition(Head_Status.PSC_UD_Pos, Head_Status.Pitch_Offset);
			break;
		case PSC_MOVE_DOWN:
			if(Head_Status.PSC_UD_Pos > Head_Status.PSC_UD_MIN_Pos){
				Head_Status.PSC_UD_Pos -= 0.5;
			}else{
			}
			SetHeadPitchPosition(Head_Status.PSC_UD_Pos, Head_Status.Pitch_Offset);
			break;
		default:break;
	}
	
}

/*=============================================================================
*  函数名 ：Moto_Level
*  作   者：hrx
*  创建时间：2022年4月11日 
*  修改时间：
*  输   入：
*           
*  输   出：
*  说   明：水平旋转控制
*/
void Moto_Level(uint8_t Level_dir)
{
	switch(Level_dir)
	{
		case PSC_MOVE_LEFT:
			if(Head_Status.PSC_Level_Pos >= Head_Status.PSC_Level_MIN_Pos + 0.5){
				Head_Status.PSC_Level_Pos -= 0.5;
			}else{
				Head_Status.PSC_Level_Pos = Head_Status.PSC_Level_MIN_Pos;
			}
			SetHeadLevelPosition(Head_Status.PSC_Level_Pos,	Head_Status.Level_Offset);
			
			break;
			
		case PSC_MOVE_RIGHT:
			if(Head_Status.PSC_Level_Pos <= Head_Status.PSC_Level_MAX_Pos - 0.5) {
				Head_Status.PSC_Level_Pos += 0.5;
			}else{
				Head_Status.PSC_Level_Pos =  Head_Status.PSC_Level_MAX_Pos ;
			}
			SetHeadLevelPosition(Head_Status.PSC_Level_Pos,	Head_Status.Level_Offset);
			break;
		default:break;
	}
}


/*=============================================================================
*  函数名 ：Head_UD_To_SomeWhere
*  作   者：hrx
*  创建时间：2022年4月11日 
*  修改时间：
*  输   入：
*           
*  输   出：
*  说   明：俯仰到指定的位置
*/
void Head_UD_To_SomeWhere(float pos)
{
	Head_Status.PSC_UD_Goal_Pos = pos;
	if(Head_Status.PSC_UD_Pos < Head_Status.PSC_UD_Goal_Pos - 0.5)
	{
		// Up
		Head_Status.PSC_Dir  |= 1<<(6-1);
		Head_Status.PSC_Dir  &= ~(1<<(8-1));
	}else if(Head_Status.PSC_UD_Pos > Head_Status.PSC_UD_Goal_Pos + 0.5){
		// Down
		Head_Status.PSC_Dir  |= 1<<(8-1);
		Head_Status.PSC_Dir  &= ~(1<<(6-1));
	}else{
		Head_Status.PSC_Dir  &= ~(1<<(6-1));
		Head_Status.PSC_Dir  &= ~(1<<(8-1));
	}
}

/*=============================================================================
*  函数名 ：Head_Level_To_SomeWhere
*  作   者：hrx
*  创建时间：2022年4月11日 
*  修改时间：
*  输   入：
*           
*  输   出：
*  说   明：水平旋转到指定的位置
*/
void Head_Level_To_SomeWhere(float pos)
{
	Head_Status.PSC_Level_Goal_Pos = pos;
	if(Head_Status.PSC_Level_Pos > Head_Status.PSC_Level_Goal_Pos + 0.5)
	{
		// left
		Head_Status.PSC_Dir |= 1<<(5-1);
		Head_Status.PSC_Dir &= ~(1<< (7-1));
	}else if(Head_Status.PSC_Level_Pos < Head_Status.PSC_Level_Goal_Pos - 0.5){
		// right
		Head_Status.PSC_Dir |= 1<<(7-1);
		Head_Status.PSC_Dir &= ~(1<< (5-1));
	}else{
		Head_Status.PSC_Dir  &= ~(1<<(5-1));
		Head_Status.PSC_Dir  &= ~(1<<(7-1));
	}
}

/*=============================================================================
*  函数名 ：Head_Level_To_SomeWhere
*  作   者：hrx
*  创建时间：2022年4月11日 
*  修改时间：
*  输   入：
*           
*  输   出：
*  说   明：头部复位，归正
*/
void Head_Back()
{
	// 俯仰
	Head_UD_To_SomeWhere(Head_Status.PSC_UD_Default_Pos);
	// 水平  
	Head_Level_To_SomeWhere(Head_Status.PSC_Level_Default_Pos);
	
}

/*=============================================================================
*  函数名 ：Check_Assign_Ok
*  作   者：hrx
*  创建时间：2022年4月11日 
*  修改时间：
*  输   入：
*           
*  输   出：
*  说   明：是否转到位，转到位则停止
*/
void Check_Assign_Ok()
{
	if(Head_Status.PSC_Level_Goal_Pos > 0)    // 水平目标值
	{
		if(fabs(Head_Status.PSC_Level_Pos - Head_Status.PSC_Level_Goal_Pos)< 0.5)
		{
			//左右停止
			Head_Status.PSC_Dir &= ~((1 << (5-1)) | (1<<(7-1)));
			Head_Status.PSC_Level_Goal_Pos = 0;
			
		}
	}
	
	if(Head_Status.PSC_UD_Goal_Pos > 0)        // 垂直目标值
	{
		if(fabs(Head_Status.PSC_UD_Pos - Head_Status.PSC_UD_Goal_Pos)<0.5)
		{
			//俯仰停止
			Head_Status.PSC_Dir &= ~((1 << (6-1)) | (1<<(8-1)));
			Head_Status.PSC_UD_Goal_Pos = 0;			
		}
	}
}

/*=============================================================================
*  函数名 ：Head_Control_Done
*  作   者：hrx
*  创建时间：2022年4月11日 
*  修改时间：
*  输   入：
*           
*  输   出：
*  说   明：头部转动控制
*/
void Head_Control_Done()
{
	OS_ERR err;
	if(Head_Status.PSC_Dir == 0){
	}else if((Head_Status.PSC_Dir & (1<<(5-1)))!= 0 ){
		Moto_Level(PSC_MOVE_LEFT);  //5  左  
	}else if((Head_Status.PSC_Dir &(1<<(7-1))) != 0 ){
			Moto_Level(PSC_MOVE_RIGHT);	//7  右   为 0
		}

	if((Head_Status.PSC_Dir &(1<<(6-1))) != 0){
		Moto_Pit(PSC_MOVE_UP);	//这个是 上
	}else if((Head_Status.PSC_Dir &(1<<(8-1))) != 0){
		Moto_Pit(PSC_MOVE_DOWN);	//这个是 下 
	}else if(Head_Status.PSC_Dir == 9){
		Head_Back();    //回中
	}

	if((Head_Status.PSC_Dir & 0xF0) == 0){
		Head_Status.PSC_Dir = 0;
	}

	if(Head_Status.PSC_Dir != 0){
		Check_Assign_Ok();
	}

}




