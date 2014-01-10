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
	DDRB = 0xff;
	
	//PD2、PD3 作为输出口，控制旋转开关
	DDRD &= ( ~(1<<PD2) & ~(1<<PD3) );
	PORTD |= (1<<PD2) | (1<<PD3);
	
	//PC6、PC7 作为输出口，控制洒水装置
	DDRC &= ( ~(1<<PC6) & ~(1<<PC7) );
	PORTC |= (1<<PC6) | (1<<PC7);
}


enum WORK_MODE read_work_mode()
{
	uchar mode = 0;
	PORTD &= (~(1<<PD3));
	PORTD |= (1<<PD2);
	
	//PIND3 PIND2 PIND1 PIND0
	//  mode = ~(PIND0 PIND1 PIND2 PIND3)
	mode = ((~PIND) & 0x0f);
	mode = ((mode&(1<<PIND3))>>3) | ((mode&(1<<PIND2))>>1) | ((mode&(1<<PIND1))<<1) | ((mode&(1<<PIND2))<<3); 
	
	return (enum WORK_MODE)(mode-1);
}

uchar read_time_set(enum WORK_MODE mode)
{
	uchar option = 0;
	PORTD &= (~(1<<PD2));
	PORTD |= (1<<PD3);
		
	//PIND3 PIND2 PIND1 PIND0
	//  mode = ~(PIND0 PIND1 PIND2 PIND3)
	option = ((~PIND) & 0x0f);
	option = ((option&(1<<PIND3))>>3) | ((option&(1<<PIND2))>>1) | ((option&(1<<PIND1))<<1) | ((option&(1<<PIND0))<<3);
		
	return time_option[mode][option-1];
}

//开启工作状态，控制洒水
void start_work_mode(enum WORK_MODE mode)
{
	//大巷模式下，工作状态不洒水
	if (mode == WORK_MODE_DAXIANG) {
		
	} else {    //令两种模式，工作状态为洒水
		
	}
}

//关闭工作状态，控制洒水
void stop_work_mode(enum WORK_MODE mode)
{
	//大巷模式下，非工作状态下洒水
	if (mode == WORK_MODE_DAXIANG) {
		
	} else {    //另两种模式，非工作状态不洒水
	
	}	
}

//检查是否有信号来，不同的工作模式下检查的传感器不一样
uchar check_signal(enum WORK_MODE mode)
{
	switch (mode) {
		
		case WORK_MODE_DAXIANG:
			
			break;
		
		case WORK_MODE_ZHUANGZAI:
		
			break;
			
		case WORK_MODE_WENKONG:
		
			break;
	}
	return 0;	
} 