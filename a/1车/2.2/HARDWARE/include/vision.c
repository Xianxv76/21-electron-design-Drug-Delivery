#include "vision.h"
#include "string.h"


 union float32 yaw_get;
 
void vision_init(void)
{
	//����1 a10 dma2 ͨ��4 ������2
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;		//dma�õ��ж�
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); //ʹ��GPIOAʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);;//ʹ��USART1ʱ��
 
	//����1��Ӧ���Ÿ���ӳ��
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2); //GPIOA9����ΪUSART1
	
	//USART1�˿�����
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_3;         
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;       
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;  
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;   
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;       
    GPIO_Init(GPIOA, &GPIO_InitStructure);  /* RXIO */  

  //USART1 ��ʼ������
	USART_DeInit(USART2);
	USART_InitStructure.USART_BaudRate = 115200;//����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//��У��
	USART_InitStructure.USART_Mode = USART_Mode_Rx;	//�շ�ģʽ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_Init(USART2, &USART_InitStructure); //��ʼ������1
	
	USART_Cmd(USART2, ENABLE);  //ʹ�ܴ���1
	USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);
	
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;//DMA2 ������2 �ж�ͨ��*******
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
}


u8 buff[20] ;
u8 inf = 0;
int buff_get = 0;
int buff_get2 = 0;

void USART2_IRQHandler(void)
{
	static int i = 0;
	if(USART_GetITStatus(USART2,USART_IT_RXNE)==SET)
	{
		inf = USART2->DR;
		if ( buff_get == 0 ){
						 if( inf == 0x55 )
						 {
							 buff[i] = inf;
							 i++;
							 buff_get = 1;
						 }}
		else
		{
						if( buff_get2 == 0 ){
											if( inf == 0x55 )
											{
												
												buff[i] = inf;
												i++;
												buff_get2 = 1;
											}
										else
											{  i = 0;
											   buff_get = 0;
											}
										
						}
						else
						{
										if( i < 9 )
										{
											buff[i] = inf;
											i++;
										}
										else if( i == 9 )
										{
											buff_get2 = 0;
											buff_get = 0;
											i = 0;
											yaw_get.data[0] = buff[2];
											yaw_get.data[1] = buff[3];   
											yaw_get.data[2] = buff[4];
											yaw_get.data[3] = buff[5];
								
										}
										else
										{
											buff_get2 = 0;
											buff_get = 0;
											i = 0;
										}
							}
		 }
	}
}

