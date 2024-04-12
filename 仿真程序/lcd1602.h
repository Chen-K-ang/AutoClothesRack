/**********************************
作者：单片机俱乐部
网站：https://www.mcuclub.cn/
**********************************/


#ifndef _LCD1602_H_
#define _LCD1602_H_


/**********************************
包含头文件
**********************************/
#include "main.h"


/**********************************
PIN口定义
**********************************/
#define LCD1602_PORT P0      		 //数据总线P0.0-P0.7
sbit LCD1602_RS=P2^5;						 //数据/命令选择引脚P2.5
sbit LCD1602_RW=P2^6;						 //读/写选择P2.6
sbit LCD1602_E=P2^7;						 //使能信号引脚P2.7
sbit LCD1602_BUSY=P0^7;					 //检测状态引脚P0.7


/**********************************
函数声明
**********************************/
void lcd1602_display_str(uchar hang, uchar add, uchar *dat);					//LCD1602显示字符串函数
void lcd1602_display_time(uchar hang, uchar add, uint time);					//LCD1602显示时间函数(显示格式：**:**:**)
void lcd1602_display_temp(uchar hang, uchar add,uint temp);						//LCD1602显示温度函数(显示格式：**.*℃)
void lcd1602_display_humi(uchar hang, uchar add,uint humi);						//LCD1602显示湿度函数(显示格式：**.*%)
void lcd1602_display_light(uchar hang, uchar add,uint light);					//LCD1602显示光照强度函数(显示格式：**.*Lx)
void lcd1602_display_water(uchar hang, uchar add,uint water);						//LCD1602显示风速函数(显示格式：**.*m/s)
void lcd1602_display_num(uchar hang, uchar add,uint num);							//LCD1602显示数字函数(最大4位)
void lcd1602_clean();																									//LCD1602清屏函数
void Lcd1602_Init();																									//LCD1602初始化函数


#endif

