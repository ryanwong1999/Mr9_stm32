#ifndef   __APP_HEAD_DONE_H__
#define   __APP_HEAD_DONE_H__

//#include  "App_StepMoto.h"
//#include  "App_Limit.h"
//#include  "Bsp_PWMCfg.h"
//#include  "Bsp_TIM2Cfg.h"
//#include  "App_AnalysisCMD.h"
#ifdef __cplusplus
 extern "C" {
#endif 
	 
/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
	 

/* 垂直默认位置 */
#define  H_Pos_Default     	75.0

//#define PSC_UD_OFFSET	    30    // 郑州1
//#define PSC_LEVEL_OFFSET  38
	 
#define PSC_UD_OFFSET	    	45
#define PSC_LEVEL_OFFSET  	45	 
	 
#define PSC_UD_DEFAULT     	90
#define PSC_UD_MAX         	135
#define PSC_UD_MIN         	45

#define PSC_LEVEL_DEFAULT   90
#define PSC_LEVEL_MAX       180
#define PSC_LEVEL_MIN       0
	                                                                                                                                                                                              
#define PSC_MOVE_LEFT				2
#define PSC_MOVE_RIGHT			3

#define PSC_MOVE_UP					2
#define PSC_MOVE_DOWN				3

typedef struct
{
	uint8_t PSC_Dir;       			  	// 电机的方向
	uint8_t PSC_Limit_Flag;  				// 限位开关的状态
	uint16_t PSC_MAX_Step;   				// 从左到右最大的 步数
	uint16_t PSC_Now_Step;					// 左右的当前 步数
	uint16_t PSC_Level_Goal_Step;		// 这个是 水平电机的 目标 数
	uint8_t PSC_Pitch_OK_Flag;
	int8_t Level_Offset;
	int8_t Pitch_Offset;
	float PSC_UD_Pos;    						// 俯仰电机的角度
	float PSC_UD_Goal_Pos;          // 这个是 俯仰电机的  目标 位置  对应的数
	float PSC_UD_Default_Pos;       // 这个是 俯仰电机在中间的 pos 数
	float PSC_UD_MAX_Pos;						// 这个是 最大的  pwm值  上下的
	float PSC_UD_MIN_Pos;
	float PSC_Level_Pos;    				// 俯仰电机的角度
	float PSC_Level_Goal_Pos;       // 这个是 俯仰电机的  目标 位置  对应的数
	float PSC_Level_Default_Pos;    // 这个是 俯仰电机在中间的 pos 数
	float PSC_Level_MAX_Pos;				// 这个是 最大的  pwm值  上下的
	float PSC_Level_MIN_Pos;
}Head_Pos_Type;	 

extern Head_Pos_Type Head_Status;

void Head_Control_Done(void);
void Head_UD_To_SomeWhere(float pos);
void Head_Level_To_SomeWhere(float pos);
void Head_Angle_Control(int level, int pitch);
void Head_Back(void);

#ifdef __cplusplus
}
#endif

#endif
