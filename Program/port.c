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
	DDRB = 0x00;
	
	//PD2��PD3 ��Ϊ����ڣ�������ת����
	DDRD  |= ( (1<<PD2) | (1<<PD3) );
	PORTD |= ( (1<<PD2) | (1<<PD3) );
	
	//PC6��PC7 ��Ϊ����ڣ�������ˮװ��
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

//��������״̬��������ˮ
//PC6 / PC7 ������ˮ��0 Ϊ��ˮ�� 1 Ϊ����ˮ
void start_work_mode(enum WORK_MODE mode)
{
	//����ģʽ�£�����״̬����ˮ
	if (mode == WORK_MODE_DAXIANG) {
		PORTC |= ( (1<<PC6) | (1<<PC7) );
	} else {    //������ģʽ������״̬Ϊ��ˮ
		PORTC &= ( ~(1<<PC6) & ~(1<<PC7) );
	}
	
	//���ù�������
	show_work_image(work_mode, time_count);
}

//�رչ���״̬��������ˮ
void stop_work_mode(enum WORK_MODE mode)
{
	//����ģʽ�£��ǹ���״̬����ˮ
	if (mode == WORK_MODE_DAXIANG) {
		PORTC &= ( ~(1<<PC6) & ~(1<<PC7) );
	} else {    //������ģʽ���ǹ���״̬����ˮ
		PORTC |= ( (1<<PC6) | (1<<PC7) );
	}	
	
	//���ù��������Ļ���
	show_work_image(work_mode, time_count);
}

//����Ƿ����ź�������ͬ�Ĺ���ģʽ�¼��Ĵ�������һ��
//����ģʽ�£���� PB4 �� PB5 �ڣ�������һ���ڽ��յ��źţ���Ϊ0�����л�������ģʽ
//װ�ص�ģʽ����� PB6 ��
//�¿�ģʽ����� PB7 ��
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