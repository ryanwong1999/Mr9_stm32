/*=============================================================================
*   Copyright (C) ".strftime("%Y")." All rights reserved.
*
*   @FileName："App_MotoCtrl.c"
*   @Author：zz(zhangzhen123good@163.com)
*   @Created：".strftime("%Y年%m月%d日")) 
*   @Description：
*
==============================================================================*/
/* Includes ------------------------------------------------------------------*/

#include "includes.h"

//#define bei 1

Moto_Typedef  Robot_Moto ;


void  Moto_Param_Init(void)
{
//	Robot_Moto.Left_Speed = 0;
	//Robot_Moto.Right_Speed = 0;
	Robot_Moto.Left_Dir = 0 ;      // 默认0
	Robot_Moto.Right_Dir = 0 ;      // 默认0
}

/*=============================================================================
*  函数名 ：Moto_Unlock
*  作   者：zz  (zhangzhen123good@163.com)
*  创建时间：2016年07月25日
*  修改时间：
*  输   入：
*  输   出：
*  说   明：电机解锁
*/
void  Moto_Unlock(void)
{
	//MOTO_LEFT_EN_ENABLE ;	
	//MOTO_RIGHT_EN_ENABLE ;
	MOTO_LEFT_EN_DISABLE ;    // 电机 >10ms 高电平脉冲解锁
	MOTO_RIGHT_EN_DISABLE ;   // 电机 >10ms 高电平脉冲解锁

	delay_ms(40);
	MOTO_LEFT_EN_ENABLE ;	
	MOTO_RIGHT_EN_ENABLE ;
	//MOTO_LEFT_EN_DISABLE ;
	//MOTO_RIGHT_EN_DISABLE ;	
}


void Moto_Set_Speed(uint8_t ucChannel, int16_t sSpeed)
{	
		switch(ucChannel)
		{
			case MOTO_LEFT_CH:
					if(sSpeed > 0){
							sSpeed = sSpeed ;
						 MOTO_LEFT_DIR_FRONT ;
						 MOTO_LEFT_EN_ENABLE ;
						
					}else if(sSpeed < 0){
							MOTO_LEFT_DIR_BACK ;
						 MOTO_LEFT_EN_ENABLE ;
							sSpeed = -sSpeed ;
					}else{
						MOTO_LEFT_EN_DISABLE ;

					}
				PWM_Output_Set(MOTO_LEFT_CH, sSpeed);	
				break;
					
					
			case MOTO_RIGHT_CH:
					if(sSpeed > 0){
						MOTO_RIGHT_DIR_FRONT ;
						//MOTO_RIGHT_EN_DISABLE ;
						 MOTO_RIGHT_EN_ENABLE ;
							sSpeed = sSpeed ;
					}else if(sSpeed < 0){
							MOTO_RIGHT_DIR_BACK ;
						 MOTO_RIGHT_EN_ENABLE ;
							sSpeed = -sSpeed ;
					}else{
						MOTO_RIGHT_EN_DISABLE ;

					}					
				PWM_Output_Set(MOTO_RIGHT_CH, sSpeed);	
				break;
					
			default :break;
		}
}

//暴死 操作
void Sport_die_off(void){
	
	Robot_Moto.Left_Dir =0;
	Robot_Moto.Right_Dir = 0;
	Robot_Moto.Left_PWM_Value = 0 ;
	Robot_Moto.Right_PWM_Value = 0;
	Moto_Set_Speed(MOTO_LEFT_CH,0);
	Moto_Set_Speed(MOTO_RIGHT_CH,0);
	
}
void Sport_Stop(void)
{
	Robot_Moto.Robot_Dir = MOVE_STOP;
	Robot_Moto.Slow_Flag = 0;
	Robot_Moto.Stop_Flag = 0;
	Robot_Moto.Left_Dir = 0;
	Robot_Moto.Right_Dir = 0;

	Robot_Moto.Left_PWM_Value = 0 ;
	Robot_Moto.Right_PWM_Value = 0;
	Robot_Moto.Left_PWM_Last = 0;
	Robot_Moto.Right_PWM_Last = 0;
	Robot_Moto.Left_Pulse_Cnt = 0;
	Robot_Moto.Right_Pulse_Cnt = 0;
	Moto_Set_Speed(MOTO_LEFT_CH,0);
  Moto_Set_Speed(MOTO_RIGHT_CH, 0);
	

}

//
void MotoCtrl_Forward(void)
{	
	Robot_Moto.Robot_Dir = MOVE_BACK;
	Robot_Moto.Left_Dir = -1;
	Robot_Moto.Right_Dir = -1;
	//Robot_Moto.Left_PWM_Value = 150 ;
	//Robot_Moto.Right_PWM_Value = 150;
	//Robot_Moto.Left_PWM_Value = -110 ; //郑州
	//Robot_Moto.Right_PWM_Value = -110;//郑州
		Robot_Moto.Left_PWM_Value = -90;  //武汉
	Robot_Moto.Right_PWM_Value = -90;   //武汉
	Moto_Set_Speed(MOTO_LEFT_CH, Robot_Moto.Left_PWM_Value);
  Moto_Set_Speed(MOTO_RIGHT_CH, Robot_Moto.Right_PWM_Value);
}
			
void MotoCtrl_TurnRight(void)
{
	Robot_Moto.Robot_Dir = MOVE_RIGHT;
	Robot_Moto.Left_Dir = 0;
	Robot_Moto.Right_Dir = -1;
	Robot_Moto.Left_PWM_Value = 0 ;
	Robot_Moto.Right_PWM_Value = -110;
	//Robot_Moto.Right_PWM_Value = 240;
	Moto_Set_Speed(MOTO_LEFT_CH, Robot_Moto.Left_PWM_Value);
  Moto_Set_Speed(MOTO_RIGHT_CH, Robot_Moto.Right_PWM_Value);
	//Moto_Set_Speed(MOTO_LEFT_CH, 0);
 // Moto_Set_Speed(MOTO_RIGHT_CH, -150);
}

void MotoCtrl_TurnLeft(void)
{
	Robot_Moto.Robot_Dir = MOVE_LEFT;
	Robot_Moto.Left_Dir = -1;
	Robot_Moto.Right_Dir = 0;
	Robot_Moto.Left_PWM_Value = -110 ;
	//Robot_Moto.Left_PWM_Value = 240 ;
	Robot_Moto.Right_PWM_Value = 0;
	Moto_Set_Speed(MOTO_LEFT_CH, Robot_Moto.Left_PWM_Value);
  Moto_Set_Speed(MOTO_RIGHT_CH, Robot_Moto.Right_PWM_Value);
	//Moto_Set_Speed(MOTO_LEFT_CH, -150);
  //Moto_Set_Speed(MOTO_RIGHT_CH, 0);
}

void MotoCtrl_TurnRightZeroRadius(void)
{
	Robot_Moto.Robot_Dir = MOVE_RIGHT;
	Robot_Moto.Left_Dir = 1;
	Robot_Moto.Right_Dir = -1;
	Robot_Moto.Left_PWM_Value = 90 ;
	Robot_Moto.Right_PWM_Value = -90;
		//Robot_Moto.Left_PWM_Value = 160 ;
	//Robot_Moto.Right_PWM_Value = 160;
	Moto_Set_Speed(MOTO_LEFT_CH, Robot_Moto.Left_PWM_Value);
	Moto_Set_Speed(MOTO_RIGHT_CH, Robot_Moto.Right_PWM_Value);
	
}

void MotoCtrl_TurnLeftZeroRadius(void)
{
	Robot_Moto.Robot_Dir = MOVE_LEFT;
	Robot_Moto.Left_Dir = -1;
	Robot_Moto.Right_Dir = 1;
	Robot_Moto.Left_PWM_Value = -85 ;
	Robot_Moto.Right_PWM_Value = 85;
		//Robot_Moto.Left_PWM_Value = 160 ;
	//Robot_Moto.Right_PWM_Value = 160;
 Moto_Set_Speed(MOTO_LEFT_CH, Robot_Moto.Left_PWM_Value);
 Moto_Set_Speed(MOTO_RIGHT_CH, Robot_Moto.Right_PWM_Value);
}
void Sport_Back(void)
{
	Robot_Moto.Robot_Dir = MOVE_FRONT;
	Robot_Moto.Left_Dir = 1;
	Robot_Moto.Right_Dir = 1;
	Robot_Moto.Left_PWM_Value = 120 ;
	Robot_Moto.Right_PWM_Value = 120;
	//Robot_Moto.Left_PWM_Value = 150 ;
	//Robot_Moto.Right_PWM_Value = 150;
	Moto_Set_Speed(MOTO_LEFT_CH, Robot_Moto.Left_PWM_Value);
	Moto_Set_Speed(MOTO_RIGHT_CH, Robot_Moto.Right_PWM_Value);
}

uint8_t  Wheel_Locded_Check(void)
{
	static uint8_t locked_sta = 0;
	static uint16_t locked_tim = 0;
	if(Robot_Moto.Left_PWM_Value == 0 && Robot_Moto.Right_PWM_Value == 0){
		//Robot_Moto.Locked_Flag = 0;
		locked_sta = 0;
		locked_tim = 0;
	}else{
		locked_tim ++;
		if(locked_sta == 0){
			if(locked_tim > 500){
				locked_tim = 0;
				locked_sta |= 0x08;
				Robot_Moto.Left_Locked_Pulse = 0;
				Robot_Moto.Right_Locked_Pulse = 0;
			}
		}else{
			if(locked_tim > 400){
				locked_tim = 0;
				if(Robot_Moto.Right_Locked_Pulse > 40){
					locked_sta |= 0x01;
				}else{
					locked_sta &= 0xfe;
				}
				
				if(Robot_Moto.Left_Locked_Pulse > 40){
					locked_sta |= 0x02;
				}else{
					locked_sta &= 0xfd;
				}
				
				Robot_Moto.Left_Locked_Pulse = 0;
				Robot_Moto.Right_Locked_Pulse = 0;
				
			}
		}
	}
	return (locked_sta&0x03);
}

void Get_Wheel_Dir(float _left_speed, float _right_speed,int8_t *_left_dir, int8_t *_right_dir)
{
		if(_left_speed > 0){
			*_left_dir = 1;
		}else if(_left_speed < 0){
			*_left_dir = -1;
		 }else{
		 *_left_dir = 0;
		 }
		  
		if(_right_speed > 0){
			*_right_dir = 1;
		}else if(_right_speed < 0){
			*_right_dir = -1;
		 }else{
		 	*_right_dir = 0;
		 }
}

uint8_t Get_Robot_Dir(float _left_pwm, float _right_pwm)
{
	uint8_t dir_tmp;
	
	if(_left_pwm > 0 && _right_pwm > 0){
		dir_tmp = MOVE_FRONT;
	}else if(_left_pwm < 0 && _right_pwm < 0){
		dir_tmp = MOVE_BACK;
	}else if(_left_pwm > 0 && _right_pwm <= 0){
		dir_tmp = MOVE_RIGHT;
	}else if(_left_pwm <= 0 && _right_pwm > 0){
		dir_tmp = MOVE_LEFT;
	}else{
		dir_tmp = MOVE_STOP;
	}
	return dir_tmp;
}


void Speed_Regulation_Process(uint16_t _set_period_l,uint16_t _set_period_r, uint8_t turn_sta)
{
	static int16_t left_pwm_temp = 0;
  static int16_t right_pwm_temp = 0;	
	
	if((SPEED_CAP_STA & 0x08) == 0x08){
		SPEED_CAP_STA &= 0xF0; 
		Speed_Set_Process(_set_period_r,Robot_Moto.Right_Period,&Robot_Moto.Right_PWM_Value);
	}
		 
	if((SPEED_CAP_STA & 0x80) == 0x80) {
		SPEED_CAP_STA &= 0x0F;
		Speed_Set_Process(_set_period_l,Robot_Moto.Left_Period,&Robot_Moto.Left_PWM_Value);
	}

}

void Speed_Set_Process(uint16_t set_period,uint16_t real_Period, int16_t *pwm_set)
{
	if(set_period > 30000){
	*pwm_set = 0;
	}else if(real_Period<set_period){
		if(real_Period<set_period-set_period/10){
			if(*pwm_set>MIN_PWM){
			  *pwm_set-=10;
				if(*pwm_set < MIN_PWM)
					 *pwm_set = MIN_PWM;
		   }
		}else if(real_Period<set_period-set_period/20){
			if(*pwm_set>MIN_PWM){
				*pwm_set-=10;
				if(*pwm_set < MIN_PWM)
					*pwm_set = MIN_PWM;
			}
		}else if(real_Period<set_period-set_period/30){
			if(*pwm_set>MIN_PWM){
				*pwm_set-=6;
				if(*pwm_set < MIN_PWM)
					*pwm_set = MIN_PWM;
			}
		}else if(real_Period<set_period-set_period/40){
			if(*pwm_set>MIN_PWM){
				*pwm_set-=4;
				if(*pwm_set < MIN_PWM)
				 *pwm_set = MIN_PWM;
			}
		}else if(real_Period<set_period-set_period/50){
			if(*pwm_set>MIN_PWM){
				*pwm_set-=2;
				if(*pwm_set < MIN_PWM)
					*pwm_set = MIN_PWM;
			}
		}
	}else{
		if(real_Period>set_period+set_period/10){
			*pwm_set+=10;
			if(*pwm_set>MAX_PWM)
				*pwm_set=MAX_PWM;
		}else if(real_Period>set_period+set_period/20){
			*pwm_set+=10;
			if(*pwm_set>MAX_PWM)
				*pwm_set=MAX_PWM; 
		}else if(real_Period>set_period+set_period/30){
			*pwm_set+=6;
			if(*pwm_set>MAX_PWM)
				*pwm_set=MAX_PWM;
		}else if(real_Period>set_period+set_period/40) {
			*pwm_set+=4;
			if(*pwm_set>MAX_PWM)
				*pwm_set=MAX_PWM;
		}else if(real_Period>set_period+set_period/50){
			*pwm_set += 2;
			if(*pwm_set>MAX_PWM)
				*pwm_set=MAX_PWM;
		}
	}	
}



/*******************************************************************************
*	函数名: Speed_up
*	功  能: 加速处理
* 参  数: 无
*	返  回: 无
*/

void Speed_Up_Down(int16_t rate,int16_t set_pwm,int16_t *pwm_value)
{
	if(set_pwm > 0){
		if(*pwm_value < set_pwm - rate){
			*pwm_value += rate;
		}else if(*pwm_value > set_pwm + rate){
				*pwm_value -= rate;
		}else{
			*pwm_value = set_pwm;
		}
		if(*pwm_value > 900){
		  *pwm_value = 900;
		}
	}else{
		if(*pwm_value > set_pwm + rate){
			*pwm_value -= rate;
		}else if(*pwm_value < set_pwm - rate){
		*pwm_value += rate;
		}else{
			*pwm_value = set_pwm;
		}
		
		if(*pwm_value < -900){
		  *pwm_value = -900;
		}
	}
}
/*
void Speed_Down(int16_t rate,int16_t set_pwm,int16_t *pwm_value)
{
	if(*pwm_value > set_pwm){
		*pwm_value -= rate;
	}else{
		*pwm_value = set_pwm;
	}
}*/
void Speed_Inc(int16_t rate,int16_t _setL_pwm,int16_t _setR_pwm,Moto_Typedef *_moto)
{
	//if(AGV_Dly1ms == 0)
	//{
		//AGV_Dly1ms = 10;
	// if(_moto->Startup_Flag == 1){
		if(_moto->Left_PWM_Value < _setL_pwm){
			_moto->Left_PWM_Value  += rate;
		}else{
			_moto->Left_PWM_Value = _setL_pwm;
		}
			
		if(_moto->Right_PWM_Value < _setR_pwm){
			_moto->Right_PWM_Value += rate;
		}else{
			_moto->Right_PWM_Value = _setR_pwm;
		}
	 //}
}

/*******************************************************************************
*	函数名: Speed_down
*	功  能: 加速处理
* 参  数: 无
*	返  回: 无
*/

void Speed_Del(int16_t rate,int16_t _setL_pwm,int16_t _setR_pwm,Moto_Typedef *_moto)
{
	//if(AGV_Dly1ms == 0  )
		//{
			//AGV_Dly1ms = 20;
			if(_moto->Left_PWM_Value >_setL_pwm)
				{
					_moto->Left_PWM_Value -= rate;
				}
			else 
				_moto->Left_PWM_Value = _setL_pwm;
			if(_moto->Right_PWM_Value > _setR_pwm)
				{
					_moto->Right_PWM_Value -= rate;
				}
			else 
				_moto->Right_PWM_Value = _setR_pwm;
		//}
}



void Speed_PID_Process(void)
{
	static uint8_t speed_sta = 0;
	static int8_t  last_left_dir = 0; 
  static int8_t  last_right_dir = 0;	
	static  uint16_t left_period_set;
	static  uint16_t right_period_set;
	static uint16_t speed_set_delay = 0;
	static int8_t left_dir = 0;
	static int8_t right_dir = 0;
	static uint8_t check_cnt = 0;
	static uint16_t beep_cnt = 0;
	static uint8_t pwm_flag = 0;
	static int16_t left_inc = 0;
	static int16_t right_inc= 0;
	static uint8_t last_left_pwm = 0;
	static uint8_t last_right_pwm = 0;
	if(Robot_Sys.Remote_flag == true){
   
		Get_Wheel_Dir(Robot_Moto.Left_Speed, Robot_Moto.Right_Speed, &left_dir, &right_dir);
		if(Robot_Moto.Left_PWM_Value ==0 && Robot_Moto.Right_PWM_Value == 0){
		 	Robot_Moto.Left_Period = 0xffff;
	    Robot_Moto.Right_Period = 0xffff;
			 SPEED_CAP_STA = 0;
		}
		
		if(left_dir != 0 || right_dir != 0){
		 if((last_left_dir != left_dir || last_right_dir != right_dir) ){
			// printf("left: %d, %d, right: %d, %d\r\n",last_left_dir,left_dir, last_right_dir,right_dir);
		 if(left_dir != right_dir && (Robot_Moto.Left_PWM_Value !=0 || Robot_Moto.Right_PWM_Value != 0)){

			Robot_Moto.Left_PWM_Value = 0;
		  Robot_Moto.Right_PWM_Value = 0;
		 	Moto_Set_Speed(MOTO_LEFT_CH,Robot_Moto.Left_PWM_Value );
		  Moto_Set_Speed(MOTO_RIGHT_CH,Robot_Moto.Right_PWM_Value );	
			delay_ms(200);
			if(last_left_dir != left_dir && last_right_dir == right_dir){
				Robot_Moto.Left_PWM_Value = 100;
				Robot_Moto.Right_PWM_Value = 0;
			}
			if( last_right_dir != right_dir && last_left_dir == left_dir){
			Robot_Moto.Left_PWM_Value = 0;
			Robot_Moto.Right_PWM_Value = 100;
			}
			}
			last_left_dir = left_dir;
	    last_right_dir = right_dir;

		 }
		}
		
		//if(Robot_Moto.Startup_Flag == 1 && (fabs(Robot_Moto.Lear) >= 0.001 || fabs(Robot_Moto.Angular) >= 0.001)){
		if(Robot_Moto.Startup_Flag == 1 && fabs(Robot_Moto.Lear) >= 0.001 && fabs(Robot_Moto.Angular) <= 0.001){
			INCPID_Init();
			
			switch(speed_sta)
			{
				case 0x00:
		       right_period_set = Robot_Moto.Right_Period_Set;
					 left_period_set = Robot_Moto.Left_Period_Set;
					 Robot_Moto.Left_PWM_Value = 80;
					 Robot_Moto.Right_PWM_Value = 80;
             pwm_flag = 1;
					 check_cnt ++;
						if(check_cnt > 10){
							check_cnt = 0;
							speed_sta ++;
						}
					break;
				case 0x01:
					 Robot_Moto.Left_PWM_Value += 5;
					 Robot_Moto.Right_PWM_Value += 5;
				   pwm_flag = 1;
					delay_ms(20);
					
					break;
				default:
				      Robot_Moto.Startup_Flag = 0;
					break;
			}
					
		}else{
			 if(Robot_Moto.Startup_Flag == 1){
			  Robot_Moto.Startup_Flag = 0;
			  Robot_Moto.Left_PWM_Value = 100;
				Robot_Moto.Right_PWM_Value = 100;
			 }
				if(Robot_Moto.Lear != 0 && Robot_Moto.Angular !=0){
					
				}

		 	if(Robot_Moto.Lear != 0 || Robot_Moto.Angular != 0){	
			 right_period_set = Robot_Moto.Right_Period_Set;
			 left_period_set = Robot_Moto.Left_Period_Set;
				
			}
			check_cnt = 0;
			speed_sta = 1;
      pwm_flag = 0;
		}
	//printf("stop:%d, Dir : %d, Obs: %02x\r\n",Robot_Sys.Mergency_Stop_flag,Robot_Moto.Robot_Dir,Robot_Sys.Ultra_sta);
	if(Robot_Sys.Mergency_Stop_flag == true){
		  speed_sta = 0;	
			Robot_Moto.Left_PWM_Value = 0;
		  Robot_Moto.Right_PWM_Value = 0;
		 	Moto_Set_Speed(MOTO_LEFT_CH,Robot_Moto.Left_PWM_Value );
		  Moto_Set_Speed(MOTO_RIGHT_CH,Robot_Moto.Right_PWM_Value );	
		  Robot_Moto.Startup_Flag = 1;
		 // Robot_Sys.Remote_flag = false;
		  SPEED_CAP_STA = 0;
			delay_ms(500);
			last_left_dir = 0;
			left_dir = 0;
			last_right_dir = 0;
			right_dir = 0;
	}else if(fabs(Robot_Moto.Lear) <= 0.001 && fabs(Robot_Moto.Angular) <= 0.001){
      speed_sta = 0;	
			Robot_Moto.Left_PWM_Value = 0;
		  Robot_Moto.Right_PWM_Value = 0;
		 	Moto_Set_Speed(MOTO_LEFT_CH,Robot_Moto.Left_PWM_Value );
		  Moto_Set_Speed(MOTO_RIGHT_CH,Robot_Moto.Right_PWM_Value );	
		  Robot_Moto.Startup_Flag = 1;
		  //Robot_Sys.Remote_flag = false;
		  SPEED_CAP_STA = 0;
			delay_ms(500);
			last_left_dir = 0;
			left_dir = 0;
			last_right_dir = 0;
			right_dir = 0;
	}
	
	else if(Robot_Moto.Robot_Dir == MOVE_FRONT &&((Robot_Sys.Ultra_sta & 0x07) != 0 \
	|| (Robot_Sys.Obs_IR_Front & 0x07) != 0 || (Robot_Sys.Obs_IR_Bottom & 0x03) != 0)){
      speed_sta = 0;	
			Robot_Moto.Left_PWM_Value = 0;
		  Robot_Moto.Right_PWM_Value = 0;
		 	Moto_Set_Speed(MOTO_LEFT_CH,Robot_Moto.Left_PWM_Value );
		  Moto_Set_Speed(MOTO_RIGHT_CH,Robot_Moto.Right_PWM_Value );	
		  Robot_Moto.Startup_Flag = 1;
		  //Robot_Sys.Remote_flag = false;
		  SPEED_CAP_STA = 0;
			delay_ms(500);
			last_left_dir = 0;
			left_dir = 0;
			last_right_dir = 0;
			right_dir = 0;

		}else if(Robot_Moto.Robot_Dir == MOVE_BACK && ((Robot_Sys.Ultra_sta & 0x30) != 0 || (Robot_Sys.Obs_IR_Bottom & 0x30) != 0)){
      speed_sta = 0;	
			Robot_Moto.Left_PWM_Value = 0;
		  Robot_Moto.Right_PWM_Value = 0;
		 	Moto_Set_Speed(MOTO_LEFT_CH,Robot_Moto.Left_PWM_Value );
		  Moto_Set_Speed(MOTO_RIGHT_CH,Robot_Moto.Right_PWM_Value );	
		  Robot_Moto.Startup_Flag = 1;
		 // Robot_Sys.Remote_flag = false;
		  SPEED_CAP_STA = 0;
			delay_ms(500);
			last_left_dir = 0;
			left_dir = 0;
			last_right_dir = 0;
			right_dir = 0;
		}else if((Robot_Moto.Robot_Dir == MOVE_LEFT || Robot_Moto.Robot_Dir == MOVE_RIGHT) && (Robot_Sys.Obs_IR_Bottom & 0x33) != 0){
			speed_sta = 0;	
			Robot_Moto.Left_PWM_Value = 0;
		  Robot_Moto.Right_PWM_Value = 0;
		 	Moto_Set_Speed(MOTO_LEFT_CH,Robot_Moto.Left_PWM_Value );
		  Moto_Set_Speed(MOTO_RIGHT_CH,Robot_Moto.Right_PWM_Value );	
		  Robot_Moto.Startup_Flag = 1;
		 // Robot_Sys.Remote_flag = false;
		  SPEED_CAP_STA = 0;
			delay_ms(500);
			last_left_dir = 0;
			left_dir = 0;
			last_right_dir = 0;
			right_dir = 0;
		}
		else{
			Robot_Moto.PID_Flag = 1;
		 if(speed_sta != 0){	
			  if(Robot_Moto.Right_Tim10us > 6000){
			Robot_Moto.Right_Tim10us = 0;
			Robot_Moto.Right_PWM_Value += 10;
			Robot_Moto.Right_Period = 20000;
      if(Robot_Moto.Right_PWM_Value > 200){
				Robot_Moto.Left_PWM_Value = 0;
		  Robot_Moto.Right_PWM_Value = 0;
		 	Moto_Set_Speed(MOTO_LEFT_CH,Robot_Moto.Left_PWM_Value );
		  Moto_Set_Speed(MOTO_RIGHT_CH,Robot_Moto.Right_PWM_Value );
				delay_ms(500);
			}					
		}else if( pwm_flag == 1){
			if(Robot_Moto.Right_Period < Robot_Moto.Right_Period_Set * 1.1 ){
				 pwm_flag = 0;
				Robot_Moto.Startup_Flag = 0;
			}
		}else if((SPEED_CAP_STA & 0x08) == 0x08){
		 SPEED_CAP_STA &= 0xF7; 
			Robot_Moto.Right_Tim10us = 0;
	  if(Robot_Moto.PID_Flag != 0){
			right_inc = Get_INCPID_PWM(&RightPID , Robot_Moto.Right_Period_Set,\
			Robot_Moto.Right_Period , Robot_Moto.Right_PWM_Value);
			if(Robot_Moto.Right_Dir != Robot_Moto.Right_Real_Dir){
				 Robot_Moto.Right_PWM_Value ++;
			}else{
			  if(Robot_Moto.Right_PWM_Value > right_inc){
			   Robot_Moto.Right_PWM_Value -= right_inc;
			}
			}

		}else{
			Speed_Set_Process(right_period_set,Robot_Moto.Right_Period,&Robot_Moto.Right_PWM_Value);
		}
		 
	}
		
	if(Robot_Moto.Left_Tim10us > 6000){
		Robot_Moto.Left_Tim10us  = 0;
	  Robot_Moto.Left_PWM_Value += 10;
		Robot_Moto.Left_Period = 20000;
		if( Robot_Moto.Left_PWM_Value > 200){
			Robot_Moto.Left_PWM_Value = 0;
		  Robot_Moto.Right_PWM_Value = 0;
		 	Moto_Set_Speed(MOTO_LEFT_CH,Robot_Moto.Left_PWM_Value );
		  Moto_Set_Speed(MOTO_RIGHT_CH,Robot_Moto.Right_PWM_Value );
				delay_ms(500);
		}
	}else if( pwm_flag == 1){
		if(Robot_Moto.Left_Period < Robot_Moto.Left_Period_Set *1.1 ){
				pwm_flag = 0;
			//printf("555\r\n");
			Robot_Moto.Startup_Flag = 0;
			}
	}else if((SPEED_CAP_STA & 0x80) == 0x80) {
		SPEED_CAP_STA &= 0x7F;
		Robot_Moto.Left_Tim10us  = 0;
		if(Robot_Moto.PID_Flag != 0){
		left_inc= Get_INCPID_PWM(&LeftPID , Robot_Moto.Left_Period_Set, \
		  Robot_Moto.Left_Period , Robot_Moto.Left_PWM_Value);
			// Robot_Moto.Left_PWM_Value -=left_inc;
			if(Robot_Moto.Left_Dir != Robot_Moto.Left_Real_Dir){
				 Robot_Moto.Left_PWM_Value ++;
//				if(Robot_Moto.Left_PWM_Value < 0){
//				  Robot_Moto.Left_PWM_Value ++;
//				}else{
//					Robot_Moto.Left_PWM_Value --;
//				}
				
			}else{
			if(Robot_Moto.Left_PWM_Value > left_inc){
			  Robot_Moto.Left_PWM_Value -=left_inc;
			}
		}
		}else{
		//printf("---%d, %d\r\n",Robot_Moto.Left_PWM_Value,left_inc);
		 Speed_Set_Process(left_period_set,Robot_Moto.Left_Period,&Robot_Moto.Left_PWM_Value);
		}
		
	 }
	}
 
	if(Robot_Moto.Left_PWM_Value > 900){
	 Robot_Moto.Left_PWM_Value = 900;
	}
	if(Robot_Moto.Right_PWM_Value > 900){
	 Robot_Moto.Right_PWM_Value = 900;
	}
	//printf("dir: %d, %d\r\n",Robot_Moto.Left_Dir,Robot_Moto.Left_Dir);
	if(left_dir< 0 ){
			Robot_Moto.Left_Dir = -1;
		 if(Robot_Moto.Left_PWM_Value < 25 &&  Robot_Moto.Left_Period < Robot_Moto.Left_Period_Set*0.9 && Robot_Moto.Left_Dir == Robot_Moto.Left_Real_Dir){
			Robot_Moto.Left_PWM_Value = 0;
			 last_left_pwm = 0;
			 //printf("L--%d, %d\r\n",Robot_Moto.Left_Period,Robot_Moto.Left_Real_Dir);
		 }else{
			 
			 if(last_left_pwm == 0){
			  last_left_pwm = 1;
				if(Robot_Moto.Left_PWM_Value < 30)
				Robot_Moto.Left_PWM_Value = 30;
			 }
		 }
		 //printf("L1: %d,%d, %d , %d\r\n",Robot_Moto.Left_Period_Set,Robot_Moto.Left_Period,Robot_Moto.Left_PWM_Value,Robot_Moto.Left_Real_Dir);
		 Moto_Set_Speed(MOTO_LEFT_CH,-Robot_Moto.Left_PWM_Value);
		}else{
			Robot_Moto.Left_Dir = 1;
		if(Robot_Moto.Left_PWM_Value < 25 &&   Robot_Moto.Left_Period < Robot_Moto.Left_Period_Set*0.9 && Robot_Moto.Left_Dir == Robot_Moto.Left_Real_Dir){
			Robot_Moto.Left_PWM_Value = 0;
			 last_left_pwm = 0;
			//printf("L--%d, %d\r\n",Robot_Moto.Left_Period,Robot_Moto.Left_Real_Dir);
		 }else{
			  if(last_left_pwm == 0){
			  last_left_pwm = 1;
				if(Robot_Moto.Left_PWM_Value < 30)
				Robot_Moto.Left_PWM_Value = 30;
			 }
		 }
		 //printf("L2: %d\r\n",Robot_Moto.Left_PWM_Value);
		 	Moto_Set_Speed(MOTO_LEFT_CH,Robot_Moto.Left_PWM_Value );
		}
		if(right_dir < 0){
			Robot_Moto.Right_Dir = -1;
		 if(Robot_Moto.Right_PWM_Value < 25 &&   Robot_Moto.Right_Period < Robot_Moto.Right_Period_Set*0.9 && Robot_Moto.Right_Dir == Robot_Moto.Right_Real_Dir){
			Robot_Moto.Right_PWM_Value = 0;
			 last_right_pwm = 0;
		 }else{
			  if(last_right_pwm == 0){
			  last_right_pwm = 1;
				if(Robot_Moto.Right_PWM_Value < 30)
				Robot_Moto.Right_PWM_Value = 30;
			 }
		 
		 }
		// printf("R1: %d\r\n",Robot_Moto.Right_PWM_Value);
		  Moto_Set_Speed(MOTO_RIGHT_CH,-Robot_Moto.Right_PWM_Value );
		}else{
			Robot_Moto.Right_Dir = 1;
		 if(Robot_Moto.Right_PWM_Value < 25 && Robot_Moto.Right_Period < Robot_Moto.Right_Period_Set*0.9 &&Robot_Moto.Right_Dir == Robot_Moto.Right_Real_Dir){
			Robot_Moto.Right_PWM_Value = 0;
			  last_right_pwm = 0;
			 //printf("R-- %d, %d\r\n",Robot_Moto.Right_Period,Robot_Moto.Right_Real_Dir);
		 }else{
			 	if(last_right_pwm == 0){
			  last_right_pwm = 1;
				if(Robot_Moto.Right_PWM_Value < 30)
				Robot_Moto.Right_PWM_Value = 30;
			 }
		 }
		// printf("R2:%d,%d, %d, %d\r\n\r\n",Robot_Moto.Right_Period_Set,Robot_Moto.Right_Period,Robot_Moto.Right_PWM_Value,Robot_Moto.Right_Real_Dir);
		  Moto_Set_Speed(MOTO_RIGHT_CH,Robot_Moto.Right_PWM_Value );
		}
	} 
		}

}


void Chg_Speed_PID_Process(void)
{
	static uint8_t speed_sta = 0;
	static int8_t  last_left_dir = 0; 
  static int8_t  last_right_dir = 0;	
	static  uint16_t left_period_set;
	static  uint16_t right_period_set;
	static uint16_t speed_set_delay = 0;
	static int8_t left_dir = 0;
	static int8_t right_dir = 0;
	static uint8_t check_cnt = 0;
	static uint16_t beep_cnt = 0;
	static uint8_t pwm_flag = 0;
	static int16_t left_inc = 0;
	static int16_t right_inc= 0;

	switch (AutoCharge.Move_Sta)
		{
			case CHG_MOVE_FORWARD:  
				
	if(Robot_Moto.Right_Tim10us > 20000 && Robot_Moto.Right_Period_Set != 0){
			Robot_Moto.Right_Tim10us = 0;
			Robot_Moto.Right_PWM_Value += 20;
			 Robot_Moto.Right_Period = 20000;
			 SPEED_CAP_STA &= 0xF0;
		}else if((SPEED_CAP_STA & 0x08) == 0x08){
			Robot_Moto.Right_Tim10us = 0;
		 SPEED_CAP_STA &= 0xF7;

			right_inc = Get_Chg_INCPID(&RightPID , Robot_Moto.Right_Period_Set,\
			Robot_Moto.Right_Period , Robot_Moto.Right_PWM_Value);
			if(Robot_Moto.Right_PWM_Value > right_inc){
			 Robot_Moto.Right_PWM_Value -= right_inc;
		}
		 //printf("right_inc %d\r\n",right_inc);
	}
		
	if(Robot_Moto.Left_Tim10us > 20000 && Robot_Moto.Left_Period_Set != 0){
		Robot_Moto.Left_Tim10us  = 0;
	  Robot_Moto.Left_PWM_Value += 20;
		Robot_Moto.Left_Period = 20000;
		SPEED_CAP_STA &= 0x0F;
		//printf("Left_Tim10us : %d, Robot_Moto.PID_Flag :%d\r\n",Robot_Moto.Left_Tim10us,Robot_Moto.PID_Flag);
	}else if((SPEED_CAP_STA & 0x80) == 0x80) {
		SPEED_CAP_STA &= 0x7F;
		//SPEED_CAP_STA &= 0x0F;
		Robot_Moto.Left_Tim10us = 0;
		left_inc= Get_Chg_INCPID(&LeftPID , Robot_Moto.Left_Period_Set, \
		  Robot_Moto.Left_Period , Robot_Moto.Left_PWM_Value);
			if(Robot_Moto.Left_PWM_Value > left_inc){
			  Robot_Moto.Left_PWM_Value -=left_inc;
			}
			 //printf("left_inc  %d\r\n",left_inc);
	 }
	if(Robot_Moto.Left_PWM_Value > 300){
	  Robot_Moto.Left_PWM_Value = 300;
	 }else if(Robot_Moto.Left_PWM_Value < 60){
	  Robot_Moto.Left_PWM_Value = 60;
	 }
		 if(Robot_Moto.Right_PWM_Value > 300){
	  Robot_Moto.Right_PWM_Value = 300;
	 }else if(Robot_Moto.Right_PWM_Value < 60){
	  Robot_Moto.Right_PWM_Value = 60;
	 }
	
	 //printf("front: %d, %d\r\n",Robot_Moto.Left_PWM_Value,Robot_Moto.Right_PWM_Value);
	  Moto_Set_Speed(MOTO_LEFT_CH,-Robot_Moto.Left_PWM_Value );
		 Moto_Set_Speed(MOTO_RIGHT_CH,-Robot_Moto.Right_PWM_Value );
	 
			  break;
	   	  case CHG_MOVE_RIGHT:

			if(Robot_Moto.Right_Tim10us > 20000 && Robot_Moto.Right_Period_Set != 0){
			Robot_Moto.Right_Tim10us = 0;
			Robot_Moto.Right_PWM_Value += 20;
				Robot_Moto.Right_Period = 20000;
				SPEED_CAP_STA &= 0xF0;
		}else if((SPEED_CAP_STA & 0x08) == 0x08){
			Robot_Moto.Right_Tim10us = 0;
		 SPEED_CAP_STA &= 0xF7;
			right_inc = Get_Chg_INCPID(&RightPID , Robot_Moto.Right_Period_Set,\
			Robot_Moto.Right_Period , Robot_Moto.Right_PWM_Value);
			if(Robot_Moto.Right_PWM_Value > right_inc){
			 Robot_Moto.Right_PWM_Value -= right_inc;
		
		}
		// printf("right_inc %d\r\n",right_inc);
	}
		if(Robot_Moto.Left_PWM_Value > 300){
	  Robot_Moto.Left_PWM_Value = 300;
	 }else if(Robot_Moto.Left_PWM_Value < 60){
	  Robot_Moto.Left_PWM_Value = 60;
	 }
			 if(Robot_Moto.Right_PWM_Value > 300){
	  Robot_Moto.Right_PWM_Value = 300;
	 }else if(Robot_Moto.Right_PWM_Value < 60){
	  Robot_Moto.Right_PWM_Value = 60;
	 }
		 //printf("tunright: %d, %d\r\n",Robot_Moto.Left_PWM_Value,Robot_Moto.Right_PWM_Value);
			Moto_Set_Speed(MOTO_LEFT_CH,0 );
		  Moto_Set_Speed(MOTO_RIGHT_CH,-Robot_Moto.Right_PWM_Value );
			  break;
			 
		    case CHG_MOVE_LEFT: 
	if(Robot_Moto.Left_Tim10us > 20000 && Robot_Moto.Left_Period_Set != 0){
		Robot_Moto.Left_Tim10us  = 0;
	  Robot_Moto.Left_PWM_Value += 20;
		Robot_Moto.Left_Period = 20000;
		SPEED_CAP_STA &= 0x0F;
		//printf("Left_Tim10us : %d, Robot_Moto.PID_Flag :%d\r\n",Robot_Moto.Left_Tim10us,Robot_Moto.PID_Flag);
	}else if((SPEED_CAP_STA & 0x80) == 0x80) {
		SPEED_CAP_STA &= 0x7F;
		//SPEED_CAP_STA &= 0x0F;
		Robot_Moto.Left_Tim10us = 0;
		left_inc= Get_Chg_INCPID(&LeftPID , Robot_Moto.Left_Period_Set, \
		  Robot_Moto.Left_Period , Robot_Moto.Left_PWM_Value);
			if(Robot_Moto.Left_PWM_Value > left_inc){
			  Robot_Moto.Left_PWM_Value -=left_inc;
			}
     //printf("left_inc %d\r\n",left_inc);
	 } 
				 if(Robot_Moto.Left_PWM_Value > 300){
	  Robot_Moto.Left_PWM_Value = 300;
	 }else if(Robot_Moto.Left_PWM_Value < 60){
	  Robot_Moto.Left_PWM_Value = 60;
	 }
		 if(Robot_Moto.Right_PWM_Value > 300){
	  Robot_Moto.Right_PWM_Value = 300;
	 }else if(Robot_Moto.Right_PWM_Value < 60){
	  Robot_Moto.Right_PWM_Value = 60;
	 }
 //printf("turnleft: %d, %d\r\n",Robot_Moto.Left_PWM_Value,Robot_Moto.Right_PWM_Value);	
				Moto_Set_Speed(MOTO_LEFT_CH,-Robot_Moto.Left_PWM_Value );
		  Moto_Set_Speed(MOTO_RIGHT_CH,0);
		    break;
		 	
		    case CHG_MOVE_ZERORIGHT:
					
		 if(Robot_Moto.Right_Tim10us > 20000 && Robot_Moto.Right_Period_Set != 0){
			Robot_Moto.Right_Tim10us = 0;
			Robot_Moto.Right_PWM_Value += 20;
			 Robot_Moto.Right_Period = 20000;
			 SPEED_CAP_STA &= 0xF0;
		}else if((SPEED_CAP_STA & 0x08) == 0x08){
			Robot_Moto.Right_Tim10us = 0;
		 SPEED_CAP_STA &= 0xF7;
			right_inc = Get_Chg_INCPID(&RightPID , Robot_Moto.Right_Period_Set,\
			Robot_Moto.Right_Period , Robot_Moto.Right_PWM_Value);
			if(Robot_Moto.Right_PWM_Value > right_inc){
			 Robot_Moto.Right_PWM_Value -= right_inc;
			}
	 //printf("right_inc %d\r\n",right_inc);
	}
		
	if(Robot_Moto.Left_Tim10us > 20000 && Robot_Moto.Left_Period_Set != 0){
		Robot_Moto.Left_Tim10us  = 0;
	  Robot_Moto.Left_PWM_Value += 20;
		Robot_Moto.Left_Period = 20000;
		SPEED_CAP_STA &= 0x0F;
		//printf("Left_Tim10us : %d, Robot_Moto.PID_Flag :%d\r\n",Robot_Moto.Left_Tim10us,Robot_Moto.PID_Flag);
	}else if((SPEED_CAP_STA & 0x80) == 0x80) {
		SPEED_CAP_STA &= 0x7F;
		//SPEED_CAP_STA &= 0x0F;
		Robot_Moto.Left_Tim10us = 0;
		left_inc= Get_Chg_INCPID(&LeftPID , Robot_Moto.Left_Period_Set, \
		  Robot_Moto.Left_Period , Robot_Moto.Left_PWM_Value);
			if(Robot_Moto.Left_PWM_Value > left_inc){
			  Robot_Moto.Left_PWM_Value -=left_inc;
			}
			// printf("left_inc %d\r\n",left_inc);
	 }
		if(Robot_Moto.Left_PWM_Value > 300){
	  Robot_Moto.Left_PWM_Value = 300;
	 }else if(Robot_Moto.Left_PWM_Value < 60){
	  Robot_Moto.Left_PWM_Value = 60;
	 }
		 if(Robot_Moto.Right_PWM_Value > 300){
	  Robot_Moto.Right_PWM_Value = 300;
	 }else if(Robot_Moto.Right_PWM_Value < 60){
	  Robot_Moto.Right_PWM_Value = 60;
	 }
	// printf("zeroright: %d, %d\r\n",Robot_Moto.Left_PWM_Value,Robot_Moto.Right_PWM_Value);
				Moto_Set_Speed(MOTO_LEFT_CH,Robot_Moto.Left_PWM_Value );
		   Moto_Set_Speed(MOTO_RIGHT_CH,-Robot_Moto.Right_PWM_Value );
				break;
		 
		  case CHG_MOVE_ZEROLEFT:
			 if(Robot_Moto.Right_Tim10us > 20000 && Robot_Moto.Right_Period_Set != 0){
			Robot_Moto.Right_Tim10us = 0;
			Robot_Moto.Right_PWM_Value += 20;
				Robot_Moto.Right_Period = 20000;
				 SPEED_CAP_STA &= 0xF0;
		}else if((SPEED_CAP_STA & 0x08) == 0x08){
			Robot_Moto.Right_Tim10us = 0;
		 SPEED_CAP_STA &= 0xF7;
			right_inc = Get_Chg_INCPID(&RightPID , Robot_Moto.Right_Period_Set,\
			Robot_Moto.Right_Period , Robot_Moto.Right_PWM_Value);
			if(Robot_Moto.Right_PWM_Value > right_inc){
			 Robot_Moto.Right_PWM_Value -= right_inc;
			}
		 //printf("right_inc %d\r\n",right_inc);
	}
		
	if(Robot_Moto.Left_Tim10us > 20000 && Robot_Moto.Left_Period_Set != 0){
		Robot_Moto.Left_Tim10us  = 0;
	  Robot_Moto.Left_PWM_Value += 20;
		Robot_Moto.Left_Period = 20000;
		SPEED_CAP_STA &= 0x0F;
		//printf("Left_Tim10us : %d, Robot_Moto.PID_Flag :%d\r\n",Robot_Moto.Left_Tim10us,Robot_Moto.PID_Flag);
	}else if((SPEED_CAP_STA & 0x80) == 0x80) {
		SPEED_CAP_STA &= 0x7F;
		//SPEED_CAP_STA &= 0x0F;
		Robot_Moto.Left_Tim10us = 0;

		left_inc= Get_Chg_INCPID(&LeftPID , Robot_Moto.Left_Period_Set, \
		  Robot_Moto.Left_Period , Robot_Moto.Left_PWM_Value);
			if(Robot_Moto.Left_PWM_Value > left_inc){
			  Robot_Moto.Left_PWM_Value -=left_inc;
			}
			 //printf("left_inc %d\r\n",left_inc);
	 }
	if(Robot_Moto.Left_PWM_Value > 300){
	  Robot_Moto.Left_PWM_Value = 300;
	 }else if(Robot_Moto.Left_PWM_Value < 60){
	  Robot_Moto.Left_PWM_Value = 60;
	 }
	 if(Robot_Moto.Right_PWM_Value > 300){
	  Robot_Moto.Right_PWM_Value = 300;
	 }else if(Robot_Moto.Right_PWM_Value < 60){
	  Robot_Moto.Right_PWM_Value = 60;
	 }
	

		Moto_Set_Speed(MOTO_LEFT_CH,-Robot_Moto.Left_PWM_Value );
		Moto_Set_Speed(MOTO_RIGHT_CH,Robot_Moto.Right_PWM_Value );
				break;
			
			 default:
				
			 break;
		}
//		 printf("zeroleft: %d, %d\r\n",Robot_Moto.Left_PWM_Value,Robot_Moto.Right_PWM_Value);
//		printf("set: %d, %d\r\n",Robot_Moto.Left_Period_Set, Robot_Moto.Right_Period_Set);
//		printf("real:%d, %d\r\n",Robot_Moto.Left_Period, Robot_Moto.Right_Period);
//		printf("\r\n");

}




