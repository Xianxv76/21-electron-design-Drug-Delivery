#include "spi.h"
//////////////////////////////////////////////////////////////////////////////////
// ������ֻ��ѧϰʹ�ã�δ���������ɣ��������������κ���;
// ALIENTEK STM32F407������
// SPI ��������
// ����ԭ��@ALIENTEK
// ������̳:www.openedv.com
// ��������:2014/5/6
// �汾��V1.0
// ��Ȩ���У�����ؾ���
// Copyright(C) �������������ӿƼ����޹�˾ 2014-2024
// All rights reserved
//////////////////////////////////////////////////////////////////////////////////

// ������SPIģ��ĳ�ʼ�����룬���ó�����ģʽ
// SPI�ڳ�ʼ��
// �������Ƕ�SPI2�ĳ�ʼ��
// PB13 SCK
// PC2 MISO
// PC3 MOSI
// PG6 CE
// PG7 CS
// PG8 IRE
void SPI2_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	SPI_InitTypeDef SPI_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); // ʹ��GPIOAʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE); // ʹ��GPIOAʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);  // ʹ��SPI1ʱ��

	// GPIOC2,3��ʼ������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_2; // PC2~3���ù������
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;		   // ���ù���
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		   // �������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	   // 100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;		   // ����
	GPIO_Init(GPIOC, &GPIO_InitStructure);				   // ��ʼ��
										   // GPIOB13��ʼ������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;		   // PB13���ù������
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;	   // ���ù���
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	   // �������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; // 100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;	   // ����
	GPIO_Init(GPIOB, &GPIO_InitStructure);			   // ��ʼ��

	GPIO_PinAFConfig(GPIOC, GPIO_PinSource2, GPIO_AF_SPI2);	 // PC2����Ϊ SPI2
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource3, GPIO_AF_SPI2);	 // PC3����Ϊ SPI2
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource13, GPIO_AF_SPI2); // PB12����Ϊ SPI2

	// ����ֻ���SPI�ڳ�ʼ��
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_SPI2, ENABLE);  // ��λSPI2
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_SPI2, DISABLE); // ֹͣ��λSPI2

	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;	 // ����SPI�������˫�������ģʽ:SPI����Ϊ˫��˫��ȫ˫��
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;						 // ����SPI����ģʽ:����Ϊ��SPI
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;					 // ����SPI�����ݴ�С:SPI���ͽ���8λ֡�ṹ
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;							 // ����ͬ��ʱ�ӵĿ���״̬Ϊ�ߵ�ƽ
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;						 // ����ͬ��ʱ�ӵĵڶ��������أ��������½������ݱ�����
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;							 // NSS�ź���Ӳ����NSS�ܽţ�����������ʹ��SSIλ������:�ڲ�NSS�ź���SSIλ����
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256; // ���岨����Ԥ��Ƶ��ֵ:������Ԥ��ƵֵΪ256
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;					 // ָ�����ݴ����MSBλ����LSBλ��ʼ:���ݴ����MSBλ��ʼ
	SPI_InitStructure.SPI_CRCPolynomial = 7;							 // CRCֵ����Ķ���ʽ
	SPI_Init(SPI2, &SPI_InitStructure);									 // ����SPI_InitStruct��ָ���Ĳ�����ʼ������SPIx�Ĵ���

	SPI_Cmd(SPI2, ENABLE); // ʹ��SPI����

	SPI2_ReadWriteByte(0xff); // ��������
}
// SPI2�ٶ����ú���
// SPI�ٶ�=fAPB1/��Ƶϵ��
//@ref SPI_BaudRate_Prescaler:SPI_BaudRatePrescaler_2~SPI_BaudRatePrescaler_256
// fAPB1ʱ��һ��Ϊ42Mhz��

void SPI2_SetSpeed(u8 SPI_BaudRatePrescaler)
{
	assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler)); // �ж���Ч��
	SPI2->CR1 &= 0XFFC7;											// λ3-5���㣬�������ò�����
	SPI2->CR1 |= SPI_BaudRatePrescaler;								// ����SPI2�ٶ�
	SPI_Cmd(SPI2, ENABLE);											// ʹ��SPI2
}
// SPI2 ��дһ���ֽ�
// TxData:Ҫд����ֽ�
// ����ֵ:��ȡ�����ֽ�
u8 SPI2_ReadWriteByte(u8 TxData)
{

	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET)
	{
	} // �ȴ���������

	SPI_I2S_SendData(SPI2, TxData); // ͨ������SPIx����һ��byte  ����

	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET)
	{
	} // �ȴ�������һ��byte

	return SPI_I2S_ReceiveData(SPI2); // ����ͨ��SPIx������յ�����
}
