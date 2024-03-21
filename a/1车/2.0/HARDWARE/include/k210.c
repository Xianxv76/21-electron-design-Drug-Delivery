#include "k210.h"
#include "vision.h"

 
void k210_init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;		//dma�õ��ж�
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE); //ʹ��GPIOCʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6, ENABLE);;//ʹ��USART6ʱ��
 
	//����1��Ӧ���Ÿ���ӳ��
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource7, GPIO_AF_USART6); //GPIOA9����ΪUSART1
	
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_7;         
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;       
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;  
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;   
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;       
    GPIO_Init(GPIOC, &GPIO_InitStructure); 

  //USART1 ��ʼ������
	USART_DeInit(USART6);
	USART_InitStructure.USART_BaudRate = 115200;//����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//��У��
	USART_InitStructure.USART_Mode = USART_Mode_Rx ;	//�շ�ģʽ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_Init(USART6, &USART_InitStructure); //��ʼ������1
	
	USART_Cmd(USART6, ENABLE);  //ʹ�ܴ���1
	USART_ITConfig(USART6,USART_IT_RXNE,ENABLE);
	
	NVIC_InitStructure.NVIC_IRQChannel = USART6_IRQn;//DMA1 ������5 �ж�ͨ��*******
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
	
}
int aa = 0;
u8 math_buff[5];
extern int turn_direc ; 

void USART6_IRQHandler(void)
{
	
	static int i = 0;
	static u8 buff = 0;
	static int grt_flag = 0,grt_flag2 = 0;
	aa++;
	if(USART_GetITStatus(USART6,USART_IT_RXNE)==SET)
	{
		buff = USART6->DR;
		if( grt_flag == 0 ){
						if( buff == 0xA5 )
						{
							i = 0;
							grt_flag = 1;
							math_buff[i] = buff;
							i++;
						}
						else
							i = 0;
		}
		else
		 {
			    if( grt_flag2 == 0 ){
								 if( buff == 0xA5 ){
										 math_buff[i] = buff ;
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
							   math_buff[i] = buff; 
							   i++;
							}
							else if ( i == 3 )
							{
							  math_buff[i] = buff;
								i = 0;
								grt_flag = 0;
								grt_flag2 = 0 ;
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





