/**********************************
包含头文件
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
变量定义
**********************************/
uchar key_num = 0;               //按键标志位
uchar flag_display = 0;	         //显示标志位
uchar flag_display_change = 0;	 //切换显示标志位
uint time_num = 0;	         //计时变量
uint light_value = 0;	         //光照强度
uint light_min = 30;	         //光照强度最小值
uint water_value = 0;	         //雨滴
uint water_max = 30;	         //雨滴最大值
uint temp_value = 0;	         //温度
uint temp_min = 10;	         //温度最小值
uint humi_value = 0;	         //湿度
uint humi_max = 60;	         //湿度最大值
uint time_value = 0;	         //定时
uint time_all = 0;	         //定时总时间
char motor_num_max = 5;             //电机总圈数
char motor_num = 0;             //电机圈数
bit flag_motor = 0;	         //电机标志位
bit motor_state = 1;           //电机状态
bit motor_stop = 0;

int speed = 100;     //0100   0110  0010  0011 0001
uchar code fan[] = {0x04,0x06,0x02,0x03,0x01,0x09,0x08,0x0c};
uchar code zheng[] = {0x08,0x09,0x01,0x03,0x02,0x06,0x04,0x0c};

/**********************************
函数声明
**********************************/
void Delay_function(uint x);	//延时函数
void Key_function(void);	//按键函数
void Monitor_function(void);	//监测函数
void Display_function(void);	//显示函数
void Manage_function(void);	//处理函数
void Time1_Init(void);		//定时器1初使化函数

/****
*******	主函数 
*****/
void main()
{
	BUZZER = 0;
	
	Lcd1602_Init();		//LCD1602初始化
	Delay_function(50);	//延时50ms
	lcd1602_clean();	//清屏
	Delay_function(50);	//延时50ms
	DHT11_Init();		//DHT11初始化
	Delay_function(50);	//延时50ms
	Timer0_Init();		//定时器0初使化
	Delay_function(50);	//延时50ms

	while(1)
	{
		Key_function();		//按键函数
		Monitor_function();	//监测函数
		Display_function();	//显示函数
		Manage_function();	//处理函数
	
		Delay_function(10);	//延时10ms
		time_num++;		//计时变量+1
		if(time_num > 5000)
		{
			time_num = 0;
		}
	}
}

/****
*******	延时 x ms函数
*****/
void Delay_function(uint x)
{
	uint m,n;
	for(m=x;m>0;m--)
	for(n=110;n>0;n--);
}

/****
*******按键函数
*****/
void Key_function(void)
{
	key_num = Chiclet_Keyboard_Scan(0);	//按键扫描
	if(key_num != 0)	//有按键按下
	{
		switch(key_num)
		{
			case 1:					//按键1
				flag_display++;					//显示标志位+1
				if(flag_display > 5)				//一共6个模式
					flag_display = 0;

				lcd1602_clean();				//清屏
			break;
			
			case 2:	//改变测量显示界面
				flag_display_change++;
				if(flag_display_change > 2)
					flag_display_change = 0;
	
				lcd1602_clean();					//清屏
			break;
				
			case 3:	//手控电机
				motor_state = ~motor_state;
			break;
				
			case 4:		
				switch(flag_display)					//根据不同的显示标志位，改变不同的标志位
				{
					case 0:						//手动控制电机正转
						flag_motor = 1; motor_stop = 1;
					case 1:						//改变温度最小值，进行+10
						if(temp_min < 99)                                                    
							temp_min += 10;                                                  
					break;                                                                       
													       
					case 2:						//改变湿度最大值，进行+10
						if(humi_max < 99)                                                    
							humi_max += 10;                                                  
					break;                                                                       
													       
					case 3:						//改变光照最小值，进行+10
						if(light_min < 99)                                                    
							light_min += 10;                                                  
					break;                                                                        
													        
					case 4:						//改变风速最大值，进行+10
						if(water_max < 99)                                                   
							water_max += 10;                                                 
					break;                                                                       
													       
					case 5:						//改变定时时间，进行+10
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
				switch(flag_display)					//根据不同的显示标志位，改变不同的标志位
				{
					case 0:							//手动控制电机反转
						flag_motor = 0; motor_stop = 1;
					break;
					
					case 1:							//改变温度最小值，进行-10
						if(temp_min > 0)                                                        
							temp_min -= 10;                                                 
					break;                                                                          
					                                                         		       
					case 2:							//改变湿度最大值，进行-10
						if(humi_max > 0)                                                        
							humi_max -= 10;                                                 
					break;                                                                          
					                                                         		       
					case 3:							//改变光照最小值，进行-10
						if(light_min > 0)                                                        
							light_min -= 10;                                                 
					break;                                                                           
					                                                         		        
					case 4:							//改变风速最大值，进行-10
						if(water_max > 0)                                                       
							water_max -= 10;                                                
					break;                                                                          
					                                                         		       
					case 5:							//改变定时时间，进行-10
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
*******监测函数
*****/
void Monitor_function(void)
{
	if(time_num % 50 == 0)//500ms检测一次
	{
		water_value = (((Adc0832_Get_Value(0)/255.0)*100));			//获取雨滴
		light_value = (((Adc0832_Get_Value(1)/255.0)*100));			//获取光照值
		Dht11_Get_Temp_Humi_Value(&temp_value, &humi_value);			//获取温湿度
	}
}

/****
*******显示函数
*****/
void Display_function(void)
{
	switch(flag_display)//根据不同的显示标志位，显示不同的设置界面
	{
		case 0://界面0
			switch(flag_display_change)	//根据切换显示标志位，显示不同的测量界面
			{
				case 1:
					lcd1602_display_str(1,0,"T:");		//显示温度
					lcd1602_display_temp(1,2,temp_value);
				
					lcd1602_display_str(2,0,"H:");		//显示湿度
					lcd1602_display_humi(2,2,humi_value * 10);
				
					lcd1602_display_str(1,10,"L:");	        //显示光照强度
					lcd1602_display_light(1,12,light_value * 10);
				
					lcd1602_display_str(2,10,"W:");		//显示风速
					lcd1602_display_water(2,12,water_value * 10);
				break;

				case 0:
					lcd1602_display_str(1,0,"Time:");		//显示定时剩余时间
					lcd1602_display_time(1,5,time_all);
				
					if (flag_motor)
						lcd1602_display_str(2,7,"CCW"); //顺时针转
					else
						lcd1602_display_str(2,7,"CW ");  //逆时针转
					
					if (motor_state)
						lcd1602_display_str(2,0, " RC "); //顺时针转
					else
						lcd1602_display_str(2,0, "Auto");  //逆时针转
					lcd1602_display_str(1,14,"Num");
					lcd1602_display_num(2,14,motor_num);					
				break;

				case 2:
					lcd1602_display_str(1,0,"M");

					lcd1602_display_str(1,2,"T:>");		//显示温度
					lcd1602_display_num(1,5,temp_min);
				
					lcd1602_display_str(2,2,"H:<");		//显示湿度
					lcd1602_display_num(2,5,humi_max);
				
					lcd1602_display_str(1,12,"L:>");        //显示光照强度
					lcd1602_display_num(1,15,light_min);
				
					lcd1602_display_str(2,12,"W:<");        //显示风速
					lcd1602_display_num(2,15,water_max);
				break;
					
				default:
				break;
			}
		break;

		case 1://设置界面1，设置温度最小值
			lcd1602_display_str(1,2,"Set Temp Min");
			lcd1602_display_num(2,7,temp_min);
		break;

		case 2://设置界面2，设置湿度最大值
			lcd1602_display_str(1,2,"Set Humi Max");
			lcd1602_display_num(2,7,humi_max);
		break;
			
		case 3://设置界面3，设置光照最小值
			lcd1602_display_str(1,1,"Set Light Min");
			lcd1602_display_num(2,7,light_min);

		break;
			
		case 4://设置界面4，设置风速最大值
			lcd1602_display_str(1,2,"Set Water Max");
			lcd1602_display_num(2,7,water_max);
		break;

		case 5://设置界面5，设置定时时间
			lcd1602_display_str(1,1,"Set Time Value");
			lcd1602_display_num(2,7,time_value);
		break;

		default:
		break;
	}
}

/****
*******处理函数
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
	
	if(flag_display == 0)	//显示标志位为0时
	{
		//温度、湿度、光照、雨滴、定时、电机状态限定之外，强制电机反转，收回晾衣架
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
