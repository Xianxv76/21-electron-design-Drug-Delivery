#ifndef __LED_H
#define __LED_H
#include "sys.h"



//LED�˿ڶ���
#define LED_red PDout(9)	// DS0
#define LED_green PDout(10)	// DS1	 
#define LED_blue PDout(11)	// DS1	 

void LED_Init(void);//��ʼ��		 				    
#endif
