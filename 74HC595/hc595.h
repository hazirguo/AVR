/*
 * _74hc595.h
 *
 * Created: 2013-12-26 10:20:36 PM
 *  Author: guohailin
 */ 


#ifndef HC595_H_
#define HC595_H_

#include <avr/io.h>

#define STCP	PB4
#define DS		PB5
#define SHCP	PB7

typedef unsigned char	uchar;
typedef unsigned int	uint;

//initialize 74hc595 port
void HC595Port_Init(void);

//write a byte
void HC595_Send(uchar data);

//shift
void HC59_Shift();

//show
void HC595_Store();

//display data to the posth led
void HC595_Display(uchar pos, uchar data);


#endif /* 74HC595_H_ */