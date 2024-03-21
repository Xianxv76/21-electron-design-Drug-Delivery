#include "sys.h"
#include "delay.h"
#include "enconder.h"
#include "led.h"
#include "pwm.h"
#include "FreeRTOS.h"
#include "task.h"
#include "vision.h"
#include "encoder_task.h"
#include "openmv.h"
#include "spi.h"
#include "24l01.h"
#include "k210.h"
#include "key.h"

//�������ȼ�
#define START_TASK_PRIO		1
//�����ջ��С	
#define START_STK_SIZE 		128  
//������
TaskHandle_t StartTask_Handler;
//������
void start_task(void *pvParameters);

//�������ȼ�
#define ENCODER_TASK_PRIO		2
//�����ջ��С	
#define ENCODER_STK_SIZE 		128  
//������
TaskHandle_t ENCODERTask_Handler;
//������
void encoder_task(void *pvParameters);

//�������ȼ�
#define LED1_TASK_PRIO		2
//�����ջ��С	
#define LED1_STK_SIZE 		128  
//������
TaskHandle_t LED1Task_Handler;
//������
void led1_task(void *pvParameters);

//�������ȼ�
#define LED2_TASK_PRIO		2
//�����ջ��С	
#define LED2_STK_SIZE 		128  
//������
TaskHandle_t LED2Task_Handler;
//������
void led2_task(void *pvParameters);


//�������ȼ�
#define BUTTON_TASK_PRIO		2
//�����ջ��С	
#define BUTTON_STK_SIZE 		128  
//������
TaskHandle_t BUTTONTask_Handler;
//������
void button_task(void *pvParameters);

int check_ok = 0;

int main(void)
{ 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);//����ϵͳ�ж����ȼ�����4
	delay_init(168);		//��ʼ����ʱ����
	LED_Init();		        //��ʼ��LED�˿�
	Enconder_init();
	TIM8_PWM_Init(1999,167);
	vision_init();
	NRF24L01_Init(); 
	LED_Init();
	open_init();
	KEY_Init();
	k210_init( );
		while(NRF24L01_Check()){};
     check_ok = 1;
		 NRF24L01_RX_Mode();	
	    delay_ms(500);
			delay_ms(500);
	//������ʼ����
    xTaskCreate((TaskFunction_t )start_task,            //������
                (const char*    )"start_task",          //��������
                (uint16_t       )START_STK_SIZE,        //�����ջ��С
                (void*          )NULL,                  //���ݸ��������Ĳ���
                (UBaseType_t    )START_TASK_PRIO,       //�������ȼ�
                (TaskHandle_t*  )&StartTask_Handler);   //������              
    vTaskStartScheduler();          //�����������
}


 
//��ʼ����������
void start_task(void *pvParameters)
{
    taskENTER_CRITICAL();           //�����ٽ���
    //����encoder����
    xTaskCreate((TaskFunction_t )encoder_task,     
                (const char*    )"encoder_task",   
                (uint16_t       )ENCODER_STK_SIZE, 
                (void*          )NULL,
                (UBaseType_t    )ENCODER_TASK_PRIO,
                (TaskHandle_t*  )&ENCODERTask_Handler); 
    //����LED1����
    xTaskCreate((TaskFunction_t )led1_task,     
                (const char*    )"led1_task",   
                (uint16_t       )LED1_STK_SIZE, 
                (void*          )NULL,
                (UBaseType_t    )LED1_TASK_PRIO,
                (TaskHandle_t*  )&LED1Task_Handler); 

    //����LED2����
    xTaskCreate((TaskFunction_t )led2_task,     
                (const char*    )"led2_task",   
                (uint16_t       )LED2_STK_SIZE, 
                (void*          )NULL,
                (UBaseType_t    )LED2_TASK_PRIO,
                (TaskHandle_t*  )&LED2Task_Handler); 
								
    //����LED2����
    xTaskCreate((TaskFunction_t )button_task,     
                (const char*    )"button_task",   
                (uint16_t       )BUTTON_STK_SIZE, 
                (void*          )NULL,
                (UBaseType_t    )BUTTON_TASK_PRIO,
                (TaskHandle_t*  )&BUTTONTask_Handler); 								
  
    vTaskDelete(StartTask_Handler); //ɾ����ʼ����
    taskEXIT_CRITICAL();            //�˳��ٽ���
}

extern motor_date_ty right_motor;
extern motor_date_ty left_motor;

int rx_check = 0;
u8 tmp_buf[33];
int flag_one = 0;  //

	
//LED1������
void led1_task(void *pvParameters)
{
    while(1)
    {
			 moto_speed_get(&right_motor,TIM3->CNT);
	     moto_speed_get(&left_motor,TIM4->CNT);
			
			rx_check = NRF24L01_RxPacket(tmp_buf);
			if(NRF24L01_RxPacket(tmp_buf)==0)//һ�����յ���Ϣ,����ʾ����.
			{
				tmp_buf[32]=0;//�����ַ���������
			} 
			flag_one  = tmp_buf[2];
			
//			if( tmp_buf[18] != 0  )
//			{
//				if( ( tmp_buf[0] > 120 && tmp_buf[0] < 130 ) || ( tmp_buf[0] > 110 && tmp_buf[0] < 120 )  )
//						tmp_buf[0] = 120;
//					forward = tmp_buf[0] - 120;
//					if( ( tmp_buf[1] > 120 && tmp_buf[1] < 125 ) || ( tmp_buf[1] > 115 && tmp_buf[1] < 120 )  )
//						tmp_buf[1] = 120;
//					rc_turn = tmp_buf[1] - 120;
//					rc_led  = tmp_buf[2];
//			}
//			else
//			{
//				rc_turn = 0;
//				rc_led = 0;
//				forward = 0;
//			}
			
       vTaskDelay(5);
    }
}

int button_keyio = 0,button_keyget = 0,button_keytime = 0,button_keyflag = 0,time_key = 0;
//����
void led2_task(void *pvParameters)
{
	while(1)
	{
//		//button_keyio =  rc_led;
//	  time_key++;
//			if( time_key <= 150 )
//			{
//					if( button_keyget == 0)
//					{
//						 if ( button_keyio == 1)
//								button_keytime ++;
//						 if( button_keytime >= 2 )
//						 {
//							 button_keyget = 1;
//							 button_keytime = 0;
//							 button_keyflag ++;      //����
//						 }	 
//				 }	
//					//��������
//				 if( button_keyget == 1 )	
//				 {
//						if( button_keyio == 0 )
//							button_keytime ++;
//						if( button_keytime >=2 )
//						 { 
//							 button_keyget = 0;
//							 button_keytime = 0;
//						 }
//				 }}
//			else
//			{
//				button_keyget = 0;
//				button_keytime = 0;
//				time_key = 0;
//				if( button_keyflag == 1 ){
////					LED_red = 0;
////					LED_green = 0;
//				}
//				else if ( button_keyflag == 2 ){
////					LED_red = 1;
////					LED_green = 0;
//				}
//				else if ( button_keyflag == 3 ){
////					LED_red = 0;
////					LED_green = 1;
//				}
//				button_keyflag = 0 ;
//				time_key = 0;
//			}
  	vTaskDelay(10);	
	}
}


