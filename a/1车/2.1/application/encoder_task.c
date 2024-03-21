#include "encoder_task.h"
#include "vision.h"

motor_date_ty right_motor;
motor_date_ty left_motor; 
PID pid_right,pid_left,pid_turn;

float out_left = 0,out_right = 0,right_set = 0,left_set = 0;
extern union float32 yaw_get;
extern float set_yaw_angle ;
float turn_out = 0;//转向输出
float right_pwm = 0;
float left_pwm = 0;
float yaw_error = 0;
extern u32 i[16];


void encoder_task(void *pvParameters)
{
	pid_init(&pid_right, 2.5, 0.1, 2, 1200, 500, 700, 2000);
	pid_init(&pid_left, 2.5, 0.1, 2, 1200, 500, 700, 2000);
	pid_init(&pid_turn, 35, 0, 60, 1600, 100, 700, 2000);
	vTaskDelay(500);vTaskDelay(500);
	while(1)
	{
   ////////     速度值解算    //////////
		if( i[0] == 1 )
		task_one();
		else if ( i[1] == 1 )
	  task_twe();
		else
		{
			right_set = 0;left_set = 0;
		}
    speed_send();
	  vTaskDelay(10);
  }
}



void speed_send(void)
{   
	  turn_out =  Calculate_Current_Value( &pid_turn,yaw_get.angle,yaw_error);  
	
	  out_right = Calculate_Current_Value( &pid_right,( right_set-turn_out),( right_motor.ecd_rate ));             // ));//right_set +turn_out ) )  ;
	  out_left =  Calculate_Current_Value( &pid_left,( left_set+turn_out),( left_motor.ecd_rate ));                  // ) ) ;
	
	if( out_left > 0 ){
   left_forward();
		left_pwm = out_left;
	}
	else{
	 left_positive();
	  left_pwm = -out_left;
	}
	
	if( out_right > 0 ){
		right_forward();
		right_pwm = out_right;
	}
	else{
	  right_positive();
	  right_pwm = -out_right;
	}
	//正反转
	   TIM_SetCompare1(TIM8,right_pwm); 
	   TIM_SetCompare3(TIM8,left_pwm); //left
}




void moto_speed_get(motor_date_ty*right_motor,int cnt)
{
	int i = 0;
	 right_motor->last_raw_value = right_motor->raw_value;   //保留上一次的值
	 right_motor->raw_value = cnt;   
	 right_motor->diff = right_motor->raw_value - right_motor->last_raw_value;  //得到差值
		if(right_motor->diff < -32767)    //两次编码器的反馈值差别太大，表示圈数发生了改变
			{
				right_motor->round_cnt++;
				right_motor->ecd_raw_rate =right_motor->diff + 65535;
			}
			else if(right_motor->diff > 32767)
			{
				right_motor->round_cnt--;
				right_motor->ecd_raw_rate = right_motor->diff - 65535;
			}		
			else
			{
				right_motor->ecd_raw_rate = right_motor->diff;
			}		
			right_motor->ecd_value = ( right_motor->raw_value + right_motor->round_cnt * 65535.f ) *360.f/4.0f/990.f;   //角度值
			
	right_motor->rate_buf[right_motor->buf_count++] = right_motor->ecd_raw_rate * 360 * 1000/3960.f ;  ///3960.f;
			
	if(right_motor->buf_count == RATE_BUF_SIZE)
	{
		right_motor->buf_count = 0;
	}
	//计算速度平均值
	for(i = 0;i < RATE_BUF_SIZE; i++)
	{
		right_motor->temp_count += right_motor->rate_buf[i];
	}
	right_motor->ecd_rate = (int32_t)(right_motor->temp_count/RATE_BUF_SIZE);			
	right_motor->temp_count = 0;
	
}



