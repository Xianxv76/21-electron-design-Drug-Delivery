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

//任务优先级
#define START_TASK_PRIO		1
//任务堆栈大小	
#define START_STK_SIZE 		128  
//任务句柄
TaskHandle_t StartTask_Handler;
//任务函数
void start_task(void *pvParameters);

//任务优先级
#define ENCODER_TASK_PRIO		2
//任务堆栈大小	
#define ENCODER_STK_SIZE 		128  
//任务句柄
TaskHandle_t ENCODERTask_Handler;
//任务函数
void encoder_task(void *pvParameters);

//任务优先级
#define LED1_TASK_PRIO		2
//任务堆栈大小	
#define LED1_STK_SIZE 		128  
//任务句柄
TaskHandle_t LED1Task_Handler;
//任务函数
void led1_task(void *pvParameters);

//任务优先级
#define LED2_TASK_PRIO		2
//任务堆栈大小	
#define LED2_STK_SIZE 		128  
//任务句柄
TaskHandle_t LED2Task_Handler;
//任务函数
void led2_task(void *pvParameters);


//任务优先级
#define BUTTON_TASK_PRIO		2
//任务堆栈大小	
#define BUTTON_STK_SIZE 		128  
//任务句柄
TaskHandle_t BUTTONTask_Handler;
//任务函数
void button_task(void *pvParameters);

int check_ok = 0;

int main(void)
{ 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);//设置系统中断优先级分组4
	delay_init(168);		//初始化延时函数
	LED_Init();		        //初始化LED端口
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
	//创建开始任务
    xTaskCreate((TaskFunction_t )start_task,            //任务函数
                (const char*    )"start_task",          //任务名称
                (uint16_t       )START_STK_SIZE,        //任务堆栈大小
                (void*          )NULL,                  //传递给任务函数的参数
                (UBaseType_t    )START_TASK_PRIO,       //任务优先级
                (TaskHandle_t*  )&StartTask_Handler);   //任务句柄              
    vTaskStartScheduler();          //开启任务调度
}


 
//开始任务任务函数
void start_task(void *pvParameters)
{
    taskENTER_CRITICAL();           //进入临界区
    //创建encoder任务
    xTaskCreate((TaskFunction_t )encoder_task,     
                (const char*    )"encoder_task",   
                (uint16_t       )ENCODER_STK_SIZE, 
                (void*          )NULL,
                (UBaseType_t    )ENCODER_TASK_PRIO,
                (TaskHandle_t*  )&ENCODERTask_Handler); 
    //创建LED1任务
    xTaskCreate((TaskFunction_t )led1_task,     
                (const char*    )"led1_task",   
                (uint16_t       )LED1_STK_SIZE, 
                (void*          )NULL,
                (UBaseType_t    )LED1_TASK_PRIO,
                (TaskHandle_t*  )&LED1Task_Handler); 

    //创建LED2任务
    xTaskCreate((TaskFunction_t )led2_task,     
                (const char*    )"led2_task",   
                (uint16_t       )LED2_STK_SIZE, 
                (void*          )NULL,
                (UBaseType_t    )LED2_TASK_PRIO,
                (TaskHandle_t*  )&LED2Task_Handler); 
								
    //创建LED2任务
    xTaskCreate((TaskFunction_t )button_task,     
                (const char*    )"button_task",   
                (uint16_t       )BUTTON_STK_SIZE, 
                (void*          )NULL,
                (UBaseType_t    )BUTTON_TASK_PRIO,
                (TaskHandle_t*  )&BUTTONTask_Handler); 								
  
    vTaskDelete(StartTask_Handler); //删除开始任务
    taskEXIT_CRITICAL();            //退出临界区
}

extern motor_date_ty right_motor;
extern motor_date_ty left_motor;

int rx_check = 0;
u8 tmp_buf[33];
int flag_one = 0;  //

	
//LED1任务函数
void led1_task(void *pvParameters)
{
    while(1)
    {
			 moto_speed_get(&right_motor,TIM3->CNT);
	     moto_speed_get(&left_motor,TIM4->CNT);
			
			rx_check = NRF24L01_RxPacket(tmp_buf);
			if(NRF24L01_RxPacket(tmp_buf)==0)//一旦接收到信息,则显示出来.
			{
				tmp_buf[32]=0;//加入字符串结束符
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
//按键
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
//							 button_keyflag ++;      //触发
//						 }	 
//				 }	
//					//触发结束
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


