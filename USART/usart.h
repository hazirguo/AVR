/*
 * usart.h
 *
 * Created: 2013-12-26 4:30:01 PM
 *  Author: guohailin
 */ 

#ifndef USART_H_
#define USART_H_

typedef unsigned int	uint;
typedef unsigned char	uchar;

//	BAUD = fosc/16(UBRR+1)
//  =>  UBRR = fosc/16BAUD - 1
#define FOSC	11059200			//clock speed
#define BAUD	9600				//baud
#define MYUBRR	FOSC/16/BAUD - 1	//ubrr

//initialize USART
void USART_Init(uint ubrr);

//receive data and return the received data
uchar USART_Receive(void);

//trasmit data(8 bits data)
void USART_Transmit(uchar data);

#endif /* USART_H_ */