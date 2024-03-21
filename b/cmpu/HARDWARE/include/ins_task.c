#include "ins_task.h"
#include "main.h"
#include "vision.h"

u8 USART_TX_BUF[10] ;
int zero_flag = 0;

float gyro_buf[3], accel_buf[3], temp_buf; //ԭʼ����

float gyro_zero[3] = {0.00013f,0.0032f,0.00232};     //��Ư

float mpu_zero_kp = 0.000204;  //��Ưϵ��      //0.0115f   100 �ε�ֵ

float gyro_pass[3] = {0,0,0},
	    accel_pass[3]= {0,0,0};     //��ͨ�˲��������

float gyro[3] = {0,0,0};      //������������
      

float low_pass_kp = 0.08f;
union float32 yaw;


/*             ������        

�޴������
			
*/

void bmi_get(void)
{
	int i;int t;
	
	  //�õ����ٶ� ��λ��ת��Ϊ�Ƕ�
	 BMI088_read(gyro_buf, accel_buf, &temp_buf);   
		//��ͨ�˲�	
	 bmi_lowpass(gyro_buf,gyro_pass,accel_buf,accel_pass);    
	
		for(i = 0; i<=2; i++)
		gyro[i] = gyro_buf[i] - gyro_zero[i];
    //��Ԫ������
		bmi_ins(accel_pass[0],accel_pass[1],accel_pass[2],gyro[0],gyro[1],gyro[2]);  //���ٶ��˲� 
	  		  
	    USART_TX_BUF[0] = 0x55;
	    USART_TX_BUF[1] = 0x55;
    	USART_TX_BUF[2] = yaw.data[0];
	    USART_TX_BUF[3] = yaw.data[1];
	    USART_TX_BUF[4] = yaw.data[2];
	    USART_TX_BUF[5] = yaw.data[3];
	
			for(t=0;t < 10; t++)
			{	
				USART_SendData(USART6, USART_TX_BUF[t]); 
        while(USART_GetFlagStatus(USART6,USART_FLAG_TC)!=SET);				
			}		
}

void bmi_lowpass(float *gyro_buf, float *gyro_pass, float*accel_buf, float*accel_pass)
{
	int i;
	for(i = 0; i<=2 ;i++)
	{
		gyro_pass[i] = ( gyro_buf[i] * low_pass_kp ) + ( 1 - low_pass_kp) * gyro_pass[i];
		
		accel_pass[i] = ( accel_buf[i] * low_pass_kp ) + ( 1 - low_pass_kp) * accel_pass[i];
	}
	
}

float q0=1,q1=0,q2=0,q3=0;
float exint=0,eyint=0,ezint=0;
float mpu_ki=0.0f,mpu_kp=3.0f;
float yaw1, pit, rol,count,yaw_last;
float vx,vy,vz;
float ex,ey,ez;
float norm;
float last = 0, now = 0;
float halfT = 0;
float half = 0;
	
	
void bmi_ins(float ax, float ay, float az, float gx, float gy, float gz)
{ 
	 last=now;
   now=xTaskGetTickCount();
	
	 half = (now-last);
   halfT = (now-last) * 0.0005f;

    if(ax*ay*az==0)
        return;
		
  norm = sqrt(ax*ax + ay*ay + az*az);       //acc���ݹ�һ��
  ax = ax /norm;
  ay = ay / norm;
  az = az / norm;
 
  // estimated direction of gravity and flux (v and w)              �����������������/��Ǩ
  vx = 2*(q1*q3 - q0*q2);                                             //��Ԫ����xyz�ı�ʾ
  vy = 2*(q0*q1 + q2*q3);
  vz = q0*q0 - q1*q1 - q2*q2 + q3*q3 ;
 
  // error is sum of cross product between reference direction of fields and direction measured by sensors
  ex = (ay*vz - az*vy) ;                                             //�������������õ���־������
  ey = (az*vx - ax*vz) ;
  ez = (ax*vy - ay*vx) ;
 
  exint = exint + ex * mpu_ki;                                //�������л���
  eyint = eyint + ey * mpu_ki;
  ezint = ezint + ez * mpu_ki;
 
  // adjusted gyroscope measurements
  gx = gx + mpu_kp*ex + exint;                                              //�����PI�󲹳��������ǣ����������Ư��
  gy = gy + mpu_kp*ey + eyint;
  gz = gz + mpu_kp*ez + ezint; 
                                         //�����gz����û�й۲��߽��н��������Ư�ƣ����ֳ����ľ��ǻ����������Լ�
//  ex1=Kp*ex + exInt;
//  ey1=Kp*ey + eyInt;
//  ey1=Kp*ez + ezInt;
  // integrate quaternion rate and normalise   //��Ԫ�ص�΢�ַ���
                          												 
  q0 = q0 + (-q1*gx - q2*gy - q3*gz)*halfT;  //1/2000.f;
  q1 = q1 + (q0*gx + q2*gz - q3*gy)*halfT;   //1/2000.f;
  q2 = q2 + (q0*gy - q1*gz + q3*gx)*halfT;   //1/2000.f;
  q3 = q3 + (q0*gz + q1*gy - q2*gx)*halfT;   //1/2000.f;
 
  // normalise quaternion
  norm = sqrt(q0*q0 + q1*q1 + q2*q2 + q3*q3);
  q0 = q0 / norm;
  q1 = q1 / norm;
  q2 = q2 / norm;
  q3 = q3 / norm;
 
  yaw1 = atan2(2*(q1*q2 + q0*q3),q0*q0+q1*q1-q2*q2-q3*q3) * 57.3 ;// yaw
    	if(yaw1 - yaw_last <= -150)
			 count++;
			else if(yaw1 - yaw_last >= 150)
			 count--;
			yaw.angle = yaw1 + count * 360.f;
			yaw_last =  yaw1;                
  pit = asin(-2 * q1 * q3 + 2 * q0* q2)* 57.3; // pitch
  rol = atan2(2 * q2 * q3 + 2 * q0 * q1, -2 * q1 * q1 - 2 * q2* q2 + 1)* 57.3; // roll
}


