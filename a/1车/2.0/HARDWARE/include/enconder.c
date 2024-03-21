#include "enconder.h"

void Enconder_init(void)
{
	// forward direction positive direction
	GPIO_InitTypeDef gpio;
  direction_init();
	
/******************a��������ʼ��*******************************/
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

	gpio.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	gpio.GPIO_Speed = GPIO_Speed_100MHz;
	gpio.GPIO_Mode = GPIO_Mode_AF;
	gpio.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOA,&gpio);
	
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource6,GPIO_AF_TIM3);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource7,GPIO_AF_TIM3);
	TIM_EncoderInterfaceConfig(TIM3, TIM_EncoderMode_TI12, TIM_ICPolarity_Falling, TIM_ICPolarity_Falling);
	TIM_Cmd(TIM3,ENABLE);
	
/******************b��������ʼ��*******************************/
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	
	gpio.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	gpio.GPIO_Speed = GPIO_Speed_100MHz;
	gpio.GPIO_Mode = GPIO_Mode_AF;
	gpio.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOB,&gpio);
	
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource6,GPIO_AF_TIM4);
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource7,GPIO_AF_TIM4);
	TIM_EncoderInterfaceConfig(TIM4, TIM_EncoderMode_TI12, TIM_ICPolarity_Falling, TIM_ICPolarity_Falling);
	TIM_Cmd(TIM4,ENABLE);
	
}

//  in1 D3 in2 D4    in3 D2 in4 D1 
void direction_init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);	//ʹ��GPIOFʱ��

	//GPIOF9,F10��ʼ������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4 ;	//LED0��LED1��ӦIO��
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;			//��ͨ���ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;			//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;		//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;			//����
	GPIO_Init(GPIOD, &GPIO_InitStructure);					//��ʼ��GPIO
	GPIO_SetBits(GPIOD,GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4);			

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);	//ʹ��GPIOFʱ��
	
	
}

void right_forward(void)
{
    PDout(1) = 0;
		PDout(2) = 1;
}

void right_positive(void)
{
    PDout(1) = 1;
		PDout(2) = 0;
}

void left_forward(void)
{
  	PDout(3) = 1;
		PDout(4) = 0;	
}

void left_positive(void)
{

   	PDout(3) = 0;
		PDout(4) = 1;
}

