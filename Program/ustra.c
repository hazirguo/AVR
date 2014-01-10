/*
 * ustra.c
 *
 * Created: 2014-01-10 10:20:46 AM
 *  Author: guohailin
 */ 
#include "sysdef.h"
#include <avr/io.h>

// BAUD = fosc/16(UBRR+1)
//  =>  UBRR = fosc/16BAUD - 1
#define FOSC        11059200                //clock speed
#define BAUD        9600                    //baud
#define MYUBRR      FOSC/16/BAUD - 1        //ubrr

void ustra_init()
{
	//set baud  9600
	UBRRH = (uchar)(MYUBRR >> 8);
	UBRRL = (uchar)MYUBRR;
	
	//UCSRA
	//    RXC   TXC   UDRE    FE    DOR   PE   U2X   MPCM
	
	//UCSRB
	//	 RXCIE  TXCIE   UDRIE   RXEN  TXEN  UCSZ2  RXB8  TXB8
	UCSRB = (1 << RXEN) | (1 << TXEN);        //Receive & Transmit Enabled, RX interrupted enabled
	
	//UCSRC
	//   URSEL  UMSEL  UPM1  UPM0   USBS   UCSZ1   UCSZ0   UCPOL
	UCSRC = (1 << URSEL) | (3 << UCSZ0);      // 1 start_bit, 8 data_bits, 1 stop_b
}

//receive data
//return the received data
uchar ustra_receive(void)
{
	//wait for data to be received
	while ( !(UCSRA & (1 << RXC)) )
		;
	
	//get the data and return
	return UDR;
}

//trasmit data(8 bits data)
void ustra_transmit(uchar data)
{
	//wait for empty transmit buffer
	while( !(UCSRA & (1<<UDRE)) )
		;
	
	//put data into buffer and send
	UDR = data;
}
