#include "sys.h"
#include "timer.h"
#include "key.h"

u32 i[16]={ 0,0,0,0,
	          0,0,0,0,
	          0,0,0,0,
	          0,0,0,0,
          } ;
u8 key_state = 0;

u32 a[3]={0,0,0},c[3]={0,0,0},d[3]={0,0,0};
u32 i_10ms=0;
u32 x;

button button_key  = {0,0,0,0};

	void button_task(void *pvParameters)
	{
		while(1)
		{
       	switch(key_state) 
		  	{
				case 0:
/////////////  �ȴ��õ��ߵ�ƽ //////////////					
									colall();
									x = GPIOC->IDR >> 10 & 0x0f ;  //��ȡ234 �ڵ�ƽ
									if(x!=0x0F)
									{
										d[0]++;
										i_10ms++;
										if(i_10ms >= 2){
											key_state=1;						//�������θߵ�ƽ
											i_10ms=0;
										}
									}
								 break ;
					
			  case 1: 
					    
 //////////////    ɨ��   /////////////////////
/////////////     ��һ��  /////////////////////
									col1();
									x = GPIOC->IDR >> 10 & 0x0f ;
									switch (x)	        
											{
											case 0x0B : i[0]++; key_state=2;
												break;   //��һ��
											case 0x0D : i[1]++; key_state=2; 
												break;	  //�ڶ���
											case 0x0E : i[2]++; key_state=2; 
												break;
											default :  break;
											}	
											
								if(key_state==2)   //ȷ��Ϊ��һ��  ��������ɨ��
									break;
								
		//////////////////  �ڶ���	//////////////////				
									col2();	
									x = GPIOC->IDR >> 10 & 0x0f ;	
									switch (x)	
											{
											case 0x0B : i[3]++; key_state=2;
												break;   //��һ��
											case 0x0D : i[4]++; key_state=2; 
												break;	  //�ڶ���
											case 0x0E : i[5]++; key_state=2; 
												break;
											default :  break;
											}	
									
							 if(key_state==2)    //ȷ��Ϊ�ڶ���  ����ɨ��
									break;

		///////////////  ������   ////////////////////					 
									col3();
									x = GPIOC->IDR >> 10 & 0x0f ;
									switch (x)	
											{
											case 0x0B : i[6]++; key_state=2;
												break;   //��һ��
											case 0x0D : i[7]++; key_state=2; 
												break;	  //�ڶ���
											case 0x0E : i[8]++; key_state=2; 
												break;
											default : key_state=2; break;
											}	
										break;
									
				case 2: 
 ///////////   �ȴ��õ��͵�ƽ ��λ   ///////////

									 x = GPIOC->IDR >> 10 & 0x0f ;
			 
									 if(x==0x0F)
									 {
										 i_10ms++;
										 if(i_10ms>=2) {
											 key_state=0;
											 i_10ms=0;
										 }	 
									 }break;
          }		
				
				button_key.io =  GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_13);
	
			if( button_key.get == 0)
			{
				 if ( button_key.io == 0)
						button_key.time ++;
				 if( button_key.time >= 3 )
				 {
					 button_key.get = 1;
					 button_key.time = 0;
					 button_key.flag ++;      //����
				 }	 
		 }	
			//��������
		 if( button_key.get == 1 )	
		 {
				if( button_key.io == 1 )
					button_key.time ++;
				if( button_key.time >=3 )
				 { 
					 button_key.get = 0;
					 button_key.time = 0;
				 }
		 }
		 
        vTaskDelay(10);	
			}					
	}




		void col1(void)
		{
				COLUMN0=0;
				COLUMN1=1;
				COLUMN2=1;
		}

		void col2(void)
		{
				COLUMN0=1;
				COLUMN1=0;
				COLUMN2=1;
		}
    void col3(void)
		{
				COLUMN0=1;
				COLUMN1=1;
				COLUMN2=0;
		}
		
    void colrest(void)
		{
				COLUMN0=1;
				COLUMN1=1;
				COLUMN2=1;
		}
    void colall(void)
		{
				COLUMN0=0;
				COLUMN1=0;
				COLUMN2=0;
		}



			
//			
//			i[0]=ROW0;
//			i[1]=ROW1;
//			i[2]=ROW2;
//			
//			////////       ��һ��  ////////////
//					colrest();   //��λ
//					col1();      //1���ø�
//			
//					if(c[0]==0)       //δ����ʱ  ���ߵ�ƽ
//										 if (i[0]==1)
//										 {
//														 a[0]++;
//														 if(a[0]>=2)
//															 {
//																a[0]=0;
//																row[0]++;   // ȷ���д���
//																c[0]=1;
////																colrest();
////																col1();
////																 if (i[0]==1)
////																	 column[0]++;
////																 
//														 
//															 }
//																 
//																 
//															 
//										 }				 
//						 
//					if(c[0]==1)        //  ������һ�� ���� ���͵�ƽ
//										if(i[0]==0)	
//										{
//														a[0]++;
//														if(a[0]>=3)
//															{
//																c[0]=0;
//																a[0]=0;
//															}							
//										}

