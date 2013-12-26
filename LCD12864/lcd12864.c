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
	DDRB = 0xFF;    //���
	
	PORTA &= ~((1<<RS) | (1<<RW) | (1<<E));    //0
	PORTA |= (1<<PSB);
	DDRA |= (1<<RS) | (1<<RW) | (1<<E) | (1<<PSB);		//1, ���
}

//initialize lcd 12864
//	clear screen, set display model, etc.
void LCD12864_Init()
{
	Write_Com(0x01);    //clear screen
	Delay_ms(5);
	Write_Com(0x38);	 //16*2 display model
	Delay_ms(5);
	//Write_Com(0x0f);	 //����ʾ�������ʾ�������˸
	Write_Com(0x0c);	 //����ʾ����겻��ʾ����겻��˸
	Delay_ms(5);
	Write_Com(0x06);	 //������ã���/дһ���ַ��󣬵�ַָ���1������1���������ƶ�
	Delay_ms(5);
}

// write a command to lcd
void Write_Com(uchar LCD_Com)
{
	Check_Busy();
	
	PORTA &= ~(1<<RS);   //RS=0: д����
	PORTA &= ~(1<<RW);   //RW=0: дָ��
	PORTA |= (1<<E);	  //E=1:  д������Ч
	Delay_ms(15);
	PORTB = LCD_Com;	  //ָ��͵����ݶ˿�
	PORTA &= ~(1<<E);    //E=0:  д����ֹͣ
	Delay_ms(15);
}

// write a byte data to lcd
void Write_Data_Byte(uchar LCD_Data)
{
	Check_Busy();
	
	PORTA |= (1<<RS);	  //RS=1: д����
	PORTA &= ~(1<<RW);   //RW=0: дָ��
	PORTA |= (1<<E);     //E=1:  д������Ч
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
	DDRB = 0x00;		 //�����ݿ�
	PORTA &= ~(1<<RS);  //RS=0: ������
	PORTA |= (1<<RW);   //RW=1: ��ָ��
	PORTA |= (1<<E);	 //E=1:  ��ʹ��
	
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

