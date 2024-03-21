#ifndef __SPI_H
#define __SPI_H
#include "sys.h"

//PB13 SCK
//PC2 MISO
//PC3 MOSI
//PG6 CE
//PG7 CS
//PG8 IRE 	    													  
void SPI2_Init(void);			 //初始化SPI1口
void SPI2_SetSpeed(u8 SpeedSet); //设置SPI1速度   
u8 SPI2_ReadWriteByte(u8 TxData);//SPI1总线读写一个字节
		 
#endif

