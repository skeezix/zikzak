// Chip Selects:
// CS0   e8 0   0c0000 - 0dffff    -> ram 128KB
//                     - 7dffff    -> ram 512KB
// CS1   e8 0   1c0000 - 1dffff    -> rom 128KB
// CS2   e8 0   2c0000 - 2cffff    -> audio
//
// 2.457MHz oscilator in place

#include <eZ80.h>
#include <uart.h>
#include <gpio.h>
#include <stdlib.h>
#include <string.h>

#include "skeelib.h"
#include "framebuffer.h"
#include <lib_ay-3-8912.h>
#include "lib_fontrender.h"
#include "lib_ps2_keyboard.h"
#include "menu.h"
#include "HAL.h"
#include "bios_video.h"
#include "demo.h"

int main ( ) {

#if 1 // sleep for a few seconds up front, making emergency reflash easier (than racing against HALTs)
	{
		delay_ms_spin(1000);
		delay_ms_spin(1000);
	}
#endif

	// enable interupts
	_init_default_vectors();

#if 0 // GPIOB for lulz debug output
	{
		PORT pb;
		UCHAR err;

		// enable gpio output mode1 port	
		pb.dr = 0; // default value 0off
		pb.ddr = 0; // output=0, input=1
		pb.alt1 = 0;
		pb.alt2 = 0;
			
		open_PortC ( &pb );

		err = setmode_PortB ( PORTPIN_ZERO, GPIOMODE_OUTPUT );
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
		
		u0.uartMode = POLL; //INTERRUPT; //POLL; //INTERRUPT;
		u0.baudRate =  BAUD_38400; //BAUD_57600; //BAUD_115200; //BAUD_38400;
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
		
		lame_itoa ( (int)37, b );
		
		write_UART0 ( b, lame_strlen ( b ) );
		write_UART0 ( "\n", 1 );
		flush_UART0 ( FLUSHFIFO_ALL );
		
	}
#endif

#if 1 // keyboard isr interrupt test; show mapped char, and scancode, on serial
	{
		uint8_t p;
		char b [ 10 ];
		
		keyb_setup();
		
		while ( 1 ) {
			p = packet_fetch_blocking();

			// scancode
			lame_itoa ( (int)p, b );
			write_UART0 ( b, lame_strlen ( b ) );
			write_UART0 ( "\n", 1 );
			// mapped char
			b [ 0 ] = map_scan_code ( p );
			b [ 1 ] = '\n';
			b [ 2 ] = '\0';
			write_UART0 ( b, 2 );

			//flush_UART0 ( FLUSHFIFO_ALL );

			//delay_loop ( 10 );
		}

	}
#endif
	
#if 0 // ram test - and why aren't I making a HAL library, and a hwtest library on top of it?
	{
		unsigned char *extram, *iter, *max;
		unsigned char v;
		char msg [ 64 ];
		
		extram = (unsigned char *) EXTRAM_BASE;

		max = extram;
		max += 65536; max += 65536;
		max += 65536; max += 65536;
		max += 65536; max += 65536;
		max += 65536; max += 65536;

		// writer stage
		strcpy ( msg, "Memory test: writing sequence to RAM\n" );
		write_UART0 ( msg, lame_strlen ( msg ) );
		
		iter = extram;
		v = 0;
		
		while ( iter != max ) {
			*iter = v;
			v++;
			iter++;
		}
		
		// verification stage
		strcpy ( msg, "Memory test: verifying sequence in RAM\n" );
		write_UART0 ( msg, lame_strlen ( msg ) );

		iter = extram;
		v = 0;
		
		while ( iter != max ) {
			if ( *iter != v ) {
				strcpy ( msg, "Memory test failed\n" );
				write_UART0 ( msg, lame_strlen ( msg ) );
				break;
			}

			v++;
			iter++;
		}
		
		if ( iter == max ) {
			strcpy ( msg, "Memory test: PASS\n" );
			write_UART0 ( msg, lame_strlen ( msg ) );
		}

	}
#endif
	
#if 0 // ext audio AY-3-8912 test
	{
		
		/*PORT pc;
		UCHAR err;
		
		pc.dr = 0x01;
		pc.ddr = 0; // 0 ddr is output
		//pc.alt0 = 0; // does not exist for F93
		pc.alt1 = 0;
		pc.alt2 = 0;
		
		open_PortC ( &pc );*/

		ay_gpio_setup();

		while ( 1 ) {
			delay_ms_spin ( 600 );
			ym_play_demo();
		} // while
		
	}
#endif

#if 1 // main menu
{
	unsigned char *extram;
	unsigned char spritelist [ TM_SPRITE_MAX * TM_SPRITE_CELL_STRIDE ];
	unsigned char *curr = spritelist;
	unsigned char iter;

	vb_enable_vblank_interupt();
	
	extram = (UINT8 *) TM_VRAM_FB;
	memset ( extram, 1, TM_TEXTLINE_STRIDE * TM_TEXTCOL_STRIDE ); // clear characters
	
	strcpy ( extram, "ZikZak SBC r3 512KB" );
	strcpy ( extram + TM_TEXTLINE_STRIDE, "Jeff Mitchell" );
		
	extram = (UINT8 *) TM_VRAM_ATTR;
	memset ( extram, 1, TM_TEXTLINE_STRIDE * TM_TEXTCOL_STRIDE ); // clear attributes
	
	extram = (UINT8 *) TM_SPRITE_LIST_BASE;
	memset ( extram, 0, TM_SPRITE_MAX * TM_SPRITE_CELL_STRIDE); // clear to hidden sprites
	*((UINT8*)TM_SPRITE_ACTIVE) = 4;

	curr [ 0 ] =  30; curr [ 1 ] =  30; curr [ 2 ] =      1; curr [ 3 ] = 32; extram += 4;
	curr [ 0 ] =  60; curr [ 1 ] =  60; curr [ 2 ] =      1; curr [ 3 ] = 32; extram += 4;
	curr [ 0 ] =  90; curr [ 1 ] =  90; curr [ 2 ] =      1; curr [ 3 ] = 32; extram += 4;
	curr [ 0 ] = 120; curr [ 1 ] = 120; curr [ 2 ] =      1; curr [ 3 ] = 32; extram += 4;
	
	// move sprites
	while ( 1 ) {
		
		curr = spritelist;
		for ( iter = 0; iter < 4; iter++ ) {
		
#if 1		
	    if ( curr [ 3 ] < 64 ) {
			// go left                                                                                                             
			if ( curr [ 0 ] > 10 ) {
              curr [ 0 ] -= 2;
            } else {
              curr [ 3 ] = 100;
            }
        } else if ( curr [ 3 ] > 64 && curr [ 3 ] < 128 ) {
            // go right                                                                                                            
            if ( curr [ 0 ] < FBWIDTH - 40 ) {
              curr [ 0 ] += 2;
            } else {
              curr [ 3 ] = 50;
            }
        } else if ( curr [ 3 ] > 128 && curr [ 3 ] < 192 ) {
            // up                                                                                                                  
            if ( curr [ 1 ] > 40 ) {
              curr [ 1 ] -= 1;
            } else {
              curr [ 3 ] = 250;
            }
        } else {
            // down                                                                                                                
            if ( curr [ 1 ] < FBHEIGHT - 40 ) {
              curr [ 1 ] += 1;
            } else {
              curr [ 3 ] = 150;
            }
         }
#endif
			curr += 4;
		} // iter
	 

		
		vb_wait_for_vblank ( VB_CLEAR_SAW_VBLANK );
	}

	while ( 1 ) {
		UINT8 *extram;
		char inbuf [ 5 ] = "\0\0\0\0\0";
		UINT24 nbytes;
		UCHAR retval;
			
		extram = (UINT8 *) TM_VRAM_FB;

		strcpy ( extram, "Zikzak http://www.zikzak.ca" );
		extram += TM_TEXTLINE_STRIDE;
		strcpy ( extram, "Zikzak http://www.zikzak.ca" );
		extram += TM_TEXTLINE_STRIDE;
		strcpy ( extram, "Zikzak http://www.zikzak.ca" );
		extram += TM_TEXTLINE_STRIDE;
		
		//render_font_8x8 ( 0, 0,  "Zikzak http://www.zikzak.ca", lame_randrange8 ( 1, 0xFF ) );
		//render_font_8x8 ( 0, 8,  "Mode: Colour 256x240", lame_randrange8 ( 1, 0xFF ) );
		//render_font_8x8 ( 0, 16, "In: CPU Serial", lame_randrange8 ( 1, 0xFF ) );
		
		//render_font_8x8 ( 0, 32, menu_mainmenu, lame_randrange8 ( 1, 0xFF ) );
		
		nbytes = 1;
		retval = read_UART0 ( inbuf, &nbytes );
		
		// echo back
		render_font_8x8 ( 0, 0,  inbuf, lame_randrange8 ( 1, 0xFF ) );
		write_UART0 ( inbuf, lame_strlen ( inbuf ) );
		
		if ( retval == UART_ERR_NONE ) {
			
			switch ( inbuf [ 0 ] ) {
				case '1':
				{
					demo_linedemo ( 500 );
					demo_pause ( "** Press key to exit demo ** " );
				}
				break;
				
				case '2':
				{
					demo_offset_squares();
					demo_pause ( "** Press key to exit demo ** " );
				}
				break;

				case '4':
				{
					demo_charset();
					demo_pause ( "** Press key to exit demo ** " );
				}
				break;

				case '5':
				{
					demo_sprite_fb();
					demo_pause ( "** Press key to exit demo ** " );
				}
				break;

				default:
					demo_pause ( "** Option not recognized. ** " );
			}
			
			
		} else {
			flush_UART0 ( FLUSHFIFO_ALL );
		}
		
	}

}	
#endif
	
#if 0 // do nothing
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
