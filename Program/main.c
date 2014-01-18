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

//���õĹ���ʱ��
uchar time_set;

int main(void)
{
	//ϵͳ��ʼ��
	//1.GPIO�ڳ�ʼ��
	port_init();
	
	//2.��ʱ����ʼ��
	timer_init();
	
	//3.���ڳ�ʼ��
	ustra_init();
	
	//4.�жϳ�ʼ�������ж�
	cli();
	
	//������ʾ
	cur_state = SYS_START;
	show_start_image();
	
	//��ȡ����ģʽ
	work_mode = read_work_mode();
	
	//��ȡʱ������
	time_set = read_time_set(work_mode);

	//���жϣ��������״̬
	cur_state = SYS_INTERIM;
	time_count = FIVE_MININUTES;
	stop_work_mode(work_mode);

	sei();
	
    while(1)
    {	
		//����Ƿ����ź���
		//	���źţ����빤��ģʽ
        if ((uchar)((uchar)(cur_state != SYS_WORK) && ((uchar)check_signal(work_mode)))) {		//?��ǿ������ת�����������why?!!!
			cur_state = SYS_WORK;			//����ϵͳ��ǰ״̬Ϊ����״̬
			time_count = time_set;			//����ʱ��
			start_work_mode(work_mode);		//����ϵͳ��ʼ����
		} 
		
		//���źţ��������
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