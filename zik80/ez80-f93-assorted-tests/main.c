// Chip Selects:
// CS0 e8 0 0c0000 0d0000 -> ram
// CS1 e8 0 1c0000 -> rom
// CS2 e8 0 2c0000 -> audio
//
// 2.457MHz oscilator in place

#include <eZ80.h>
#include <uart.h>
#include <gpio.h>

#include "skeelib.h"
#include <lib_ay-3-8912.h>

int main ( ) {

#if 1 // sleep for a few seconds up front, making emergency reflash easier (than racing against HALTs)
	{
		delay_ms_spin(1000);
		delay_ms_spin(1000);
	}
#endif
	
#if 0 // uart logger - WORKS
	{
		// F93 default is 20MHz - happens to be right for us
		// uart0 is on port d
		// uart1 is on port c
		
		//UCHAR open_UARTx(UART * pUART);
		
		UART u0;
		char b [ 16 ];
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
		
		lame_itoa ( (int) b, b );
		
		write_UART0 ( b, lame_strlen ( b ) );
		write_UART0 ( "\n", 1 );
		
	}
#endif
	
#if 0 // blinker - WORKS
	{
		PORT pc;
		UCHAR err;
		
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
		
		while ( 1 ) {
			delay_loop ( 15 );
			PC_DR ^= 1;
			//SETDR_PORTC ( 1 );
		}
	}		
#endif

#if 0 // ext ram check
	{
		unsigned char *extram;

		extram = (unsigned char *) 0x0C0000;
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

	}
#endif
	
#if 1 // ext audio AY-3-8912 test
	{
		
		PORT pc;
		UCHAR err;
		
		pc.dr = 0x01;
		pc.ddr = 0; // 0 ddr is output
		//pc.alt0 = 0; // does not exist for F93
		pc.alt1 = 0;
		pc.alt2 = 0;
		
		open_PortC ( &pc );

		while ( 1 ) {
			delay_ms_spin ( 1000 );
			ym_play_demo();
		} // while
		
	}
#endif
	
	return ( 0 );
}
