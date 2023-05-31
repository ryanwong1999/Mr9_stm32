/*=============================================================================
*  Copyright (C) ".strftime("%Y")." All rights reserved.
*
*  文件名："App_AutoCharge.c"
*  作者：hrx
*  创建时间：2022年4月11日 
*  修改时间：
*  说明：自动充电处理
*
==============================================================================*/


#include "includes.h"

Charge_typedef AutoCharge;

/*=============================================================================
*  函数名 ：Charge_Tim1ms
*  作   者：hrx
*  创建时间：2022年4月11日 
*  修改时间：
*  输   入：
*           
*  输   出：
*  说   明：充电延时标志
*
==============================================================================*/
void Charge_Tim1ms(void)
{
  AutoCharge.OverTime ++;
	AutoCharge.Loop_Tim ++;
	if(Robot_Sys.Last_Task == CHG_TASK && (Pms.Bat_Sta & 0x01) == 0){
		AutoCharge.Time_out ++;
	}else{
		AutoCharge.Time_out = 0;
	}
}


/*=============================================================================
*  函数名 ：Charge_Init
*  作   者：hrx
*  创建时间：2022年4月11日 
*  修改时间：
*  输   入：
*           
*  输   出：
*  说   明：自动充电参数初始化
*
==============================================================================*/
void Charge_Init(void)
{
	AutoCharge.Position = POSITION_NULL;              // ???????
	AutoCharge.Last_Position = POSITION_NULL;         //
	AutoCharge.Loop_Tim = 0;	
	AutoCharge.OverTime = 0;
	AutoCharge.Time_out = 0;
	AutoCharge.NotFind_Flag = false;
	AutoCharge.Move_Sta = CHG_MOVE_STOP;
	
	memset(AutoCharge.CH1_IrDa_Dat, 0x00, 4);  //?150MS????0
	memset(AutoCharge.CH2_IrDa_Dat, 0x00, 4);
	memset(AutoCharge.CH3_IrDa_Dat, 0x00, 4);
	AutoCharge.Move_Sta = CHG_MOVE_STOP;
}


/*=============================================================================
*  函数名 ：Get_Charger_Code
*  作   者：hrx
*  创建时间：2022年4月11日 
*  修改时间：
*  输   入：
*           
*  输   出：
*  说   明：读取自动充电编码
*
==============================================================================*/
void Get_Charger_Code(void)
{
//	AutoCharge.CH_IrDate[0] = AutoCharge.CH1_IrDa_Dat[0]|AutoCharge.CH1_IrDa_Dat[1]|AutoCharge.CH1_IrDa_Dat[2]|AutoCharge.CH1_IrDa_Dat[3];   	//左边接收头收到充电座码则不为0
//	AutoCharge.CH_IrDate[1] = AutoCharge.CH2_IrDa_Dat[0]|AutoCharge.CH2_IrDa_Dat[1]|AutoCharge.CH2_IrDa_Dat[2]|AutoCharge.CH2_IrDa_Dat[3];   	//中间接收头收到的码		没用到
//	AutoCharge.CH_IrDate[2] = AutoCharge.CH3_IrDa_Dat[0]|AutoCharge.CH3_IrDa_Dat[1]|AutoCharge.CH3_IrDa_Dat[2]|AutoCharge.CH3_IrDa_Dat[3];  	//右边接收头收到的码
//	AutoCharge.CH_IrDate[3] = AutoCharge.CH1_IrDa_Dat[0]|AutoCharge.CH2_IrDa_Dat[0]|AutoCharge.CH3_IrDa_Dat[0];    	//收到充电座左边的码则不为0
//	AutoCharge.CH_IrDate[4] = AutoCharge.CH1_IrDa_Dat[1]|AutoCharge.CH2_IrDa_Dat[1]|AutoCharge.CH3_IrDa_Dat[1];   	//充电座左上的码
//	AutoCharge.CH_IrDate[5] = AutoCharge.CH1_IrDa_Dat[2]|AutoCharge.CH2_IrDa_Dat[2]|AutoCharge.CH3_IrDa_Dat[2];   	//充电座右边的码
//	AutoCharge.CH_IrDate[6] = AutoCharge.CH1_IrDa_Dat[3]|AutoCharge.CH2_IrDa_Dat[3]|AutoCharge.CH3_IrDa_Dat[3];   	//充电座右上的码
	
	AutoCharge.CH_IrDate[0] = AutoCharge.CH1_IrDa_Dat[0]|AutoCharge.CH1_IrDa_Dat[1]|AutoCharge.CH1_IrDa_Dat[2]|AutoCharge.CH1_IrDa_Dat[3];   	//左边接收头收到充电座码则不为0
	AutoCharge.CH_IrDate[2] = AutoCharge.CH3_IrDa_Dat[0]|AutoCharge.CH3_IrDa_Dat[1]|AutoCharge.CH3_IrDa_Dat[2]|AutoCharge.CH3_IrDa_Dat[3];  	//右边接收头收到的码
	
	AutoCharge.CH_IrDate[3] = AutoCharge.CH1_IrDa_Dat[0]|AutoCharge.CH3_IrDa_Dat[0];    //收到充电座左边的码则不为0
	AutoCharge.CH_IrDate[4] = AutoCharge.CH1_IrDa_Dat[1]|AutoCharge.CH3_IrDa_Dat[1];   	//充电座左上的码
	AutoCharge.CH_IrDate[5] = AutoCharge.CH1_IrDa_Dat[2]|AutoCharge.CH3_IrDa_Dat[2];   	//充电座右边的码
	AutoCharge.CH_IrDate[6] = AutoCharge.CH1_IrDa_Dat[3]|AutoCharge.CH3_IrDa_Dat[3];   	//充电座右上的码
}

/*=============================================================================
*  函数名 ：AutoCharge_Processing
*  作   者：hrx
*  创建时间：2022年4月11日 
*  修改时间：
*  输   入：
*           
*  输   出：
*  说   明：自动充电处理函数
*
==============================================================================*/
void AutoCharge_Processing(void)
{
  static u8 Position_Cnt=0;

	Get_Charger_Code();
	
	if((AutoCharge.Chg_Sta & 0x20) == 0){						// 首次进入充电程序 0010 0000
		AutoCharge.Chg_Sta |= 0x20;        					
		AutoCharge.Chg_Sta &= 0x20;
		Charge_Init();
	}else{
		if(AutoCharge.Position == POSITION_NULL){
			AutoCharge.Chg_Sta &= 0xfe;		//1111 1110    									
//			if((AutoCharge.CH_IrDate[0] == 0) && (AutoCharge.CH_IrDate[1] == 0) && (AutoCharge.CH_IrDate[2] == 0)){						//完全没收到信号
			if((AutoCharge.CH_IrDate[0] == 0) && (AutoCharge.CH_IrDate[2] == 0)){						//完全没收到信号
				Position_Cnt = 0;
				AutoCharge.Position = POSITION_NULL;              
				AutoCharge.Last_Position = POSITION_NULL;          
				AutoCharge.Move_Sta = CHG_MOVE_ZEROLEFT;
				if(AutoCharge.OverTime > 60000){ 				
					AutoCharge.Chg_Sta |= 0x40;		//0100 0000 				
					AutoCharge.NotFind_Flag = true;		//找不到充电桩
				}
			}else{		//有收到信号
				AutoCharge.Chg_Sta |= 0x01;    
				Position_Cnt = 0;
				AutoCharge.Position = POSITION_IR;		        
				Chg_Position_Judge();		//机器人相对充电桩位置判断
				if((AutoCharge.CH_IrDate[4] != 0) && (AutoCharge.CH_IrDate[6] == 0)){			
					AutoCharge.Last_Position = POSITION_LEFT;
					AutoCharge.Move_Sta = CHG_MOVE_ZERORIGHT;
				}else if((AutoCharge.CH_IrDate[4] == 0) && (AutoCharge.CH_IrDate[6] != 0)){ 			
					AutoCharge.Last_Position = POSITION_RIGHT;
					AutoCharge.Move_Sta = CHG_MOVE_ZEROLEFT;
				}else if((AutoCharge.CH_IrDate[4] != 0) && (AutoCharge.CH_IrDate[6] != 0)){
					AutoCharge.Last_Position = POSITION_MID;		
					AutoCharge.Move_Sta = CHG_MOVE_FORWARD;
				}else{		//两边都为0
					AutoCharge.Last_Position = AutoCharge.Last_Position;
					AutoCharge.Move_Sta = CHG_MOVE_ZEROLEFT;
				}		
			}
		}else{
			Chg_Position_Judge();                   			// 位置判断   
			Charge_OverTime_Process();              			// 超时处理
			Charge_Crash_Pross();                   			// 充电时碰撞处理
			Charge_IRCode_Process(AutoCharge.Move_Sta);		// 收到的码进行数据运动处理         
			AutoCharge_move(AutoCharge.Move_Sta);         // 行走处理
			if(AutoCharge.Position == POSITION_IR){				// 根据这个标志位进入上次充电桩位置判断   
				Chg_Last_Position_Judge();
			}
//			if(AutoCharge.CH_IrDate[0] == 0 && AutoCharge.CH_IrDate[1] == 0 && AutoCharge.CH_IrDate[2] == 0){			//完全没收到信号
			if((AutoCharge.CH_IrDate[0] == 0) && (AutoCharge.CH_IrDate[2] == 0)){						//完全没收到信号
				Position_Cnt++;
				if(Position_Cnt > 200){
					Position_Cnt = 0; 
					AutoCharge.Position = POSITION_NULL;
					AutoCharge.Last_Position = POSITION_NULL;
				}
			}else{
				Position_Cnt = 0;
			}	
		}

//			printf("CH1_DAT1=%02X CH1_DAT2=%02X CH1_DAT3=%02X  CH1_DAT4=%02X \r\n",AutoCharge.CH1_IrDa_Dat[0],  
//			AutoCharge.CH1_IrDa_Dat[1],AutoCharge.CH1_IrDa_Dat[2],AutoCharge.CH1_IrDa_Dat[3] );

//			printf("CH3_DAT1=%02X CH3_DAT2=%02X CH3_DAT3=%02X CH3_DAT4=%02X \r\n",AutoCharge.CH3_IrDa_Dat[0],   
//			AutoCharge.CH3_IrDa_Dat[1],AutoCharge.CH3_IrDa_Dat[2],AutoCharge.CH3_IrDa_Dat[3] );
//			printf("Move_Sta = %d,%d\r\n",AutoCharge.Position,AutoCharge.Move_Sta);
//			printf("\r\n");
		memset(AutoCharge.CH1_IrDa_Dat, 0x00, 4);  
		memset(AutoCharge.CH3_IrDa_Dat, 0x00, 4);
	}
	AutoCharge_move(AutoCharge.Move_Sta);		// 行走处理    
}

/*=============================================================================
*  函数名 ：AutoCharge_move
*  作   者：hrx
*  创建时间：2022年4月11日 
*  修改时间：
*  输   入： Move_sta1 -> 运动状态
*           
*  输   出：
*  说   明：自动充电速度设置
*
==============================================================================*/

void AutoCharge_move(uint8_t Move_sta1)
{	
	static uint16_t left_period_set;
	static uint16_t right_period_set;
	static int16_t max_pwm;
	static int16_t min_pwm;
	static uint8_t sta_last = 0;
	if(Robot_Sys.Mergency_Stop_flag == true){
		Move_sta1 = CHG_MOVE_STOP;
		sta_last = 0;
	}
	switch (Move_sta1)
		{
			case CHG_MOVE_FORWARD:  
				if(sta_last != Move_sta1){
					sta_last = Move_sta1;
					Moto.cmd_addr1 = 0x2610;
					Moto.cmd_addr2 = 0x3610;
					AutoCharge.set_lear = -60;
					AutoCharge.set_angle = 0;
					}		
				break;
				
			case CHG_MOVE_RIGHT:
				if(sta_last != Move_sta1){
					sta_last = Move_sta1;
					Moto.cmd_addr1 = 0x2610;
					Moto.cmd_addr2 = 0x3610;
					#ifdef ROBOT_M100
					AutoCharge.set_lear = -30;     //60,230
					AutoCharge.set_angle = -110;
					#else						
					AutoCharge.set_lear = -40;     //60,230
					AutoCharge.set_angle = -246;
					#endif
					}
				break;
		 
			case CHG_MOVE_LEFT: 
				if(sta_last != Move_sta1){
					sta_last = Move_sta1;
					Moto.cmd_addr1 = 0x2610;
					Moto.cmd_addr2 = 0x3610;
					#ifdef ROBOT_M100
					AutoCharge.set_lear = -30;
					AutoCharge.set_angle = 110;
					#else
					AutoCharge.set_lear = -40;
					AutoCharge.set_angle = 246;
					#endif
					}
			 break;
		
			case CHG_MOVE_ZERORIGHT:
				if(sta_last != Move_sta1){
					sta_last = Move_sta1;
					Moto.cmd_addr1 = 0x2620;
					Moto.cmd_addr2 = 0x3620;
					#ifdef ROBOT_M100
					AutoCharge.set_lear = 0;
					AutoCharge.set_angle = -200;
					#else
					AutoCharge.set_lear = 0;
					AutoCharge.set_angle = -250;
					#endif
					}
				break;
	 
			case CHG_MOVE_ZEROLEFT:
				if(sta_last != Move_sta1){
					sta_last = Move_sta1;
					Moto.cmd_addr1 = 0x2620;
					Moto.cmd_addr2 = 0x3620;
					#ifdef ROBOT_M100
					AutoCharge.set_lear = 0;
					AutoCharge.set_angle = 200;
					#else
					AutoCharge.set_lear = 0;
					AutoCharge.set_angle = 250;
					#endif
					}
					break;
		
			case CHG_MOVE_BACKWARD:
				Moto.set_left_pwm = 90 ;
				Moto.set_right_pwm = 90;
				break;
			
		 default:
				sta_last = 0;
				break;
	}
}

/*=============================================================================
*  函数名 ：Charge_IRCode_Process
*  作   者：hrx
*  创建时间：2022年4月11日 
*  修改时间：
*  输   入： Move_sta1 -> 运动状态
*           
*  输   出：
*  说   明：运动处理函数
*
==============================================================================*/
void Charge_IRCode_Process(uint8_t _move_sta)
{
	switch(_move_sta)
		{
			case CHG_MOVE_FORWARD :
				chg_move_forward_Process();
				break;		
			case CHG_MOVE_RIGHT :
				chg_move_right_process();
				break;		
			case CHG_MOVE_LEFT :
				chg_move_left_process();
				break;
			case CHG_MOVE_ZERORIGHT:
				chg_move_zeroright_process();		//原地右转
				break;	
			case CHG_MOVE_ZEROLEFT :
				chg_move_zeroleft_process();		//原地左转
				break;
			default :
				break;
	}
}

/*=============================================================================
*  函数名 ：Chg_Position_Judge
*  作   者：hrx
*  创建时间：2022年4月11日 
*  修改时间：
*  输   入： 
*           
*  输   出：
*  说   明：机器人相对充电桩位置判断
*
==============================================================================*/					
void Chg_Position_Judge(void)
{
	static u8 Left_Cnt = 0;
	static u8 Right_Cnt = 0;
	static u8 mid_cnt = 0;
	
	if((AutoCharge.CH1_IrDa_Dat[0] == TXDAT_LEFT) && (AutoCharge.CH3_IrDa_Dat[2] == TXDAT_RIGHT)){		//左、右
		if(AutoCharge.Position == POSITION_IR){
			AutoCharge.Position = POSITION_MID;
		}else{
			mid_cnt++;
			if(mid_cnt >= 1){
				mid_cnt = 0;
				Left_Cnt = 0;
				Right_Cnt = 0;
				AutoCharge.Position = POSITION_MID;
			}
		}
	}else if((AutoCharge.CH1_IrDa_Dat[0] == TXDAT_LEFT) && (AutoCharge.CH3_IrDa_Dat[0] == TXDAT_LEFT)){		//左、左
		if(AutoCharge.Position == POSITION_IR){
			AutoCharge.Position = POSITION_LEFT;
		}else{
			Left_Cnt++;
			Right_Cnt = 0;
			mid_cnt = 0;
			if(Left_Cnt >= 1){
				Left_Cnt = 0;
				Right_Cnt = 0;
				AutoCharge.Position = POSITION_LEFT;
			}
		}		
	}else if((AutoCharge.CH1_IrDa_Dat[2] == TXDAT_RIGHT) && (AutoCharge.CH3_IrDa_Dat[2] == TXDAT_RIGHT)){		//右、右
		if(AutoCharge.Position == POSITION_IR){
			AutoCharge.Position = POSITION_RIGHT;
		}else{
			Left_Cnt = 0;
			mid_cnt = 0;
			Right_Cnt++;
			if(Right_Cnt >= 1){
				Right_Cnt = 0;
				AutoCharge.Position = POSITION_RIGHT; 		
			}		 
		}
	}else{
		if(AutoCharge.Position == POSITION_IR){
			if((AutoCharge.CH_IrDate[3] != 0) && (AutoCharge.CH_IrDate[5] == 0)){		//左、右
				AutoCharge.Position = POSITION_LEFT;
			}else if((AutoCharge.CH_IrDate[3] == 0) && (AutoCharge.CH_IrDate[5] != 0)){
				AutoCharge.Position = POSITION_RIGHT;
			}
		}
		Left_Cnt = 0;
		Right_Cnt = 0;
		mid_cnt = 0;
	}
}
	
/*=============================================================================
*  函数名 ：Chg_Last_Position_Judge
*  作   者：hrx
*  创建时间：2022年4月11日 
*  修改时间：
*  输   入： 
*           
*  输   出：
*  说   明：判断上次机器人位置
*
==============================================================================*/		
void Chg_Last_Position_Judge(void)
{
	if((AutoCharge.CH_IrDate[4] != 0) && (AutoCharge.CH_IrDate[6] == 0)){			
		AutoCharge.Last_Position = POSITION_LEFT;
	}else if((AutoCharge.CH_IrDate[4] == 0) && (AutoCharge.CH_IrDate[6] != 0)){ 			
		AutoCharge.Last_Position = POSITION_RIGHT;
	}else if((AutoCharge.CH_IrDate[4] != 0 ) && (AutoCharge.CH_IrDate[6] != 0)){
		AutoCharge.Last_Position = POSITION_MID;		
	}else{
		AutoCharge.Last_Position = AutoCharge.Last_Position;
	}		
}

/*=============================================================================
*  函数名 ：chg_move_forward_Process
*  作   者：hrx
*  创建时间：2022年4月11日 
*  修改时间：
*  输   入： 
*           
*  输   出：
*  说   明：自动充电前进时处理
*
==============================================================================*/	
void chg_move_forward_Process(void)
{
	static uint8_t cnt1 = 0;
	static uint8_t cnt2 = 0;
	if(AutoCharge.Position == POSITION_LEFT){
		AutoCharge.Move_Sta = CHG_MOVE_RIGHT;
	}else if(AutoCharge.Position == POSITION_RIGHT){
    AutoCharge.Move_Sta = CHG_MOVE_LEFT;
	}else if(AutoCharge.Position == POSITION_MID){
		if(AutoCharge.CH1_IrDa_Dat[0] == TXDAT_LEFT && AutoCharge.CH3_IrDa_Dat[2] == TXDAT_RIGHT){
			if(AutoCharge.CH3_IrDa_Dat[0] == TXDAT_LEFT && AutoCharge.CH1_IrDa_Dat[2] == 0){
				cnt1++;
				cnt2 = 0;
				if(cnt1 >= 2){
				 cnt1 = 0;
				 AutoCharge.Move_Sta = CHG_MOVE_RIGHT;	
				}
			}else if(AutoCharge.CH3_IrDa_Dat[0] == 0 && AutoCharge.CH1_IrDa_Dat[2] == TXDAT_RIGHT){
				cnt1 = 0;
				cnt2++;
				if(cnt2 >= 2){
					cnt2 = 0;
					AutoCharge.Move_Sta = CHG_MOVE_LEFT;
				}			
			}else{
				AutoCharge.Move_Sta = CHG_MOVE_FORWARD;	
			}	
	  }
		
//-----------------------------------------------------------
		else if(AutoCharge.CH1_IrDa_Dat[0] == 0 ){
			if(AutoCharge.CH3_IrDa_Dat[0] == TXDAT_LEFT){
				AutoCharge.Move_Sta = CHG_MOVE_RIGHT;	
			}
	  }else if(AutoCharge.CH3_IrDa_Dat[2] == 0 ){
			if(AutoCharge.CH1_IrDa_Dat[2] == TXDAT_RIGHT){
				AutoCharge.Move_Sta = CHG_MOVE_LEFT;	
			}
		}else{
			if(AutoCharge.CH1_IrDa_Dat[0] == TXDAT_LEFT && AutoCharge.CH3_IrDa_Dat[2] == TXDAT_RIGHT){
				AutoCharge.Move_Sta = CHG_MOVE_FORWARD;	
	    }else{
				AutoCharge.Move_Sta = CHG_MOVE_RIGHT;
			}	 	
		}
//--------------------------------------------------------------------
		
//		else if(AutoCharge.CH1_IrDa_Dat[0]==0 && AutoCharge.CH3_IrDa_Dat[0]==TXDAT_LEFT){
//		 AutoCharge.Move_Sta=CHG_MOVE_RIGHT;	
//		}else if(AutoCharge.CH1_IrDa_Dat[0]==0 && AutoCharge.CH1_IrDa_Dat[2]==TXDAT_RIGHT){
//		 AutoCharge.Move_Sta=CHG_MOVE_LEFT;	
//		}else if(AutoCharge.CH3_IrDa_Dat[2]==0 && AutoCharge.CH1_IrDa_Dat[2]==TXDAT_RIGHT){
//			AutoCharge.Move_Sta=CHG_MOVE_LEFT;
//		}else if(AutoCharge.CH3_IrDa_Dat[2]==0 && AutoCharge.CH3_IrDa_Dat[0]==TXDAT_LEFT){
//		  AutoCharge.Move_Sta=CHG_MOVE_RIGHT;
//		}else{
//			AutoCharge.Move_Sta=CHG_MOVE_RIGHT;	 	
//		}	
		
	}else{       
		if(AutoCharge.CH_IrDate[4] != 0 && AutoCharge.CH_IrDate[6] != 0){
			delay_ms(500);
			AutoCharge.Move_Sta = CHG_MOVE_FORWARD;
			//charge_IR_cnt = 0;
		}else{
			delay_ms(2000);
			if(AutoCharge.CH_IrDate[4] != 0){
					AutoCharge.Move_Sta = CHG_MOVE_ZERORIGHT;
				}else if(AutoCharge.CH_IrDate[6] != 0){
				  AutoCharge.Move_Sta = CHG_MOVE_ZEROLEFT;
				}else if(AutoCharge.Last_Position == POSITION_RIGHT){
					AutoCharge.Move_Sta = CHG_MOVE_ZERORIGHT;	
			}else if(AutoCharge.Last_Position == POSITION_LEFT){
				 AutoCharge.Move_Sta = CHG_MOVE_ZEROLEFT;
			}else{
				AutoCharge.Move_Sta = CHG_MOVE_ZERORIGHT;	
			}				
		}
	}
}

/*=============================================================================
*  函数名 ：chg_move_right_process
*  作   者：hrx
*  创建时间：2022年4月11日 
*  修改时间：
*  输   入： 
*           
*  输   出：
*  说   明：自动充电右转时处理函数
*
==============================================================================*/	
void chg_move_right_process(void)
{
	static uint8_t right_cnt = 0;
	static uint8_t cnt = 0;

	if(AutoCharge.Position == POSITION_LEFT){
    right_cnt = 0;		
		if(AutoCharge.CH_IrDate[3] == 0 && AutoCharge.CH_IrDate[5] == 0){
			if(AutoCharge.No_Sigh_Flag != 1){
				cnt++;
				if(cnt >= 2){
					cnt = 0;
					AutoCharge.Move_Sta = CHG_MOVE_LEFT;
				}
			}
		}else if(AutoCharge.CH1_IrDa_Dat[2] == TXDAT_RIGHT){
			cnt = 0;
			AutoCharge.Move_Sta = CHG_MOVE_LEFT;
		}
		
	}else if(AutoCharge.Position == POSITION_RIGHT){		
		cnt = 0;
		if(AutoCharge.CH1_IrDa_Dat[2] == TXDAT_RIGHT && AutoCharge.CH3_IrDa_Dat[2] == TXDAT_RIGHT){
			 right_cnt = 0;
			AutoCharge.Move_Sta = CHG_MOVE_LEFT;
		}	else if(AutoCharge.CH1_IrDa_Dat[2] == TXDAT_RIGHT && AutoCharge.CH3_IrDa_Dat[2] == 0x0){
			AutoCharge.Move_Sta = CHG_MOVE_LEFT;
		}else if(AutoCharge.CH_IrDate[0] != 0 && AutoCharge.CH_IrDate[2] == 0){
			AutoCharge.Move_Sta = CHG_MOVE_LEFT;
		}
	}else if(AutoCharge.Position == POSITION_MID){
   right_cnt = 0;		
		cnt = 0;
		if(AutoCharge.CH1_IrDa_Dat[2] == TXDAT_RIGHT && AutoCharge.CH3_IrDa_Dat[2] == 0x0){
			AutoCharge.Move_Sta = CHG_MOVE_LEFT;
		}else if(AutoCharge.CH3_IrDa_Dat[0] == TXDAT_LEFT && AutoCharge.CH1_IrDa_Dat[0] == 0x0){
			AutoCharge.Move_Sta = CHG_MOVE_RIGHT;	
	  }else if(AutoCharge.CH1_IrDa_Dat[0] == TXDAT_LEFT && AutoCharge.CH3_IrDa_Dat[2] == TXDAT_RIGHT){
			AutoCharge.Move_Sta = CHG_MOVE_FORWARD;	
	  }
	}else{
		cnt = 0;
		right_cnt = 0;
		if(AutoCharge.CH_IrDate[4] != 0 && AutoCharge.CH_IrDate[6] == 0){
		  AutoCharge.Last_Position = POSITION_LEFT;
		  AutoCharge.Move_Sta = CHG_MOVE_ZERORIGHT;	
	  }else if((AutoCharge.CH_IrDate[4] == 0) && (AutoCharge.CH_IrDate[6] != 0)){
		  AutoCharge.Last_Position = POSITION_RIGHT;
			AutoCharge.Move_Sta = CHG_MOVE_ZEROLEFT;
	  }else if((AutoCharge.CH_IrDate[4]!=0) && (AutoCharge.CH_IrDate[6] != 0)){
			AutoCharge.Last_Position = POSITION_LEFT;	
			AutoCharge.Move_Sta = CHG_MOVE_FORWARD;
		}
	}
}

/*=============================================================================
*  函数名 ：chg_move_left_process
*  作   者：hrx
*  创建时间：2022年4月11日 
*  修改时间：
*  输   入： 
*           
*  输   出：
*  说   明：自动充电左转时处理函数
*
==============================================================================*/	

void chg_move_left_process(void)
{
	static uint8_t left_cnt = 0;
	static uint8_t cnt = 0;
	if(AutoCharge.Position == POSITION_LEFT){
		cnt = 0;
		if(AutoCharge.CH1_IrDa_Dat[0] == TXDAT_LEFT && AutoCharge.CH3_IrDa_Dat[0] == TXDAT_LEFT){
			left_cnt = 0;
			AutoCharge.Move_Sta = CHG_MOVE_RIGHT;	
		}else if(AutoCharge.CH1_IrDa_Dat[0] == 0x0 && AutoCharge.CH3_IrDa_Dat[0] == TXDAT_LEFT){
			AutoCharge.Move_Sta = CHG_MOVE_RIGHT;			
		}else if(AutoCharge.CH_IrDate[0] == 0 && AutoCharge.CH_IrDate[2] != 0){
			AutoCharge.Move_Sta = CHG_MOVE_RIGHT;
    }
	 
	}else if(AutoCharge.Position == POSITION_RIGHT){
		left_cnt = 0;
		if(AutoCharge.CH_IrDate[3] == 0 && AutoCharge.CH_IrDate[5] == 0){
			if(AutoCharge.No_Sigh_Flag != 1){
				cnt ++;
				if(cnt >= 2){
				 cnt = 0;
					AutoCharge.Move_Sta=CHG_MOVE_RIGHT;
				}
			}
		}else if(AutoCharge.CH3_IrDa_Dat[0] == TXDAT_LEFT){
			AutoCharge.Move_Sta = CHG_MOVE_RIGHT;	
		}
	}else if(AutoCharge.Position == POSITION_MID){
		left_cnt = 0;
		cnt = 0;
		if(AutoCharge.CH1_IrDa_Dat[0] == TXDAT_LEFT && AutoCharge.CH3_IrDa_Dat[2] == TXDAT_RIGHT){
			AutoCharge.Move_Sta = CHG_MOVE_FORWARD;	
	  }else if(AutoCharge.CH1_IrDa_Dat[0] == 0x0 && AutoCharge.CH3_IrDa_Dat[0] == TXDAT_LEFT){
			AutoCharge.Move_Sta = CHG_MOVE_RIGHT;	
			}
		else if(AutoCharge.CH3_IrDa_Dat[2]==0x0 && AutoCharge.CH1_IrDa_Dat[2]==TXDAT_RIGHT){
			AutoCharge.Move_Sta = CHG_MOVE_LEFT;	
	  }
	}else{
		left_cnt = 0;
		if(AutoCharge.CH_IrDate[4] != 0 && AutoCharge.CH_IrDate[6] == 0){
		  AutoCharge.Last_Position = POSITION_LEFT;
			AutoCharge.Move_Sta = CHG_MOVE_ZERORIGHT;
	  }else if(AutoCharge.CH_IrDate[4] == 0 && AutoCharge.CH_IrDate[6] != 0){
		  AutoCharge.Last_Position = POSITION_RIGHT;
			AutoCharge.Move_Sta = CHG_MOVE_ZEROLEFT;
	  }else if(AutoCharge.CH_IrDate[4] != 0 && AutoCharge.CH_IrDate[6] != 0){
			AutoCharge.Last_Position = POSITION_LEFT;	
			AutoCharge.Move_Sta = CHG_MOVE_FORWARD;
		}
	}	
}

/*=============================================================================
*  函数名 ：chg_move_zeroright_process
*  作   者：hrx
*  创建时间：2022年4月11日 
*  修改时间：
*  输   入： 
*           
*  输   出：
*  说   明：自动充电原地右转时处理函数
*
==============================================================================*/	


void chg_move_zeroright_process(void)
{
  if(AutoCharge.Position == POSITION_LEFT){
		AutoCharge.Move_Sta = CHG_MOVE_RIGHT;
	}else if(AutoCharge.Position == POSITION_MID){
		AutoCharge.Move_Sta = CHG_MOVE_FORWARD;	
	}else if(AutoCharge.Position == POSITION_RIGHT){
		AutoCharge.Move_Sta = CHG_MOVE_LEFT;
	}else{
		if(AutoCharge.Last_Position == POSITION_RIGHT){
			if(AutoCharge.CH_IrDate[4] != 0 && AutoCharge.CH_IrDate[6] == 0){
				AutoCharge.Move_Sta = CHG_MOVE_ZERORIGHT;
	    }else if(AutoCharge.CH_IrDate[4] == 0 && AutoCharge.CH_IrDate[6] != 0){
		    AutoCharge.Last_Position = POSITION_RIGHT;
				AutoCharge.Move_Sta = CHG_MOVE_ZEROLEFT;
	    }else if(AutoCharge.CH_IrDate[5] != 0){
				AutoCharge.Last_Position = POSITION_RIGHT;
				AutoCharge.Move_Sta = CHG_MOVE_ZEROLEFT;
			}else if(AutoCharge.CH_IrDate[4] !=0 && AutoCharge.CH_IrDate[6] != 0){
				delay_ms(500);
				AutoCharge.Move_Sta = CHG_MOVE_FORWARD;
			}
		}else if(AutoCharge.Last_Position == POSITION_LEFT){
			if(AutoCharge.CH_IrDate[4] != 0 && AutoCharge.CH_IrDate[6] == 0){
		    AutoCharge.Last_Position = POSITION_LEFT;
				AutoCharge.Move_Sta = CHG_MOVE_ZERORIGHT;
	    }else if(AutoCharge.CH_IrDate[4] == 0 && AutoCharge.CH_IrDate[6] != 0){
				AutoCharge.Move_Sta = CHG_MOVE_ZEROLEFT;
	    }else {
				delay_ms(600);
				AutoCharge.Move_Sta = CHG_MOVE_FORWARD;
				//charge_IR_cnt = 0;
			}
		}else{
			if(AutoCharge.CH_IrDate[4] != 0 && AutoCharge.CH_IrDate[6] == 0){
		    AutoCharge.Last_Position = POSITION_LEFT;
	    }else if(AutoCharge.CH_IrDate[4] == 0 && AutoCharge.CH_IrDate[6] != 0){
		    AutoCharge.Last_Position = POSITION_RIGHT;
	    }
		}
	}
} 

/*=============================================================================
*  函数名 ：chg_move_zeroleft_process
*  作   者：hrx
*  创建时间：2022年4月11日 
*  修改时间：
*  输   入： 
*           
*  输   出：
*  说   明：自动充电原地左转时处理函数
*
==============================================================================*/	
void chg_move_zeroleft_process(void)
{
	if(AutoCharge.angle < -200){
		AutoCharge.Position = POSITION_NULL;             
		AutoCharge.Last_Position = POSITION_NULL;        
		AutoCharge.Move_Sta = CHG_MOVE_ZERORIGHT;
	  //printf("turn right");
	}else if(AutoCharge.Position == POSITION_LEFT){
		AutoCharge.Move_Sta = CHG_MOVE_RIGHT;
	}else if(AutoCharge.Position == POSITION_MID){
		AutoCharge.Move_Sta = CHG_MOVE_FORWARD;	
	}else if(AutoCharge.Position == POSITION_RIGHT){
		AutoCharge.Move_Sta = CHG_MOVE_LEFT;
	}else{
		if(AutoCharge.Last_Position == POSITION_RIGHT){
			if(AutoCharge.CH_IrDate[4] != 0 && AutoCharge.CH_IrDate[6] == 0){
				AutoCharge.Move_Sta = CHG_MOVE_ZERORIGHT;
	    }else if(AutoCharge.CH_IrDate[4] == 0 && AutoCharge.CH_IrDate[6] != 0){
				AutoCharge.Last_Position = POSITION_RIGHT;
				AutoCharge.Move_Sta = CHG_MOVE_ZEROLEFT;
			}else{
				delay_ms(500);
				AutoCharge.Move_Sta = CHG_MOVE_FORWARD;
			}
		}else if(AutoCharge.Last_Position == POSITION_LEFT){
			if(AutoCharge.CH_IrDate[4] != 0 && AutoCharge.CH_IrDate[6] == 0){
		    AutoCharge.Last_Position = POSITION_LEFT;
				AutoCharge.Move_Sta = CHG_MOVE_ZERORIGHT;
	    }else if(AutoCharge.CH_IrDate[4] == 0 && AutoCharge.CH_IrDate[6] != 0){
				AutoCharge.Move_Sta = CHG_MOVE_ZEROLEFT;
	    }else if(AutoCharge.CH_IrDate[3] != 0){	  
				AutoCharge.Last_Position = POSITION_LEFT;
				AutoCharge.Move_Sta = CHG_MOVE_ZERORIGHT;
			}else if(AutoCharge.CH_IrDate[4] != 0 && AutoCharge.CH_IrDate[6] != 0){
				delay_ms(500);
				AutoCharge.Move_Sta = CHG_MOVE_FORWARD;
			}
		}else{
			if(AutoCharge.CH_IrDate[4] != 0 && AutoCharge.CH_IrDate[6] == 0){
		    AutoCharge.Last_Position = POSITION_LEFT;
			}else if(AutoCharge.CH_IrDate[4] == 0 && AutoCharge.CH_IrDate[6] != 0){
		    AutoCharge.Last_Position = POSITION_RIGHT;	
	    }else if(AutoCharge.CH_IrDate[4] != 0 && AutoCharge.CH_IrDate[6] != 0){
				delay_ms(500);
				AutoCharge.Move_Sta = CHG_MOVE_FORWARD;
			}
		}
	}
	if(AutoCharge.Move_Sta != CHG_MOVE_ZEROLEFT){
		AutoCharge.OverTime = 0;
	}
}


/*=============================================================================
*  函数名 ：Charge_OverTime_Process
*  作   者：hrx
*  创建时间：2022年4月11日 
*  修改时间：
*  输   入： 
*           
*  输   出：
*  说   明：自动充电超时处理
*
==============================================================================*/	

void Charge_OverTime_Process(void)
{
	if(AutoCharge.OverTime > 20000){
	 AutoCharge.Last_Position = POSITION_NULL;
	}
}


/*=============================================================================
*  函数名 ：Charge_Crash_Pross
*  作   者：hrx
*  创建时间：2022年4月11日 
*  修改时间：
*  输   入： 
*           
*  输   出：
*  说   明：自动充电碰撞处理
*
==============================================================================*/	

void Charge_Crash_Pross(void)
{
	if((Robot_Moto.Locked_Flag & 0x03) != 0 || (Robot_Sys.Crash_Flag & 0x02) != 0){
		AutoCharge.Chg_Sta = 0;
	}
}


