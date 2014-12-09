#include <eZ80.h>
#include <uart.h>
#include <gpio.h>
#include <stdlib.h>
#include <string.h>

#include "skeelib.h"
#include "framebuffer.h"
#include <lib_ay-3-8912.h>
#include "lib_fontrender.h"
#include "menu.h"

void demo_linedemo ( UINT16 maxlines ) {
	
	//while ( 1 ) {
		UINT8 x, y, lx = 0, ly; // x, y, last-x, last-y
		UINT8 c;
		UINT16 i;
		UINT8 *extram;
			
		extram = (UINT8 *) 0x0C0000;

		//fb_render_rect_filled ( extram, 0, 0, 250, 230, 1 );
		memset ( extram, 1, 256*200 );
		
		render_font_8x8 ( 0, 0,  "Zikzak http://www.zikzak.ca", lame_randrange8 ( 1, 0xFF ) );
		render_font_8x8 ( 0, 8,  "Mode: Colour 256x240", lame_randrange8 ( 1, 0xFF ) );
		render_font_8x8 ( 0, 16, "In: CPU Serial", lame_randrange8 ( 1, 0xFF ) );
		
		x = y = lx = ly = 0;

		for ( i = 0; i < maxlines; i++ ) {
			
			x = lame_randrange8 ( 10, 230 );
			y = lame_randrange8 ( 30, 170 );
			c = lame_randrange8 ( 0, 0x3F ); // 0b00111111
			
			if ( lx != 0 ) {
				zl_render_line ( extram, c, lx, ly, x, y );
			}
			
			lx = x;
			ly = y;
			
		} // for

		//delay_ms_spin ( 20 );		
		
		//while(1);

	//}

	return;
}

void demo_pause ( char *message ) {
	char inbuf [ 5 ] = "\0\0\0\0\0";
	UINT24 nbytes;

	UINT8 *extram;
	extram = (UINT8 *) 0x0C0000;
	memset ( extram, 1, 256*8*3 );

	render_font_8x8 ( 0, 0,  message, lame_randrange8 ( 1, 0xFF ) );
	nbytes = 1;
	read_UART0 ( inbuf, &nbytes );
	
	return;
}

void demo_offset_squares ( void ) {
    UINT16 x, y, i;
    unsigned char v;
	
	UINT8 *extram;
	extram = (UINT8 *) 0x0C0000;

	memset ( extram, 1, 256*200 );
	
    for ( y = 0; y < FBHEIGHT; y++ ) {

      //i = 0;
      i = ( y / 10 ) % 5;

      for ( x = 0; x < (FBWIDTH/12); x++ ) {

        i++;

        if ( i == 0 ) {
          v = (unsigned char) 1;
        } else if ( i == 1 ) {
          v = (unsigned char) 2;
        } else if ( i == 2 ) {
          v = (unsigned char) 4;
        } else if ( i == 3 ) {
          v = (unsigned char) 8;
        } else if ( i == 4 ) {
          v = (unsigned char) 16;
        } else if ( i == 5 ) {
          v = (unsigned char) 32;
        } else {
          i = 0;
          v = (unsigned char) 1;
        }

        if ( y % 10 == 0 ) {
          v = 0; // black
        }

		memset ( extram + ( y * FBWIDTH ) + (x*12), v, 12 );

      } // x

    } // y

	return;	
}

void demo_charset ( void ) {
	UINT8 *extram;
	UINT8 c;
	char b [ 3 ] = "\0\0\0";
			
	extram = (UINT8 *) 0x0C0000;

	memset ( extram, 1, 256*200 );
	
	for ( c = 0; c < 128; c++ ) {
		b [ 0 ] = c;
		render_font_8x8 ( (c % 16) * 12, 32 + ((c / 16) * 16), b, 0xFF );
	}
	
}

void demo_sprite_fb ( void ) {
	UINT8 *extram, *spriterow;
	UINT8 x;
	char b [ 3 ] = "\0\0\0";
	UINT8 rounds;
	
	b [ 0 ] = 15;
	
	extram = (UINT8 *) 0x0C0000;
	memset ( extram, 1, 256*200 );
	
	spriterow = extram + ( 256*64 );

	for ( rounds = 0; rounds < 5; rounds++ ) {

		for ( x = 10; x < 230; x++ ) {
			memset ( spriterow, 1, 256*8 );
			render_font_8x8 ( x, 64, b, 0xFF );
		}
		for ( ; x > 10; x-- ) {
			memset ( spriterow, 1, 256*8 );
			render_font_8x8 ( x, 64, b, 0xFF );
		}

	} // rounds

}
