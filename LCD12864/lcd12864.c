/*
 * lcd12864.c
 *
 * Created: 2013-12-26 7:10:23 PM
 *  Author: guohailin
 */ 

#include <avr/io.h>
#include <util/delay.h>
#include "lcd12864.h"

//initialize lcd 12864 port
//	1. data port---D0~D7
//	2. control port---RS/RW/E/PSB
void LCD12864Port_Init()
{
	PORTB = 0x00;
	DDRB = 0xFF;    //输出
	
	PORTA &= ~((1<<RS) | (1<<RW) | (1<<E));    //0
	PORTA |= (1<<PSB);
	DDRA |= (1<<RS) | (1<<RW) | (1<<E) | (1<<PSB);		//1, 输出
}

//initialize lcd 12864
//	clear screen, set display model, etc.
void LCD12864_Init()
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
	
	//0(0x80)-->2-->1-->3 
	switch(x)
	{
		case 0:
			address = 0x80;
			break;
		case 1:
			address = 0x90;
			break;
		case 2:
			address = 0x88;
			break;
		case 3:
			address = 0x98;
			break;
		default:
			address = 0x80;
			break;		
	}
	
	address += x;
	
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

