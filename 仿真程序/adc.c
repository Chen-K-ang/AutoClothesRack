/**********************************
����ͷ�ļ�
**********************************/
#include "adc.h"


/**********************************
��������
**********************************/
/****
*******��ȡadcֵ����
*******�������壺CH:ͨ����  0��ͨ��0   1��ͨ��1
*******����ֵ��adcֵ
*****/
uint Adc0832_Get_Value(uchar CH)
{
	uint dat = 0x00; 
	uchar i,test,adval;
	adval = 0x00;
	test = 0x00;

	//��ʼ��
	ADC_CLK = 0;      
	ADC_DATI = 1;
	_nop_();   _nop_();
	ADC_CS = 0;
	_nop_();
	ADC_CLK = 1;
	_nop_();  _nop_();

	//ͨ��ѡ��
	if(CH == 0x00)     
	{
		ADC_CLK = 0;
		ADC_DATI = 1;      					//ͨ��0�ĵ�һλ
		_nop_();
		ADC_CLK = 1;
		_nop_();  _nop_();

		ADC_CLK = 0;
		ADC_DATI = 0;      					//ͨ��0�ĵڶ�λ
		_nop_();  _nop_();

		ADC_CLK = 1;
		_nop_();
	} 
	else
	{
		ADC_CLK = 0;
		ADC_DATI = 1;      					//ͨ��1�ĵ�һλ
		_nop_();  _nop_();
		ADC_CLK = 1;
		_nop_();  _nop_();

		ADC_CLK = 0;
		ADC_DATI = 1;      					//ͨ��1�ĵڶ�λ
		_nop_();
		ADC_CLK = 1;
		_nop_();
	}

	ADC_CLK = 0;   _nop_();
	ADC_DATI = 1;

	for( i = 0;i < 8;i++ )      	//��ȡǰ8λ��ֵ
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
	for (i = 0; i < 8; i++)       //��ȡ��8λ��ֵ
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

	//�Ƚ�ǰ8λ���8λ��ֵ���������ͬ��ȥ����һֱ������ʾΪ�㣬�뽫����ȥ��
	if (adval == test)     
		dat = test;
	_nop_();  _nop_();
	ADC_CS = 1;        						//�ͷ�ADC0832
	ADC_DATO = 1;
	ADC_CLK = 1;

	return dat;
}

