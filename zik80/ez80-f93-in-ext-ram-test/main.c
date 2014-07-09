#include <eZ80.h>
#include <uart.h>
#include <gpio.h>

static void delay_loop ( unsigned int c ) {
	while ( c ) {
		c--;
	}
}

int main ( ) {
	
#if 0 // uart logger
	// F93 default is 20MHz - happens to be right for us
	// uart0 is on port d
	// uart1 is on port c
	
	//UCHAR open_UARTx(UART * pUART);
	
	UART u0;
	char b [ 10 ];
	unsigned char i;
	
	u0.uartMode = POLL; //INTERRUPT;
	u0.baudRate =  BAUD_38400;
	u0.dataBits = DATABITS_8;
	u0.stopBits = STOPBITS_1;
	u0.parity = PAR_NOPARITY;
	u0.fifoTriggerLevel = FIFO_TRGLVL_1;
	u0.hwFlowControl = DISABLE_HWFLOW_CONTROL;
	u0.swFlowControl = DISABLE_SWFLOW_CONTROL;
	
	open_UART0 ( &u0 );
	
	b [ 0 ] = 'A';
	b [ 1 ] = 'B';
	b [ 2 ] = 'C';
	b [ 3 ] = '\0';
	
	for ( i = 0; i < 5; i++ ) {
		write_UART0 ( b, 3 );
	}
#endif
	
#if 1 // blinker
	PORT pc;
	UCHAR err;
	unsigned char *extram;
	//unsigned char i;
	
	pc.dr = 0x01;
	pc.ddr = 0; // 0 ddr is output
	//pc.alt0 = 0; // does not exist for F93
	pc.alt1 = 0;
	pc.alt2 = 0;
	
	open_PortC ( &pc );
	//control_PortC ( & pc);

	err = setmode_PortC ( PORTPIN_ZERO, GPIOMODE_OUTPUT );
	
	SETDR_PORTC ( 1 );
	
	//SETDR_PORTC ( 0xFF );
	
	extram = (unsigned char *) 0xAA0000;
#if 0
	for ( i = 0; i < 20; i++ ) {
		*extram++ = i; //0x01;
	}
#endif
#if 1
	*extram = 0;
	extram++;
	*extram = 1;
	extram++;
	*extram = 2;
	extram++;
	*extram = 3;
	extram++;
	*extram = 4;
	extram++;
	*extram = 5;
	extram++;
	*extram = 6;
	extram++;
#endif
	
	
	while ( 1 ) {
		delay_loop ( 15 );
		PC_DR ^= 1;
		//SETDR_PORTC ( 1 );
	}
	
#endif

	//return ( 0 );
}
