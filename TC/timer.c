/*
 * timer.c
 *
 * Created: 2014-01-03 8:13:13 PM
 *  Author: guohailin
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include "lcd12864.h"

typedef unsigned char uchar;

uchar hour = 0;
uchar min = 0;
uchar second = 0;

//init TC0 -- as a timer
void TC0_init()
{
	//Timer/Counter Control Register(TCCR0)
	//	FOC0	WGM00	COM01	COM00	WGM01	CS02	CS01	CS00
	//		FOC		-- Force Output Compare 
	//		WGM01:0	-- Waveform Generation Mode
	//		COM01:0	-- Compare Match Output Mode
	//		CS02:0	-- Clock Select
	TCCR0 = 0x0C;
	TCNT0 = 0x00;
	OCR0  = 43;		//1ms
	TIMSK = 0x02;	//enable t/c0 compare interrupt
}


ISR(TIMER0_COMP_vect)
{
	sei();					//allow interrupt nesting
	static int cnt = 0;
	cnt++;
	if (cnt >= 1000)		//1s
	{
		cnt = 0;
		PORTB = second;
		Update_Time();
		Display_Time();
	}
}

void Update_Time()
{
	second++;
	if(second >= 60){
		min++;
		second = 0;
		if(min >= 60) {
			min = 0;
			hour++;
			if (hour >= 60) {
				hour = 0;
			}
		}
	}		
}

void Display_Time()
{
	Write_Place_xy(3, 0);
	
	Write_Data_Byte(hour/10+'0');
	Write_Data_Byte(hour%10+'0');
	Write_Data_Byte(':');
	Write_Data_Byte(min/10+'0');
	Write_Data_Byte(min%10+'0');
	Write_Data_Byte(':');
	Write_Data_Byte(second/10+'0');
	Write_Data_Byte(second%10+'0');
}