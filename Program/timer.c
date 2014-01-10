/*
 * timer.c
 *
 * Created: 2014-01-10 10:27:29 AM
 *  Author: guohailin
 */ 

#include "sysdef.h"
#include <avr/interrupt.h>


//倒计时计数，单位为 s
uchar time_count;

void timer_init()
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


//时间中断, 1ms 进入中断
ISR(TIMER0_COMP_vect)
{
	static uint i = 0;
	i++;
	
	// 1 s
	if (i >= 1000) {
		
		i = 0;
		time_count--;
		
		switch(cur_state) {
			
			case SYS_WORK:		//工作状态
			//显示倒计时画面
			show_work_image(work_mode, time_count);
			
			//倒计时结束
			if(time_count == 0) {
				stop_work_mode(work_mode);		//停止工作状态
				cur_state = SYS_INTERIM;
				time_count = FIVE_MININUTES;	//五分钟倒计时
			}
			break;
			
			case SYS_INTERIM:	//过渡状态
			if(time_count == 0) {
				cur_state = SYS_SCREEN;
				//显示屏保画面
				show_screen_image();
				time_count = FIVE_SECONDS;		//五秒钟倒计时
			}
			break;
			
			case SYS_SCREEN:	//屏保状态
			if(time_count == 0) {
				//显示下一张屏保图片
				show_screen_image();
				time_count = FIVE_SECONDS;
			}
			break;
			
		} //end of 'switch(cur_state)'...
	} //end of 'if(i>=1000)'...
}
