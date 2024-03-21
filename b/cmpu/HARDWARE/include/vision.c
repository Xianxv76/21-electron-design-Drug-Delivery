#include "vision.h"
#include "string.h"

 
void vision_init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG,ENABLE); //使能GPIOA时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6, ENABLE);;//使能USART1时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE); //DMA2时钟使能 ---********************
 
	//串口1对应引脚复用映射
	GPIO_PinAFConfig(GPIOG, GPIO_PinSource9, GPIO_AF_USART6); //GPIOA9复用为USART1
	GPIO_PinAFConfig(GPIOG, GPIO_PinSource14, GPIO_AF_USART6); //GPIOA10复用为USART1
	
	//USART1端口配置
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_9;         
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;       
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;  
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;   
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;       
    GPIO_Init(GPIOG, &GPIO_InitStructure);  /* TXIO */  

    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_14;                
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
	  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;  
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;   
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;       
    GPIO_Init(GPIOG, &GPIO_InitStructure);  /* RXIO */

  //USART1 初始化设置
	USART_DeInit(USART6);
	USART_InitStructure.USART_BaudRate = 115200;//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无校验
	USART_InitStructure.USART_Mode =   USART_Mode_Tx;	//收发模式
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_Init(USART6, &USART_InitStructure); //初始化串口1
	
	USART_Cmd(USART6, ENABLE);  //使能串口1
	

}
