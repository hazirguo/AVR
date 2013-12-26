/*
 * ds18b20.c
 *
 * Created: 2013-12-26 8:18:04 PM
 *  Author: guohailin
 */ 
#include "ds18b20.h"
#include <avr/interrupt.h>

//initialize ds18b20 port
void DS18b20Port_Init()
{
	SET_IN;
	CLR_DQ;	
}

//reset ds18b20
uchar DS18b20_Init()
{
	uchar OK_Flag;
	SET_OUT;
	Delay_us(490);
	SET_IN;
	Delay_us(68);
	
	if (DQ_IN)	
		OK_Flag = 0;
	else
		OK_Flag = 1;
	
	Delay_us(422);
	return OK_Flag;
}

//read ds18b20
uchar Read_18b20()
{
	uchar i;
	uchar data = 0;
	
	for (i=0; i<8; i++)
	{
		SET_OUT;
		Delay_us(2);
		SET_IN;
		Delay_us(4);
		data >>= 1;
		
		if (DQ_IN)
			data |= 0x80;
		Delay_us(62);		
	}	
	
	return data;
}

//write ds18b20
void Write_18b20(uchar data)
{
	uchar i;
	for (i=0; i<8; i++)
	{
		SET_OUT;
		Delay_us(2);
		if (data & 0x01)
			SET_IN;
		else
			SET_OUT;	
		data >>= 1;
		Delay_us(62);
		
		SET_IN;
		Delay_us(2);	
	}	
}

//read temperature
void Read_Temperature(int *temp_Int, int *temp_Float)
{
	uchar i;
	uchar temp_L, temp_H;
	cli();
	DS18b20_Init();
	Write_18b20(0xcc);		//发送ROM指令，跳过ROM匹配
	Write_18b20(0x44);		//发送温度转化指令
	sei();
	
	for(i=0; i<50; i++)		//延时1s，等待转换完成
		Delay_ms(20);	
	
	cli();
	
	DS18b20_Init();
	Write_18b20(0xcc);
	Write_18b20(0xbe);			//发送读取暂存器指令
	
	temp_L = Read_18b20();		//温度的低位
	temp_H = Read_18b20();		//      高
	
	USART_Transmit(temp_L);
	USART_Transmit(temp_H);
	
	sei();
	
	if(temp_H & 0x80)		//温度为负
	{
		temp_H = ~temp_H;
		temp_L = ~temp_L;
		SREG |= ~(1<<SREG_C);  //清除进位位标志
		temp_L++;
		if (SREG & (1<<SREG_C))
			temp_H++;
	}	
			
	*temp_Int = ((temp_H << 4) & 0x70) | (temp_L >> 4);    //获得温度的整数值
		
	*temp_Float = temp_L & 0x0f;
	*temp_Float *= 625;							//*0.0625，这里扩大10000倍
}