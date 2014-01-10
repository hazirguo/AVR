/*
 * timer.c
 *
 * Created: 2014-01-10 10:27:29 AM
 *  Author: guohailin
 */ 

#include "sysdef.h"
#include <avr/interrupt.h>


//����ʱ��������λΪ s
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


//ʱ���ж�, 1ms �����ж�
ISR(TIMER0_COMP_vect)
{
	static uint i = 0;
	i++;
	
	// 1 s
	if (i >= 1000) {
		
		i = 0;
		time_count--;
		
		switch(cur_state) {
			
			case SYS_WORK:		//����״̬
			//��ʾ����ʱ����
			show_work_image(work_mode, time_count);
			
			//����ʱ����
			if(time_count == 0) {
				stop_work_mode(work_mode);		//ֹͣ����״̬
				cur_state = SYS_INTERIM;
				time_count = FIVE_MININUTES;	//����ӵ���ʱ
			}
			break;
			
			case SYS_INTERIM:	//����״̬
			if(time_count == 0) {
				cur_state = SYS_SCREEN;
				//��ʾ��������
				show_screen_image();
				time_count = FIVE_SECONDS;		//�����ӵ���ʱ
			}
			break;
			
			case SYS_SCREEN:	//����״̬
			if(time_count == 0) {
				//��ʾ��һ������ͼƬ
				show_screen_image();
				time_count = FIVE_SECONDS;
			}
			break;
			
		} //end of 'switch(cur_state)'...
	} //end of 'if(i>=1000)'...
}
