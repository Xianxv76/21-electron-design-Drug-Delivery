#include "main.h"
#include "vision.h"



//�������ȼ�
#define START_TASK_PRIO		1
//�����ջ��С	
#define START_STK_SIZE 		128  
//������
TaskHandle_t StartTask_Handler;
//������
void start_task(void *pvParameters);

//�������ȼ�
#define INS_TASK_PRIO		2
//�����ջ��С	
#define INS_STK_SIZE 		128  
//������
TaskHandle_t INSTask_Handler;
//������
void ins_task(void *pvParameters);

int a = 0;
int main(void)
{ 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);//����ϵͳ�ж����ȼ�����4
	delay_init(168);		//��ʼ����ʱ����
	
  delay_ms(500);
	delay_ms(500);
	vision_init();
	bim088_spi_init();      //bmi088��spi�ĳ�ʼ��
	
	while(BMI088_init())
    { ; }
	 a = 1 ;
	LED_Init();		        //��ʼ��LED�˿�
	
	
	
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
    //����LED0����
    xTaskCreate((TaskFunction_t )ins_task,     	
                (const char*    )"ins_task",   	
                (uint16_t       )INS_STK_SIZE, 
                (void*          )NULL,				
                (UBaseType_t    )INS_TASK_PRIO,	
                (TaskHandle_t*  )&INSTask_Handler);   

								
    vTaskDelete(StartTask_Handler); //ɾ����ʼ����
    taskEXIT_CRITICAL();            //�˳��ٽ���
}




//LED0������ 
void ins_task(void *pvParameters)
{
	
    while(1)
    {
     bmi_get();
			
     vTaskDelay(1);
    }
}   





