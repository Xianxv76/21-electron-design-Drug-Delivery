#ifndef _TIMER_H
#define _TIMER_H

#include "sys.h"
#include "FreeRTOS.h"
#include "task.h"


typedef struct
{
  u16 get;  		//实现归零的标志位
	u16 io;  		  //io口电平
	u16 time; 		//高电平次数
	u16 flag;	 	  //触发标志位
	
}button;  //按键信息

void ENC_Init(void);
void button_task(void *pvParameters);
 void colrest(void);
 void col3(void);
 void col2(void);
 void col1(void);
 void colall(void);
 
 
 

#endif



