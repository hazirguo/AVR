/*
 * ds18b20.h
 *
 * Created: 2013-12-26 6:41:58 PM
 *  Author: guohailin
 */ 


#ifndef DS18B20_H_
#define DS18B20_H_
#include <avr/io.h>
#include <util/delay.h>

typedef unsigned char	uchar;
typedef unsigned int	uint;

//DS12B20 macro define
#define SET_DQ	(PORTC |= (1<<PC7))		//���� ����ߵ�ƽ			MCU 1-> DS12B20
#define CLR_DQ	(PORTC &= ~(1<<PC7))	//���� ����͵�ƽ			MCU 0-> DS12B20
#define DQ_IN	(PINC & (1<<PC7))		//����DS12B20����ź�		MCU <-0?1? DS12B20
	
#define SET_OUT	(DDRC |= (1<<PC7))		//��������Ϊ �����		MCU -> DS12B20
#define SET_IN	(DDRC &= ~(1<<PC7))		//��������Ϊ �����		MCU <- DS12B20
	
//initialize DS18B20 PORT
void DS18b20Port_Init(void);

uchar DS18b20_Init(void);

uchar Read_18b20(void);
void Write_18b20(uchar data);

#endif /* DS18B20_H_ */