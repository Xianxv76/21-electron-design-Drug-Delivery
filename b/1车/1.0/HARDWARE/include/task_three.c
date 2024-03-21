#include "task_three.h"
#include "task_two.h"
#include "led.h"
#include "k210.h"

 task two;
extern button button_key;
extern motor_date_ty right_motor;
extern motor_date_ty left_motor; 
extern u8 math_buff[5];
extern float right_set,left_set ;
extern float turn_flag ;
extern float yaw_error ;
extern union float32 yaw_get;
extern float set_yaw_angle ;
extern float stop_flag ;

void task_three(void)
{
	  two.road_act[0] = 0;
	  two.road_act[1] = 1200;
	  two.road_act[2] = 1000;
	  two.road_act[3] = 900;
	
		//two.road_act[4] = 1150;
		two.road_act[5] = 1200;
	  two.road_act[5] = 1200;
	  two.road_act[6] = 1000;
	
  if( math_buff[2] == 9 )
	{
		two.math_get = math_buff[3];  //������һ��ʶ�𵽵�����
		math_buff[2] = 10;          //��ձ�־λ
		math_buff[3] = 10;          //��ձ�־λ
		
	}
	
  if( button_key.flag != 0 && button_key.get == 1 )   //��������û��̧�� ǰ����ָ��ҩ��    
	  xunxian_three(0);
	else if(  button_key.flag !=0 && button_key.get == 0 ) //����������̧�𣬷�����ʼ��
	{
		LED_red = 0;
		if( two.back_one == 0 )
						{
							two.back_time = 0;
							stop_flag = 0;
							yaw_error = yaw_get.angle + 185;
							two.wait_time_stop = 0;
						  two.back_one = 1;//�Ѿ�ת���־λ
							if( two.turn_times > 5 )
							{
								two.right_left[4] = two.right_left[6];
								two.right_left[3] = 1;
							}
							two.turn_times = 5;
							
						}
					else
					{
						two.back_time++;
					 	if( two.back_time > 130 ) 
						{
							xunxian_three(1);      ///////����1;
						}
						else
						{
							right_set = 0;
							left_set = 0;
						}
					}
	}
	else    //û�а��£���ǰ��
	{
		right_set = 0;
		left_set = 0;
	}
}



int aa = 0;

void xunxian_three(int mod)
{
	if( stop_flag == 0)  //û�н��յ�ֹͣ�ź�
	{
	 if( two.arrive_flag == 0 )
		{
		  if( turn_flag == 0 )
			{
				USART_Cmd(USART3, ENABLE);
				right_set = speed_set;
				left_set = speed_set;    //ǰ��
			}
			else  
			{
				if( mod == 0 )
			     two.turn_times ++;   //���������
				else 
					 two.turn_times --;
				
				two.arrive_flag = 1; 
				two.ech_i++;       //���������
				two.ech_hold[two.turn_times] = ( right_motor.raw_value + left_motor.raw_value )/2 ; //������ֵ  
			}
			yaw_error =  yaw_get.angle - set_yaw_angle;  
		}
		else//( two.arrive_flag == 0 )
		{
			if( two.turn_times <= 2 || two.turn_times == 5 )
			{
				 USART_Cmd(USART3, DISABLE);
				 right_set = speed_set;left_set = speed_set;
  			 turn_flag = 0; 
				 if(( (( right_motor.raw_value + left_motor.raw_value )/2) - two.ech_hold[two.turn_times] ) > two.road_act[two.turn_times] )  //��һ��·�ڲ�ͣ������openmv
				 {
					 USART_Cmd(USART3, ENABLE);
					 two.arrive_flag = 0;
				 }
			}
			else if( two.turn_times == 3 )
			{
				 USART_Cmd(USART3, DISABLE);
				 right_set = speed_set;left_set = speed_set;
  			 turn_flag = 0; 
				 if(( (( right_motor.raw_value + left_motor.raw_value )/2) - two.ech_hold[two.turn_times] ) < two.road_act[two.turn_times] )  //�ڶ���·�ڣ�����openmv
				 {
					 right_set = speed_set;
					 left_set = speed_set;
					 USART_Cmd(USART3, DISABLE);
	      	 turn_flag = 0;	 
				 }
				 else//(( (( right_motor.raw_value + left_motor.raw_value )/2) - two.ech_hold[two.ech_i] ) < two.road_act[two.ech_i] )
				 {
				   right_set = 0;left_set = 0; 
								if( two.set_one == 0 )
								{
											if( mod == 0 )
											 {	right_set = 0;left_set = 0;
												 if( two.math_get == math_buff[2] )
													{
														yaw_error = yaw_get.angle + 80;  //��ת
														two.right_left[two.turn_times] = 1 ; //
													}
													else 
													{
														yaw_error = yaw_get.angle - 80;  //��ת
														two.right_left[two.turn_times] = 2 ; 
													}	
													  two.set_one = 1;
														math_buff[2] = 10;
														math_buff[3] = 10;
											}
									 	else
											{
												right_set = 0;left_set = 0;
												if( two.right_left[two.turn_times] ==  1 )
												{
													yaw_error = yaw_get.angle - 80;  //��ת
													two.set_one = 1;
												}
												else
												{
													yaw_error = yaw_get.angle + 80;  //��ת
													two.set_one = 1;
												}
											}
								}
								else
								{
									 right_set = 0;
									 left_set = 0;
									 two.wait_time_turn++;
											if( two.wait_time_turn > 70 )
											{
												two.arrive_flag = 0;
												USART_Cmd(USART3, ENABLE);
												two.wait_time_turn = 0;
												two.set_one = 0;
											}
								}
			     }
			}
			else if( two.turn_times == 4 || two.turn_times == 6 )
			{
				// USART_Cmd(USART3, DISABLE);
				 right_set = speed_set;left_set = speed_set;
  			 turn_flag = 0; 
								if( mod ==  1 )
									two.road_act[two.turn_times] = 1000;
								else
									two.road_act[two.turn_times] = 1200;
							
				 if(( (( right_motor.raw_value + left_motor.raw_value )/2) - two.ech_hold[two.turn_times] ) < two.road_act[two.turn_times] )  //�ڶ���·�ڣ�����openmv
				 {
					 right_set = speed_set;
					 left_set = speed_set;
					 USART_Cmd(USART3, DISABLE);
	      	 turn_flag = 0;	 
				 }
				 else//(( (( right_motor.raw_value + left_motor.raw_value )/2) - two.ech_hold[two.ech_i] ) < two.road_act[two.ech_i] )
				 {
				   right_set = 0;left_set = 0; 
								if( two.set_one == 0 )
								{
											if( mod == 0 )
											 {	
												 if( two.math_get == math_buff[2] )
													{
														right_set = 0;left_set = 0;
														yaw_error = yaw_get.angle + 90;  //��ת
														two.right_left[two.turn_times] = 1;
														two.turn_direc = 1;
														two.turn_time_set = 70;
														two.set_one = 1;
														math_buff[2] = 10;
														math_buff[3] = 10;
													}
													else if( two.math_get == math_buff[3] )
													{
														right_set = 0;left_set = 0;
														yaw_error = yaw_get.angle - 90;  //��ת
														two.right_left[two.turn_times] = 2;
														two.turn_direc = 0;
														two.turn_time_set = 70;
														two.set_one = 1;
														math_buff[2] = 10;
														math_buff[3] = 10;
													}
													else
													{
														right_set = 0;left_set = 0;
														yaw_error = yaw_get.angle - 180;  //��ת
														two.turn_time_set = 120;
														two.set_one = 1;
														math_buff[2] = 10;
														math_buff[3] = 10;
													}
											}
									 	else
											{
												if( two.right_left[two.turn_times] == 1 )
												{
													right_set = 0;left_set = 0;
													yaw_error = yaw_get.angle - 80;  //��ת
													two.turn_time_set = 70;
													two.set_one = 1;
												}
												else
												{
													right_set = 0;left_set = 0;
													yaw_error = yaw_get.angle + 80;  //��ת
													two.turn_time_set = 70;
													two.set_one = 1;
												}
											}
								}
								else
								{
									 right_set = 0;
									 left_set = 0;
									 two.wait_time_turn++;
											if( two.wait_time_turn > two.turn_time_set )
											{
												two.arrive_flag = 0;
												USART_Cmd(USART3, ENABLE);
												two.wait_time_turn = 0;
												two.set_one = 0;
											}
								}
			     }
			}
		}
	}
	else
  {
		if( mod == 0 )
		{
			if( two.turn_times > 3 )
			{
				USART_Cmd(USART3, DISABLE);
					two.arrive_flag = 0;
					stop_flag = 1;
					two.wait_time_stop++;
						if( two.wait_time_stop > 30 )
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
        else{		
							right_set = speed_set;
							left_set = speed_set;
						}
    }
		else
		{
			if( two.turn_times < 2 )
			{
				USART_Cmd(USART3, DISABLE);
					two.arrive_flag = 0;
					stop_flag = 1;
					two.wait_time_stop++;
						if( two.wait_time_stop > 30 )
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
        else{		
							right_set = speed_set;
							left_set = speed_set;
						}
		}
			
  }
}		
		
		