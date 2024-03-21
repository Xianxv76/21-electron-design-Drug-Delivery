#include "openmv.h"
#include "string.h"
#include "vision.h"

 
void open_init(void)
{
	//串口1 a10 dma2 通道4 数据流2
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;		//dma用到中断
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE); //使能GPIOA时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);;//使能USART1时钟
 
	//串口1对应引脚复用映射
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_USART3); //GPIOA9复用为USART1
	
	//USART1端口配置
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_11;         
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;       
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;  
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;   
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;       
    GPIO_Init(GPIOB, &GPIO_InitStructure);  /* RXIO */  

  //USART1 初始化设置
	USART_DeInit(USART3);
	USART_InitStructure.USART_BaudRate = 115200;//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无校验
	USART_InitStructure.USART_Mode = USART_Mode_Rx;	//收发模式
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_Init(USART3, &USART_InitStructure); //初始化串口1
	
	USART_Cmd(USART3, ENABLE);  //使能串口1
	USART_ITConfig(USART3,USART_IT_RXNE,ENABLE);
	
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;//DMA2 数据流2 中断通道*******
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
}

u8 angle[5] ;
int grt_flag = 0,grt_flag2 = 0;
union int16 set_yaw;
float set_yaw_angle = 0;
float turn_flag = 0;
float stop_flag = 0;
float aaa = 0;
void USART3_IRQHandler(void)
{	
	static u8 i = 0;
	static u8 open = 0;
	if(USART_GetITStatus(USART3,USART_IT_RXNE)==SET)
	{
		open = USART3->DR;
		if( grt_flag == 0 ){
						if( open == 0xA5 )
						{
							i = 0;
							grt_flag = 1;
							angle[i] = open;
							i++;
						}
						else
							i = 0;
		}
		else
		 {
			    if( grt_flag2 == 0 ){
								 if( open == 0xA5 ){
										 angle[i] = open ;
										 i++;
									   grt_flag2 = 1;
									  }
								 else
										{
											grt_flag = 0;
											grt_flag2 = 0 ;
											i = 0;
										}	
					  }
					else
						{
							if( i == 2 ){
							   angle[i] = open; 
							   i++;
							}
							else if ( i == 3 )
							{
							  angle[i] = open;
								i = 0;
								grt_flag = 0;
								grt_flag2 = 0 ;
								set_yaw.data[0] = angle[3];
								set_yaw.data[1] = angle[2];
								aaa = set_yaw.angle;
								if( set_yaw.angle == 400 )
								{
									set_yaw_angle = 0;
								   turn_flag = 1;
									stop_flag = 0;
								}
								else if( set_yaw.angle == 666 )
								{
									stop_flag = 1;
									set_yaw_angle = 0;
								}
								else
								{
								  set_yaw_angle = (set_yaw.angle / 20.0f) -8.0f;
									turn_flag = 0;
									stop_flag = 0;
								}
							}
							else
							{
								i = 0;
								grt_flag = 0;
								grt_flag2 = 0 ;
							}
						}
		 }	
	}
}
