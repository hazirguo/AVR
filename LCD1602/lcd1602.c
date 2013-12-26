/*
 * lcd1602.c
 *
 * Created: 2013-12-26 4:54:13 PM
 *  Author: guohailin
 */ 

#include <avr/io.h>
#include <util/delay.h>
#include "lcd1602.h"

//initialize lcd 1602 port
//	1. data port---D0~D7
//	2. control port---RS/RW/E
void LCDPort_Init()
{
	//data port
	PORTB = 0x00;
	DDRB = 0xFF;    //输出
	
	PORTA &= ~((1<<RS) | (1<<RW) | (1<<E));    //0, disabled
	DDRA |= (1<<RS) | (1<<RW) | (1<<E);		//1, 输出
}

//initialize lcd 1602
//	clear screen, set display model, etc.
void LCD_Init()
{
	Write_Com(0x01);    //clear screen
	Delay_ms(5);
	Write_Com(0x38);	 //16*2 display model
	Delay_ms(5);
	//Write_Com(0x0f);	 //开显示，光标显示，光标闪烁
	Write_Com(0x0c);	 //开显示，光标不显示，光标不闪烁
	Delay_ms(5);
	Write_Com(0x06);	 //光标设置，读/写一个字符后，地址指针加1，光标加1，整屏不移动
	Delay_ms(5);
}

// write a command to lcd
void Write_Com(uchar LCD_Com)
{
	Check_Busy();
	
	PORTA &= ~(1<<RS);   //RS=0: 写命令
	PORTA &= ~(1<<RW);   //RW=0: 写指令
	PORTA |= (1<<E);	  //E=1:  写操作有效
	Delay_ms(15);
	PORTB = LCD_Com;	  //指令传送到数据端口
	PORTA &= ~(1<<E);    //E=0:  写操作停止
	Delay_ms(15);
}

// write a byte data to lcd
void Write_Data_Byte(uchar LCD_Data)
{
	Check_Busy();
	
	PORTA |= (1<<RS);	  //RS=1: 写数据
	PORTA &= ~(1<<RW);   //RW=0: 写指令
	PORTA |= (1<<E);     //E=1:  写操作有效
	Delay_ms(15);
	PORTB = LCD_Data;
	PORTA &= ~(1<<E);	  //E=0
	Delay_ms(15);
}

//set write place (x,y)
void Write_Place_xy(uchar x, uchar y)
{
	uchar address;
	
	if (y == 0)		//first line
		address = 0x80 + x;
	else
		address = 0x80 + 0x40 + x;
	
	Write_Com(address);
}

//write string from place (x,y)
void Write_Data_String(uchar x, uchar y, uchar *data)
{
	Write_Place_xy(x, y);
	while(*data)
	{
		Write_Data_Byte(*data);
		data++;
	}
}

//write a char data in place (x,y)
void Write_Data_Char(uchar x, uchar y, uchar data)
{
	Write_Place_xy(x, y);
	Write_Data_Byte(data);
}

//check busy
void Check_Busy()
{
	DDRB = 0x00;		 //读数据口
	PORTA &= ~(1<<RS);  //RS=0: 读命令
	PORTA |= (1<<RW);   //RW=1: 读指令
	PORTA |= (1<<E);	 //E=1:  读使能
	
	while(0x80 & PINB);
	
	PORTA &= ~(1<<E);   //E=0
	DDRB = 0xFF;
}

//dalay us
void Delay_us(unsigned int us)
{
	while(us--)
		_delay_loop_2(4);
}

//delay ms
void Delay_ms(unsigned int ms)
{
	while(ms--)
		Delay_us(1000);
}

