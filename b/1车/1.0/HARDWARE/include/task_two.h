#ifndef __TASK_TWO_H
#define __TASK_TWO_H

#include "sys.h"
#include "encoder_task.h"
#include "vision.h"
#include "led.h"
#include "timer.h"


typedef struct
{
	int math_get;
  u16 turn_direc;  	  //旋转方向	   
	u16 arrive_flag; 		//到达转向位置标志位 
	u16 turn_times;	 	  //路口次数
	u16 ech_hold[20];       //编码器保存值
	u16 ech_i;
	u16 set_one;
	u16 road_act[20];
	u16 wait_time_turn;
	u16 wait_time_stop;
	u16 back_one;
	u16 back_time;
	u16 stop_time;
	u16 right_left[20];
	u16 turn_time_set;
	
	
}task;  //按键信息


void task_twe(void);
void xunxian_two(int mod);


#endif

