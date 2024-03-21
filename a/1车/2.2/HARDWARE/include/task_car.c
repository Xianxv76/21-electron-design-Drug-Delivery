#include "task_car.h"
#include "led.h"
#include "k210.h"

static int t = 0;
extern button button_key;
extern motor_date_ty right_motor;
extern motor_date_ty left_motor; 
extern float stop_flag ;
extern float turn_flag ;//识别到路口标志位
static int turn_arrive = 0;   //到达路口标志位      
static int turn_direc = 1;    //转方向选哪个标志位
static int set_one = 0; //转弯时设置一次角度标志位
extern float right_pwm ;
extern float left_pwm ;
extern float right_set ,left_set ;
static float turn_ecd_hold = 0;
extern union float32 yaw_get;
extern float set_yaw_angle ;
extern float yaw_error ;
extern u8 math_buff[5];
static int back_one = 0;
static int time_back = 0;
static float road_act = 0;

void task_one(void)
{
	if( math_buff[2] == 9 )
	{
		  if( math_buff[3] == 1 )
				 turn_direc = 1;
			else
				 turn_direc = 0;
	}

			if( button_key.flag != 0 && button_key.get == 1 )   //触发，但没有抬起
			{ 
			   xunxian_one(0);
		  }
			else if(  button_key.flag !=0 && button_key.get == 0 )
			{
					if( back_one == 0 )
						{
							t = 0;
							stop_flag = 0;
							yaw_error = yaw_get.angle + 175;
						  back_one = 1;//已经转向标志位
						}
					else
					{
						time_back++;
					 	if( time_back > 100 ) 
						{
							xunxian_one(1);
						}
						else
						{
							right_set = 0;
							left_set = 0;
						}
					}
			}
			else
			{
					right_set = 0;
					left_set = 0;
			}
  
}



void  xunxian_one(int mod)
{
	if( mod ==1 )
	  	LED_red = 0;
	if( stop_flag == 0 )  //停止标志位
		{
			 if( turn_arrive == 0 )   //没有到十字路口
				{			
							if( turn_flag == 0 )
							{
								t = 0 ;
								USART_Cmd(USART3, ENABLE);
								right_set = speed_set;
								left_set = speed_set;
							}
							else
							{
							//	t = 0 ;
								turn_arrive = 1;
							//	right_set = 0;left_set = 0;
								turn_ecd_hold = ( right_motor.raw_value + left_motor.raw_value )/2 ;  //保存此时编码器值
							}
							yaw_error =  yaw_get.angle - set_yaw_angle;     //转向闭环来自视觉
				}
				else
				{   
					if ( mod == 0 )
						road_act = 1500;
					else
					{
						if ( turn_direc == 1 )
						  road_act = 1500;
						else
						  road_act = 1000;
					}
						 if( ( (( right_motor.raw_value + left_motor.raw_value )/2) - turn_ecd_hold ) < road_act )
							 {
								 right_set = speed_set;
								 left_set = speed_set;
								 USART_Cmd(USART3, DISABLE);
								 turn_flag = 0;
							 }	
							else 
							{
								
								if( set_one == 0 )
											{
													if( mod == 0 )
														{
															if( turn_direc == 0 )
															yaw_error = yaw_get.angle + 90;
															else
															yaw_error = yaw_get.angle - 90;
														}
													else
														{
															if( turn_direc == 0 )
															yaw_error = yaw_get.angle - 85;
															else
															yaw_error = yaw_get.angle + 85;
														}	
											
											 set_one = 1;//已经转向标志位
											}
								 else
											{
											 right_set = 0;//USART_Cmd(USART3, DISABLE);
											 left_set = 0;
											 t++;
													if( t > 80 )
													{
														turn_arrive = 0;
														USART_Cmd(USART3, ENABLE);
														t = 0;
														set_one = 0;
													}
											} 										
							 }
				 }
		}	
		else
		{ 
						USART_Cmd(USART3, DISABLE);
						stop_flag = 1;
						t++;
							if( t > 30 )
							{													
								right_set = 0;
								left_set = 0;
								if( mod == 0) 
								LED_red = 1;
								else
								LED_green = 1;
								yaw_error = yaw_get.angle;
							}
							else{		
								right_set = speed_set;
								left_set = speed_set;
							}
		}

}


