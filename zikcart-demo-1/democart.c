#include <eZ80.h>
#include <uart.h>
#include <gpio.h>
#include <stdlib.h>
#include <string.h>

int main ( ) {

	// enable interupts
	_init_default_vectors();

#if 1 // uart logger - WORKS
	{
		// F93 default is 20MHz - happens to be right for us
		// uart0 is on port d
		// uart1 is on port c
		
		UART u0;
		char b [ 16 ];
		unsigned char i;
		
		u0.uartMode = POLL; //INTERRUPT; //POLL; //INTERRUPT;
		u0.baudRate =  BAUD_38400; //BAUD_57600; //BAUD_115200; //BAUD_38400;
		u0.dataBits = DATABITS_8;
		u0.stopBits = STOPBITS_1;
		u0.parity = PAR_NOPARITY;
		u0.fifoTriggerLevel = FIFO_TRGLVL_1;
		u0.hwFlowControl = DISABLE_HWFLOW_CONTROL;
		u0.swFlowControl = DISABLE_SWFLOW_CONTROL;
		
		open_UART0 ( &u0 );
		
		b [ 0 ] = 'D';
		b [ 1 ] = 'R';
		b [ 2 ] = 'P';
		b [ 3 ] = '\0';
		
		for ( i = 0; i < 5; i++ ) {
			write_UART0 ( b, 3 );
		}
		
		lame_itoa ( (int)2, b );
		
		write_UART0 ( b, lame_strlen ( b ) );
		write_UART0 ( "\n", 1 );
		flush_UART0 ( FLUSHFIFO_ALL );
		
	}

#endif

}
