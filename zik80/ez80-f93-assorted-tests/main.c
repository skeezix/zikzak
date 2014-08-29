// Chip Selects:
// CS0 e8 0 0c0000 0dffff -> ram
// CS1 e8 0 1c0000 1dffff -> rom
// CS2 e8 0 2c0000 2cffff -> audio
//
// 2.457MHz oscilator in place

#include <eZ80.h>
#include <uart.h>
#include <gpio.h>
#include <stdlib.h>

#include "skeelib.h"
#include "framebuffer.h"
#include <lib_ay-3-8912.h>

int main ( ) {

#if 1 // sleep for a few seconds up front, making emergency reflash easier (than racing against HALTs)
	{
		delay_ms_spin(1000);
		delay_ms_spin(1000);
	}
#endif
	
#if 1 // uart logger - WORKS
	{
		// F93 default is 20MHz - happens to be right for us
		// uart0 is on port d
		// uart1 is on port c
		
		//UCHAR open_UARTx(UART * pUART);
		
		UART u0;
		char b [ 16 ];
		unsigned char i;
		
		u0.uartMode = INTERRUPT; //POLL; //INTERRUPT;
		u0.baudRate =  BAUD_57600; //BAUD_115200; //BAUD_38400;
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
	
#if 0 // ext ram write
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
	
#if 0 // ext audio AY-3-8912 test
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

//#__pragma noopt
#if 0 // ext rom read
	//while (1)
	{
		unsigned char *extram;
		unsigned char b [ 251 ];
		unsigned char i;
		unsigned char v;
		
		extram = (unsigned char *) 0x1C0000;
		
		for ( i = 0; i < 250; i++ ) {
			v = *extram;
			b [ i ] = v;
			extram++;
		}
		
		write_UART0 ( (char*)b, i - 1 );
		
		extram = (unsigned char *) 0x1C0000;

	}
#endif

#if 0 // ext rom write-delay-loop - full screen render
	{
		UINT8 shift = 0;
		while (1)
		{
			unsigned char *extram;
			UINT32 x, y;
			
			extram = (unsigned char *) 0x0C0000;
			
			for ( y = 0; y < 192; y++ ) {
				for ( x = 0; x < 256; x++ ) {
					*extram = y + (shift<<1);
					extram++;
				}
			}
			
			//delay_ms_spin ( 10 );
			
			//extram = (unsigned char *) 0x0C0000;
			
			shift++;

		}
	}
#endif

#if 0 // ext ram write-delay-loop - minor update per frame
	{
		UINT16 y = 0;
		UINT16 c = 0;
		UINT16 cx = 2, cy = 150;
		while (1)
		{
			unsigned char *extram;
			UINT16 x;
			
			extram = (unsigned char *) 0x0C0000;
			extram += ( y * 256 );
			
			if ( y > 190 ) continue;
			
			for ( x = 0; x < 256; x++ ) {
				*extram = c & 0xFF;
				extram++;
			}
			
			delay_ms_spin ( 10 );
			
			y++;
			c++;
			
		}
	}
#endif

	
#if 1 // RAM random line demo
	while ( 1 ) {
		UINT8 x, y, lx = 0, ly; // x, y, last-x, last-y
		UINT8 c;
		UINT16 maxlines = 1250;
		UINT8 i;
		UINT8 *extram;
			
		extram = (UINT8 *) 0x0C0000;

		fb_render_rect_filled ( extram, 0, 0, 250, 190, 1 );
		
		x = y = lx = ly = 0;

		for ( i = 0; i < maxlines; i++ ) {
			
			x = lame_randrange8 ( 10, 230 );
			y = lame_randrange8 ( 10, 170 );
			c = lame_randrange8 ( 0, 0x3F ); // 0b00111111
			
			if ( lx != 0 ) {
				zl_render_line ( extram, c, lx, ly, x, y );
			}
			
			lx = x;
			ly = y;
			
		} // for

		delay_ms_spin ( 20 );		
		
		while(1);

	}
#endif
	
#if 1 // blinker - WORKS
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
			delay_loop ( 150 );
			PC_DR ^= 1;
			//SETDR_PORTC ( 1 );
		}
	}		
#endif

	
#if 1 // do nothing
	{
		char b [ 20 ];
		UINT8 v = 1;
		while ( 1 ) {
			lame_itoa ( v, b );
		}
	}
#endif
	
	return ( 0 );
}
