#ifndef _INS_TASK_H_
#define _INS_TASK_H_
#include "main.h"


void bmi_zero_val(float *gyro,float *gyro_zero); 
void bmi_get(void);
void bmi_ins(float ax, float ay, float az, float gx, float gy, float gz);	
void bmi_lowpass(float *gyro_buf, float *gyro_pass, float*accel_buf, float*accel_pass);

#endif
