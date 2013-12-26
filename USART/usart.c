/*
 * usart.c
 *
 * Created: 2013-12-26 4:28:59 PM
 *  Author: guohailin
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include "usart.h"

//initialize USART
void USART_Init(uint ubrr)
{
	//set baud
	UBRRH = (uchar)(ubrr >> 8);
	UBRRL = (uchar)ubrr;
	
	//UCSRA
	//    RXC   TXC   UDRE    FE    DOR   PE   U2X   MPCM
	
	//UCSRB
	//	 RXCIE  TXCIE   UDRIE   RXEN  TXEN  UCSZ2  RXB8  TXB8
	UCSRB = (1 << RXCIE) | (1 << RXEN) | (1 << TXEN);        //Receive & Transmit Enabled, RX interrupted enabled
	
	//UCSRC
	//   URSEL  UMSEL  UPM1  UPM0   USBS   UCSZ1   UCSZ0   UCPOL
	UCSRC = (1 << URSEL) | (3 << UCSZ0);      // 1 start_bit, 8 data_bits, 1 stop_bit
}

//receive data
//return the received data
uchar USART_Receive(void)
{
	//wait for data to be received
	while ( !(UCSRA & (1 << RXC)) )
		;
	
	//get the data and return
	return UDR;
}

//trasmit data(8 bits data)
void USART_Transmit(uchar data)
{
	//wait for empty transmit buffer
	while( !(UCSRA & (1<<UDRE)) )
		;
	
	//put data into buffer and send
	UDR = data;
}

//receive data compelte interrupt vector
ISR(USART_RXC_vect)
{
	uchar data;
	//wait for data to be received
	while ( !(UCSRA & (1 << RXC)) )
	;
	data = UDR;
	
	//to do sth.
	//.......
}
