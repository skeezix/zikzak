#include <eZ80.h>
#include <stdio.h>

#include "skeelib.h"
#include "lib_fontrender.h"
#include "font_vincent.h"

void render_font_8x8 ( unsigned char tx, unsigned char ty, char *text, unsigned char colour ) {
	signed char x, y;
	UINT8 *extram;
	UINT8 *linestart;
	unsigned char *p;
	
	extram = (UINT8 *) 0x0C0000;       // start of RAM
	extram += ((UINT16)ty)*256;        // skip down to target Y
	linestart = extram;
	extram += tx;                      // skip over to target X
	
	if ( ! text ) {
		return;
	}
	
	while ( *text != '\0' ) {
		
		if ( *text == '\n' ) {
			text++;
			linestart += ( 256 * 8 );
			extram = linestart;
		}

		p = vincent_data [ *text ];
		
		for ( y = 0; y < 8; y++ ) {
			
			for ( x = 7; x >= 0; x-- ) {
				
				if ( (*p) & (1<<x) ) {
					*extram = colour;
				} else {
					*extram = 1;
				}
				
				extram++; // move over a byte (X increment on display)
			} // x shifter
			
			extram -= 8; // next line, so go back to left-edge of character
			extram += 256; // go to next line of display
			
			p++;
		} // y
		
		extram -= (8*256); // rewind back to top-left corner
		extram += 8;
		
		text++;
	} // while not end of string
	
	return;
}