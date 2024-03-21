#ifndef __KEY_H
#define __KEY_H	 

#include "sys.h" 
#include "delay.h" 


#define ROW2 		GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_10)  //PE4
#define ROW1 		GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_11)  //PE3 
#define ROW0 		GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_12)  //PE2


#define COLUMN0  PDout(5)
#define COLUMN1  PDout(6)
#define COLUMN2  PDout(7)


void KEY_Init(void);


#endif
