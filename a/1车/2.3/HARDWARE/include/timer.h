#ifndef _TIMER_H
#define _TIMER_H

#include "sys.h"
#include "FreeRTOS.h"
#include "task.h"


typedef struct
{
  u16 get;  		//ʵ�ֹ���ı�־λ
	u16 io;  		  //io�ڵ�ƽ
	u16 time; 		//�ߵ�ƽ����
	u16 flag;	 	  //������־λ
	
}button;  //������Ϣ

void ENC_Init(void);
void button_task(void *pvParameters);
 void colrest(void);
 void col3(void);
 void col2(void);
 void col1(void);
 void colall(void);
 
 
 

#endif



