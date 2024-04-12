/**********************************
包含头文件
**********************************/
#include "lcd1602.h"


/**********************************
函数定义
**********************************/
/****
*******1602检测忙函数 
*****/
void lcd1602_check_busy()
{  
	P0=0xff;
	do
	{
		LCD1602_RS=0;
		LCD1602_RW=1;
		LCD1602_E=0;
		LCD1602_E=1;
	} while(LCD1602_BUSY==1);
	LCD1602_E=0;
}

/****
*******1602写数据忙函数 
*****/
void lcd1602_write_date(uchar date)
{
	lcd1602_check_busy();
	LCD1602_E=0;
	LCD1602_PORT=date;
	LCD1602_RS=1;
	LCD1602_RW=0;
	LCD1602_E=1;
	LCD1602_E=0;
}

/****
*******1602写命令函数 
*****/
void lcd1602_write_com(uchar com)
{
	lcd1602_check_busy();
	LCD1602_E=0;
	LCD1602_PORT=com;
	LCD1602_RS=0;
	LCD1602_RW=0;
	LCD1602_E=1;
	LCD1602_E=0;
}

/****
*******LCD1602显示字符串函数 
*******参量定义：	hang：输入显示行数  1 第一行  2 第二行
*******           add：偏移量，0-15
*******           dat：所要显示的字符串，输入格式 " "
*****/
void lcd1602_display_str(uchar hang, uchar add, uchar *dat)
{
	if(hang==1)   
		lcd1602_write_com(0x80+add);			//第一行
	else
		lcd1602_write_com(0x80+0x40+add);	//第二行

	while(*dat != '\0')
	{
		lcd1602_write_date(*dat++);
	
	}
}

/****
*******LCD1602显示时间函数(显示格式：**:**:**)
*******参量定义：	hang：输入显示行数  1 第一行  2 第二行
*******           add：偏移量，0-15
*******           time：所要显示的时间，以秒为单位
*****/
void lcd1602_display_time(uchar hang, uchar add, uint time)
{
	if(hang==1)   
		lcd1602_write_com(0x80+add);				//第一行
	else
		lcd1602_write_com(0x80+0x40+add);		//第二行

	
	lcd1602_write_date(time/3600/10%10+48);				//时钟的十位
	lcd1602_write_date(time/3600/1%10+48);				//时钟的个位
	lcd1602_write_date(':');											//；
	lcd1602_write_date(time%3600/60/10%10+48);		//分钟的十位
	lcd1602_write_date(time%3600/60/1%10+48);			//分钟的个位
	lcd1602_write_date(':');											//；
	lcd1602_write_date(time%60/10%10+48);					//秒钟的十位
	lcd1602_write_date(time%60/1%10+48);					//秒钟的个位
}

/****
*******LCD1602显示温度函数(显示格式：**.*℃)
*******参量定义：	hang：输入显示行数  1 第一行  2 第二行
*******           add：偏移量，0-15
*******           temp：所要显示的温度
*****/
void lcd1602_display_temp(uchar hang, uchar add,uint temp)
{
	if(hang==1)   
		lcd1602_write_com(0x80+add);			//第一行
	else
		lcd1602_write_com(0x80+0x40+add);	//第二行

	lcd1602_write_date(temp/100%10+48);
	lcd1602_write_date(temp/10%10+48);
	lcd1602_write_date('.');
	lcd1602_write_date(temp/1%10+48);
	lcd1602_write_date(0xdf);				//显示温度的小圆圈符号，0xdf是液晶屏字符库的该符号地址码
  lcd1602_write_date(0x43);				//显示"C"符号，0x43是液晶屏字符库里大写C的地址码
}

/****
*******LCD1602显示湿度函数(显示格式：**.*%)
*******参量定义：	hang：输入显示行数  1 第一行  2 第二行
*******           add：偏移量，0-15
*******           humi：所要显示的湿度
*****/
void lcd1602_display_humi(uchar hang, uchar add,uint humi)
{
	if(hang==1)   
		lcd1602_write_com(0x80+add);			//第一行
	else
		lcd1602_write_com(0x80+0x40+add);	//第二行

	lcd1602_write_date(humi/100%10+48);
	lcd1602_write_date(humi/10%10+48);
	lcd1602_write_date('.');
	lcd1602_write_date(humi/1%10+48);
	lcd1602_write_date('%');
}

/****
*******LCD1602显示光照强度函数(显示格式：**.*Lx)
*******参量定义：	hang：输入显示行数  1 第一行  2 第二行
*******           add：偏移量，0-15
*******           light：所要显示的光照强度
*****/
void lcd1602_display_light(uchar hang, uchar add,uint light)
{
	if(hang==1)   
		lcd1602_write_com(0x80+add);			//第一行
	else
		lcd1602_write_com(0x80+0x40+add);	//第二行

	lcd1602_write_date(light/100%10+48);
	lcd1602_write_date(light/10%10+48);
	lcd1602_write_date('.');
	lcd1602_write_date(light/1%10+48);
	lcd1602_write_date('L');
	lcd1602_write_date('x');
}

/****
*******LCD1602显示风速(显示格式：**.*m/s)
*******参量定义：	hang：输入显示行数  1 第一行  2 第二行
*******           add：偏移量，0-15
*******          water：所要显示的雨滴
*****/
void lcd1602_display_water(uchar hang, uchar add,uint water)
{
	if(hang==1)   
		lcd1602_write_com(0x80+add);			//第一行
	else
		lcd1602_write_com(0x80+0x40+add);	//第二行

	lcd1602_write_date(water/100%10+48);
	lcd1602_write_date(water/10%10+48);
	lcd1602_write_date('.');
	lcd1602_write_date(water/1%10+48);
	lcd1602_write_date('%');
}

/****
*******LCD1602显示数字函数(最大4位)
*******参量定义：	hang：输入显示行数  1 第一行  2 第二行
*******           add：偏移量，0-15
*******           num：所要显示的数据，最大4位
*****/
void lcd1602_display_num(uchar hang, uchar add, uint num)
{
	if(hang==1)   
		lcd1602_write_com(0x80+add);				//第一行
	else
		lcd1602_write_com(0x80+0x40+add);		//第二行

	if(num > 999)
		lcd1602_write_date(num/1000%10+48);	//千
	if(num > 99)
		lcd1602_write_date(num/100%10+48);	//百
	if(num > 9)
		lcd1602_write_date(num/10%10+48);		//十
	lcd1602_write_date(num/1%10+48);		//个
	lcd1602_write_date(' ');
	lcd1602_write_date(' ');
	lcd1602_write_date(' ');
}

/****
*******LCD1602清屏函数 
*****/
void lcd1602_clean()
{
	lcd1602_display_str(1,0,"                        ");
	lcd1602_display_str(2,0,"                        ");
}

/****
*******LCD1602初始化函数 
*****/
void Lcd1602_Init()
{
	lcd1602_write_com(0x38);
	lcd1602_write_com(0x0c);
	lcd1602_write_com(0x06);
	lcd1602_write_com(0x01);
}

