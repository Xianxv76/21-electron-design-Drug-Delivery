#ifndef __SPI_H
#define __SPI_H
#include "sys.h"

//PB13 SCK
//PC2 MISO
//PC3 MOSI
//PG6 CE
//PG7 CS
//PG8 IRE 	    													  
void SPI2_Init(void);			 //��ʼ��SPI1��
void SPI2_SetSpeed(u8 SpeedSet); //����SPI1�ٶ�   
u8 SPI2_ReadWriteByte(u8 TxData);//SPI1���߶�дһ���ֽ�
		 
#endif

