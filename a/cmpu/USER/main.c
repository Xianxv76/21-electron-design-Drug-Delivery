#include "main.h"
#include "vision.h"



//任务优先级
#define START_TASK_PRIO		1
//任务堆栈大小	
#define START_STK_SIZE 		128  
//任务句柄
TaskHandle_t StartTask_Handler;
//任务函数
void start_task(void *pvParameters);

//任务优先级
#define INS_TASK_PRIO		2
//任务堆栈大小	
#define INS_STK_SIZE 		128  
//任务句柄
TaskHandle_t INSTask_Handler;
//任务函数
void ins_task(void *pvParameters);

int a = 0;
int main(void)
{ 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);//设置系统中断优先级分组4
	delay_init(168);		//初始化延时函数
	
  delay_ms(500);
	delay_ms(500);
	vision_init();
	bim088_spi_init();      //bmi088的spi的初始化
	
	while(BMI088_init())
    { ; }
	 a = 1 ;
	LED_Init();		        //初始化LED端口
	
	
	
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
    //创建LED0任务
    xTaskCreate((TaskFunction_t )ins_task,     	
                (const char*    )"ins_task",   	
                (uint16_t       )INS_STK_SIZE, 
                (void*          )NULL,				
                (UBaseType_t    )INS_TASK_PRIO,	
                (TaskHandle_t*  )&INSTask_Handler);   

								
    vTaskDelete(StartTask_Handler); //删除开始任务
    taskEXIT_CRITICAL();            //退出临界区
}




//LED0任务函数 
void ins_task(void *pvParameters)
{
	
    while(1)
    {
     bmi_get();
			
     vTaskDelay(1);
    }
}   





