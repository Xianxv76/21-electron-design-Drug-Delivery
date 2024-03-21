#ifndef __ENCODER_TASK_H_
#define __ENCODER_TASK_H_

#include "sys.h"
#include "FreeRTOS.h"
#include "task.h"
#include "rm_pid.h"
#include "enconder.h"
#include "led.h"
#include "task_car.h"
#include "task_two.h"
#include "task_three.h"


#define RATE_BUF_SIZE 6
#define speed_set 1500

typedef struct{
	int32_t raw_value;   					 //编码器不经处理的原始值
	int32_t last_raw_value;					 //上一次的编码器原始值
	float ecd_value;                       //经过处理后连续的编码器值
	int32_t diff;							 //两次编码器之间的差值
	int32_t temp_count;                      //计数用
	uint8_t buf_count;						 //滤波更新buf用
	int32_t ecd_raw_rate;					 //通过编码器计算得到的速度原始值
	int32_t rate_buf[RATE_BUF_SIZE];	     //buf，for filter
	int32_t round_cnt;						 //圈数
	int32_t ecd_rate;					 //速度

		
}motor_date_ty;

void encoder_task(void *pvParameters);
void moto_speed_get(motor_date_ty*right_motor,int cnt);
void speed_send(void);


#endif
