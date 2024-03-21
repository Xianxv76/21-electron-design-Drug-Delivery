#ifndef __ENCODER_TASK_H_
#define __ENCODER_TASK_H_

#include "sys.h"
#include "FreeRTOS.h"
#include "task.h"
#include "rm_pid.h"
#include "enconder.h"
#include "led.h"
#include "task_car.h"
#include "task_two.h"
#include "task_three.h"


#define RATE_BUF_SIZE 6
#define speed_set 1500

typedef struct{
	int32_t raw_value;   					 //���������������ԭʼֵ
	int32_t last_raw_value;					 //��һ�εı�����ԭʼֵ
	float ecd_value;                       //��������������ı�����ֵ
	int32_t diff;							 //���α�����֮��Ĳ�ֵ
	int32_t temp_count;                      //������
	uint8_t buf_count;						 //�˲�����buf��
	int32_t ecd_raw_rate;					 //ͨ������������õ����ٶ�ԭʼֵ
	int32_t rate_buf[RATE_BUF_SIZE];	     //buf��for filter
	int32_t round_cnt;						 //Ȧ��
	int32_t ecd_rate;					 //�ٶ�

		
}motor_date_ty;

void encoder_task(void *pvParameters);
void moto_speed_get(motor_date_ty*right_motor,int cnt);
void speed_send(void);


#endif
