/*
 * port.c
 *
 * Created: 2014-01-10 9:54:54 AM
 *  Author: guohailin
 */ 

#include "sysdef.h"
#include <avr/io.h>

//系统当前状态
enum SYSTEM_STATE cur_state;

//系统所处工作模式
enum WORK_MODE  work_mode;

//时间设置
//	每种工作模式下对应9种时间选择
uchar time_option[WORK_MODE_LAST][9] = {
	{20, 40, 60, 80, 100, 120, 140, 160, 180},
	{3,  4,  5,  6,  7,   8,   9,   10,  11 },
	{20, 40, 60, 80, 100, 120, 140, 160, 180}
};

void port_init()
{
	//PORTB 作为输入口，接收旋转开关和外部信号的输入
	DDRB = 0x00;
	
	//PD2、PD3 作为输出口，控制旋转开关
	DDRD  |= ( (1<<PD2) | (1<<PD3) );
	PORTD |= ( (1<<PD2) | (1<<PD3) );
	
	//PC6、PC7 作为输出口，控制洒水装置
	DDRC  &= ( ~(1<<PC6) & ~(1<<PC7) );
	PORTC |= ( (1<<PC6) | (1<<PC7) );
}


enum WORK_MODE read_work_mode()
{
	uchar mode = 0;
	PORTD &= (~(1<<PD3));
	PORTD |= (1<<PD2);
	
	//PINB3 PINB2 PINB1 PINB0
	//  moBe = ~(PINB0 PINB1 PINB2 PINB3)
	mode = ((~PINB) & 0x0f);
	mode = ((mode&(1<<PINB3))>>3) | ((mode&(1<<PINB2))>>1) | ((mode&(1<<PINB1))<<1) | ((mode&(1<<PINB0))<<3); 
	
	return (enum WORK_MODE)(mode-1);
}

uchar read_time_set(enum WORK_MODE mode)
{
	uchar option = 0;
	PORTD &= (~(1<<PD2));
	PORTD |= (1<<PD3);
		
	//PINB3 PINB2 PINB1 PINB0
	//  moBe = ~(PINB0 PINB1 PINB2 PINB3)
	option = ((~PINB) & 0x0f);
	option = ((option&(1<<PINB3))>>3) | ((option&(1<<PINB2))>>1) | ((option&(1<<PINB1))<<1) | ((option&(1<<PINB0))<<3);
		
	return time_option[mode][option-1];
}

//开启工作状态，控制洒水
//PC6 / PC7 控制洒水，0 为洒水， 1 为不洒水
void start_work_mode(enum WORK_MODE mode)
{
	//大巷模式下，工作状态不洒水
	if (mode == WORK_MODE_DAXIANG) {
		PORTC |= ( (1<<PC6) | (1<<PC7) );
	} else {    //令两种模式，工作状态为洒水
		PORTC &= ( ~(1<<PC6) & ~(1<<PC7) );
	}
	
	//设置工作画面
	show_work_image(work_mode, time_count);
}

//关闭工作状态，控制洒水
void stop_work_mode(enum WORK_MODE mode)
{
	//大巷模式下，非工作状态下洒水
	if (mode == WORK_MODE_DAXIANG) {
		PORTC &= ( ~(1<<PC6) & ~(1<<PC7) );
	} else {    //另两种模式，非工作状态不洒水
		PORTC |= ( (1<<PC6) | (1<<PC7) );
	}	
	
	//设置工作结束的画面
	show_work_image(work_mode, time_count);
}

//检查是否有信号来，不同的工作模式下检查的传感器不一样
//大巷模式下，检查 PB4 和 PB5 口，若任意一个口接收到信号（变为0）则切换到工作模式
//装载点模式，检查 PB6 口
//温控模式，检查 PB7 口
uchar check_signal(enum WORK_MODE mode)
{
	switch (mode) {

		case WORK_MODE_DAXIANG:
			//ustra_transmit(0x58);
			return !(((PINB>>PINB4) & 0x01) && ((PINB>>PINB5) & 0x01));
		
		case WORK_MODE_ZHUANGZAI:
			//ustra_transmit(!((PINB>>PINB6) & 0x01));
			return !((PINB>>PINB6) & 0x01);
			
		case WORK_MODE_WENKONG:
			return !((PINB>>PINB7) & 0x01);
	}
} 