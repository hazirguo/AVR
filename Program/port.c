/*
 * port.c
 *
 * Created: 2014-01-10 9:54:54 AM
 *  Author: guohailin
 */ 

#include "sysdef.h"
#include <avr/io.h>

//ϵͳ��ǰ״̬
enum SYSTEM_STATE cur_state;

//ϵͳ��������ģʽ
enum WORK_MODE  work_mode;

//ʱ������
//	ÿ�ֹ���ģʽ�¶�Ӧ9��ʱ��ѡ��
uchar time_option[WORK_MODE_LAST][9] = {
	{20, 40, 60, 80, 100, 120, 140, 160, 180},
	{3,  4,  5,  6,  7,   8,   9,   10,  11 },
	{20, 40, 60, 80, 100, 120, 140, 160, 180}
};


void port_init()
{
	//PORTB ��Ϊ����ڣ�������ת���غ��ⲿ�źŵ�����
	DDRB = 0xff;
	
	//PD2��PD3 ��Ϊ����ڣ�������ת����
	DDRD &= ( ~(1<<PD2) & ~(1<<PD3) );
	PORTD |= (1<<PD2) | (1<<PD3);
	
	//PC6��PC7 ��Ϊ����ڣ�������ˮװ��
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

//��������״̬��������ˮ
void start_work_mode(enum WORK_MODE mode)
{
	//����ģʽ�£�����״̬����ˮ
	if (mode == WORK_MODE_DAXIANG) {
		
	} else {    //������ģʽ������״̬Ϊ��ˮ
		
	}
}

//�رչ���״̬��������ˮ
void stop_work_mode(enum WORK_MODE mode)
{
	//����ģʽ�£��ǹ���״̬����ˮ
	if (mode == WORK_MODE_DAXIANG) {
		
	} else {    //������ģʽ���ǹ���״̬����ˮ
	
	}	
}

//����Ƿ����ź�������ͬ�Ĺ���ģʽ�¼��Ĵ�������һ��
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