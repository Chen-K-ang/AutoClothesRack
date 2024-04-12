/**********************************
����ͷ�ļ�
**********************************/
#include "main.h"
#include "lcd1602.h"
#include "key.h"
#include "adc.h"
#include "dht11.h"
#include "timer.h"
#include "motor_bujin.h"

sbit BUZZER = P1^4;

/**********************************
��������
**********************************/
uchar key_num = 0;               //������־λ
uchar flag_display = 0;	         //��ʾ��־λ
uchar flag_display_change = 0;	 //�л���ʾ��־λ
uint time_num = 0;	         //��ʱ����
uint light_value = 0;	         //����ǿ��
uint light_min = 30;	         //����ǿ����Сֵ
uint water_value = 0;	         //���
uint water_max = 30;	         //������ֵ
uint temp_value = 0;	         //�¶�
uint temp_min = 10;	         //�¶���Сֵ
uint humi_value = 0;	         //ʪ��
uint humi_max = 60;	         //ʪ�����ֵ
uint time_value = 0;	         //��ʱ
uint time_all = 0;	         //��ʱ��ʱ��
char motor_num_max = 5;             //�����Ȧ��
char motor_num = 0;             //���Ȧ��
bit flag_motor = 0;	         //�����־λ
bit motor_state = 1;           //���״̬
bit motor_stop = 0;

int speed = 100;     //0100   0110  0010  0011 0001
uchar code fan[] = {0x04,0x06,0x02,0x03,0x01,0x09,0x08,0x0c};
uchar code zheng[] = {0x08,0x09,0x01,0x03,0x02,0x06,0x04,0x0c};

/**********************************
��������
**********************************/
void Delay_function(uint x);	//��ʱ����
void Key_function(void);	//��������
void Monitor_function(void);	//��⺯��
void Display_function(void);	//��ʾ����
void Manage_function(void);	//������
void Time1_Init(void);		//��ʱ��1��ʹ������

/****
*******	������ 
*****/
void main()
{
	BUZZER = 0;
	
	Lcd1602_Init();		//LCD1602��ʼ��
	Delay_function(50);	//��ʱ50ms
	lcd1602_clean();	//����
	Delay_function(50);	//��ʱ50ms
	DHT11_Init();		//DHT11��ʼ��
	Delay_function(50);	//��ʱ50ms
	Timer0_Init();		//��ʱ��0��ʹ��
	Delay_function(50);	//��ʱ50ms

	while(1)
	{
		Key_function();		//��������
		Monitor_function();	//��⺯��
		Display_function();	//��ʾ����
		Manage_function();	//������
	
		Delay_function(10);	//��ʱ10ms
		time_num++;		//��ʱ����+1
		if(time_num > 5000)
		{
			time_num = 0;
		}
	}
}

/****
*******	��ʱ x ms����
*****/
void Delay_function(uint x)
{
	uint m,n;
	for(m=x;m>0;m--)
	for(n=110;n>0;n--);
}

/****
*******��������
*****/
void Key_function(void)
{
	key_num = Chiclet_Keyboard_Scan(0);	//����ɨ��
	if(key_num != 0)	//�а�������
	{
		switch(key_num)
		{
			case 1:					//����1
				flag_display++;					//��ʾ��־λ+1
				if(flag_display > 5)				//һ��6��ģʽ
					flag_display = 0;

				lcd1602_clean();				//����
			break;
			
			case 2:	//�ı������ʾ����
				flag_display_change++;
				if(flag_display_change > 2)
					flag_display_change = 0;
	
				lcd1602_clean();					//����
			break;
				
			case 3:	//�ֿص��
				motor_state = ~motor_state;
			break;
				
			case 4:		
				switch(flag_display)					//���ݲ�ͬ����ʾ��־λ���ı䲻ͬ�ı�־λ
				{
					case 0:						//�ֶ����Ƶ����ת
						flag_motor = 1; motor_stop = 1;
					case 1:						//�ı��¶���Сֵ������+10
						if(temp_min < 99)                                                    
							temp_min += 10;                                                  
					break;                                                                       
													       
					case 2:						//�ı�ʪ�����ֵ������+10
						if(humi_max < 99)                                                    
							humi_max += 10;                                                  
					break;                                                                       
													       
					case 3:						//�ı������Сֵ������+10
						if(light_min < 99)                                                    
							light_min += 10;                                                  
					break;                                                                        
													        
					case 4:						//�ı�������ֵ������+10
						if(water_max < 99)                                                   
							water_max += 10;                                                 
					break;                                                                       
													       
					case 5:						//�ı䶨ʱʱ�䣬����+10
						if(time_value < 24)
						{
							time_value += 10;
							time_all = time_value;
						}
					break;
					
					default:
					break;
				}
			break;

			case 5:	
				switch(flag_display)					//���ݲ�ͬ����ʾ��־λ���ı䲻ͬ�ı�־λ
				{
					case 0:							//�ֶ����Ƶ����ת
						flag_motor = 0; motor_stop = 1;
					break;
					
					case 1:							//�ı��¶���Сֵ������-10
						if(temp_min > 0)                                                        
							temp_min -= 10;                                                 
					break;                                                                          
					                                                         		       
					case 2:							//�ı�ʪ�����ֵ������-10
						if(humi_max > 0)                                                        
							humi_max -= 10;                                                 
					break;                                                                          
					                                                         		       
					case 3:							//�ı������Сֵ������-10
						if(light_min > 0)                                                        
							light_min -= 10;                                                 
					break;                                                                           
					                                                         		        
					case 4:							//�ı�������ֵ������-10
						if(water_max > 0)                                                       
							water_max -= 10;                                                
					break;                                                                          
					                                                         		       
					case 5:							//�ı䶨ʱʱ�䣬����-10
						if(time_value > 0)
							{
								time_value -= 10;
								time_all = time_value * 6;
							}
					break;
					
					default:
					break;
				}				
			break;

			default:
			break;
		}
	}
}

/****
*******��⺯��
*****/
void Monitor_function(void)
{
	if(time_num % 50 == 0)//500ms���һ��
	{
		water_value = (((Adc0832_Get_Value(0)/255.0)*100));			//��ȡ���
		light_value = (((Adc0832_Get_Value(1)/255.0)*100));			//��ȡ����ֵ
		Dht11_Get_Temp_Humi_Value(&temp_value, &humi_value);			//��ȡ��ʪ��
	}
}

/****
*******��ʾ����
*****/
void Display_function(void)
{
	switch(flag_display)//���ݲ�ͬ����ʾ��־λ����ʾ��ͬ�����ý���
	{
		case 0://����0
			switch(flag_display_change)	//�����л���ʾ��־λ����ʾ��ͬ�Ĳ�������
			{
				case 1:
					lcd1602_display_str(1,0,"T:");		//��ʾ�¶�
					lcd1602_display_temp(1,2,temp_value);
				
					lcd1602_display_str(2,0,"H:");		//��ʾʪ��
					lcd1602_display_humi(2,2,humi_value * 10);
				
					lcd1602_display_str(1,10,"L:");	        //��ʾ����ǿ��
					lcd1602_display_light(1,12,light_value * 10);
				
					lcd1602_display_str(2,10,"W:");		//��ʾ����
					lcd1602_display_water(2,12,water_value * 10);
				break;

				case 0:
					lcd1602_display_str(1,0,"Time:");		//��ʾ��ʱʣ��ʱ��
					lcd1602_display_time(1,5,time_all);
				
					if (flag_motor)
						lcd1602_display_str(2,7,"CCW"); //˳ʱ��ת
					else
						lcd1602_display_str(2,7,"CW ");  //��ʱ��ת
					
					if (motor_state)
						lcd1602_display_str(2,0, " RC "); //˳ʱ��ת
					else
						lcd1602_display_str(2,0, "Auto");  //��ʱ��ת
					lcd1602_display_str(1,14,"Num");
					lcd1602_display_num(2,14,motor_num);					
				break;

				case 2:
					lcd1602_display_str(1,0,"M");

					lcd1602_display_str(1,2,"T:>");		//��ʾ�¶�
					lcd1602_display_num(1,5,temp_min);
				
					lcd1602_display_str(2,2,"H:<");		//��ʾʪ��
					lcd1602_display_num(2,5,humi_max);
				
					lcd1602_display_str(1,12,"L:>");        //��ʾ����ǿ��
					lcd1602_display_num(1,15,light_min);
				
					lcd1602_display_str(2,12,"W:<");        //��ʾ����
					lcd1602_display_num(2,15,water_max);
				break;
					
				default:
				break;
			}
		break;

		case 1://���ý���1�������¶���Сֵ
			lcd1602_display_str(1,2,"Set Temp Min");
			lcd1602_display_num(2,7,temp_min);
		break;

		case 2://���ý���2������ʪ�����ֵ
			lcd1602_display_str(1,2,"Set Humi Max");
			lcd1602_display_num(2,7,humi_max);
		break;
			
		case 3://���ý���3�����ù�����Сֵ
			lcd1602_display_str(1,1,"Set Light Min");
			lcd1602_display_num(2,7,light_min);

		break;
			
		case 4://���ý���4�����÷������ֵ
			lcd1602_display_str(1,2,"Set Water Max");
			lcd1602_display_num(2,7,water_max);
		break;

		case 5://���ý���5�����ö�ʱʱ��
			lcd1602_display_str(1,1,"Set Time Value");
			lcd1602_display_num(2,7,time_value);
		break;

		default:
		break;
	}
}

/****
*******������
*****/
void Manage_function(void)
{
	unsigned char i, j;
	if (motor_state && motor_stop) {
		motor_stop = 0;
		if (flag_motor && motor_num < motor_num_max) {
			motor_num++;
			for(i=0;i<8;i++){
				P3 = zheng[i];
				Delay_function(speed);
			}
		} else if (!flag_motor && motor_num > 0){
			for(i=0;i<8;i++){
				P3 = fan[i];
				Delay_function(speed);	
			}
			motor_num--;		
		}
		
		if (motor_num > motor_num_max)
			motor_num = motor_num_max;
		else if (motor_num < 0)
			motor_num = 0;
		
	}
	
	if(flag_display == 0)	//��ʾ��־λΪ0ʱ
	{
		//�¶ȡ�ʪ�ȡ����ա���Ρ���ʱ�����״̬�޶�֮�⣬ǿ�Ƶ����ת���ջ����¼�
		if((temp_value < (temp_min) || humi_value > (humi_max)
			|| light_value < (light_min) || water_value > (water_max)
			||(time_value != 0 && time_all == 0)) && motor_num)		
		{
			if (water_value > water_max)
				BUZZER = 1;
			for (j = 0; j < motor_num; j++)
				for(i=0;i<8;i++){
					P3 = fan[i];
					Delay_function(speed);	
				}
			motor_stop = 0;
			motor_num = 0;
			motor_state = 0;
			BUZZER = 0;
		}
	}
}
