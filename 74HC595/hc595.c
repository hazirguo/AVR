/*
 * _74hc595.c
 *
 * Created: 2013-12-26 10:29:30 PM
 *  Author: guohailin
 */ 

#include "hc595.h"

uchar LedData[]= {0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xF8,
                        0x80, 0x90, 0x88, 0x83, 0xc6, 0xa1, 0x86, 0x8e};	//共阳数码管段选数组（0-F）
uchar LedPos[] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80};	//数码管位选数组，分别选通每一位数码管


//initialize 74hc595 port
void HC595Port_Init(void)
{
	DDRB |= (1 << STCP) | (1 << DS) | (1 << SHCP);
	PORTB &= ~( (1<<STCP) | (1 << DS) | (1 << SHCP) );
}

//write a byte
void HC595_Send(uchar data)
{
	uchar i, temp;
	for (i=0; i<8; i++)		//一个字节需要分8次写入
	{
		temp = data & 0x80;
		if (temp)
			PORTB |= (1<<DS);
		else
			PORTB &= ~(1<<DS);
		HC59_Shift();
		data <<= 1;
	}
}

//shift
void HC59_Shift()
{
	PORTB |= (1<<SHCP);
	PORTB &= ~(1<<SHCP);
}

//show
void HC595_Store()
{
	PORTB |= (1<<STCP);
	PORTB &= ~(1<<STCP);
}

//display data to the posth led
void HC595_Display(uchar pos, uchar data)
{
	HC595_Send(LedPos[pos]);
	HC595_Send(LedData[data]);
	HC595_Store();
}

