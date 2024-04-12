/**********************************
包含头文件
**********************************/
#include "adc.h"


/**********************************
函数定义
**********************************/
/****
*******获取adc值函数
*******参量定义：CH:通道数  0：通道0   1：通道1
*******返回值：adc值
*****/
uint Adc0832_Get_Value(uchar CH)
{
	uint dat = 0x00; 
	uchar i,test,adval;
	adval = 0x00;
	test = 0x00;

	//初始化
	ADC_CLK = 0;      
	ADC_DATI = 1;
	_nop_();   _nop_();
	ADC_CS = 0;
	_nop_();
	ADC_CLK = 1;
	_nop_();  _nop_();

	//通道选择
	if(CH == 0x00)     
	{
		ADC_CLK = 0;
		ADC_DATI = 1;      					//通道0的第一位
		_nop_();
		ADC_CLK = 1;
		_nop_();  _nop_();

		ADC_CLK = 0;
		ADC_DATI = 0;      					//通道0的第二位
		_nop_();  _nop_();

		ADC_CLK = 1;
		_nop_();
	} 
	else
	{
		ADC_CLK = 0;
		ADC_DATI = 1;      					//通道1的第一位
		_nop_();  _nop_();
		ADC_CLK = 1;
		_nop_();  _nop_();

		ADC_CLK = 0;
		ADC_DATI = 1;      					//通道1的第二位
		_nop_();
		ADC_CLK = 1;
		_nop_();
	}

	ADC_CLK = 0;   _nop_();
	ADC_DATI = 1;

	for( i = 0;i < 8;i++ )      	//读取前8位的值
	{
		_nop_();
		adval <<= 1;
		ADC_CLK = 1;
		_nop_();  _nop_();

		ADC_CLK = 0;
		_nop_();

		if (ADC_DATO)
			adval |= 0x01;
		else
			adval |= 0x00;
	}
	for (i = 0; i < 8; i++)       //读取后8位的值
	{
		test >>= 1;
		if (ADC_DATO)
			test |= 0x80;
		else 
			test |= 0x00;
		_nop_();
		ADC_CLK = 1;
		_nop_();  _nop_();

		ADC_CLK = 0;
		_nop_();

	}

	//比较前8位与后8位的值，如果不相同舍去。若一直出现显示为零，请将该行去掉
	if (adval == test)     
		dat = test;
	_nop_();  _nop_();
	ADC_CS = 1;        						//释放ADC0832
	ADC_DATO = 1;
	ADC_CLK = 1;

	return dat;
}

