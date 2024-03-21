#include "key.h"
#include "delay.h" 


void KEY_Init(void)
{
	
	GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC|RCC_AHB1Periph_GPIOD, ENABLE);//使能GPIOA,GPIOE时钟

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13; //KEY0 KEY1 KEY2对应引脚
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;   //普通输入模式
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;  //100M
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;    //下拉
  GPIO_Init(GPIOC, &GPIO_InitStructure);//初始化GPIOE2,3,4

  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7| GPIO_Pin_5 ;//LED0和LED1对应IO口
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//开漏输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
  GPIO_Init(GPIOD, &GPIO_InitStructure);//初始化GPIO
	
	GPIO_SetBits(GPIOD,GPIO_Pin_6 | GPIO_Pin_7| GPIO_Pin_5 );//GPIOF9,F10设置low

 
} 
















