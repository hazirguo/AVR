/*
 * main.c
 *
 * Created: 2014-01-09 11:23:03 PM
 *  Author: guohailin
 */ 


#include <avr/io.h>
#include <avr/interrupt.h>
#include "sysdef.h"
#include <util/delay.h>

//设置的工作时间
uchar time_set;

int main(void)
{
	//系统初始化
	//1.GPIO口初始化
	port_init();
	
	//2.计时器初始化
	timer_init();
	
	//3.串口初始化
	ustra_init();
	
	//4.中断初始化，关中断
	cli();
	
	//开机显示
	cur_state = SYS_START;
	show_start_image();
	
	//读取工作模式
	work_mode = read_work_mode();
	
	//读取时间设置
	time_set = read_time_set(work_mode);

	//开中断，进入过渡状态
	cur_state = SYS_INTERIM;
	time_count = FIVE_MININUTES;
	stop_work_mode(work_mode);

	sei();
	
    while(1)
    {	
		//检查是否有信号来
		//	有信号，进入工作模式
        if ((uchar)((uchar)(cur_state != SYS_WORK) && ((uchar)check_signal(work_mode)))) {		//?不强制类型转化会出错！！！why?!!!
			cur_state = SYS_WORK;			//设置系统当前状态为工作状态
			time_count = time_set;			//重置时间
			start_work_mode(work_mode);		//设置系统开始工作
		} 
		
		//无信号，继续检查
    }
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