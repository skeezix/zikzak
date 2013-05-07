#include <avr/io.h>
#include <string.h>
#include <stdlib.h>
#include "main.h"
#include "lcd_drv.h"

// adafruit lcd wiring tutorial: http://learn.adafruit.com/character-lcds/wiring-a-character-lcd
// install path: /usr/lib/avr/include/avr

#include <avr/pgmspace.h>
 
int main( void )
{
  uint32_t val = 0;
  char s[sizeof("4294967295")];		// ASCII space for 32 bit number
  char textbuf [ 16 + 1 ];

  //_delay_ms ( 5000 );

  lcd_init();

#if 1
  char *p;
  //p = malloc ( 20 );

  if ( 1 ) {
#if 0
    char  *bitches = "bitchesbecrazy";
    unsigned int i;
    for ( i = 0; i < /*strlen ( bitches )*/6; i++ ) {
      textbuf [ i ] = bitches [ i ];
    }
    textbuf [ i ] = '\0';
#endif
    lcd_xy( 0, 1 );
    unsigned int i;
    i = 0;
    textbuf [ i++ ] = 'b';
    textbuf [ i++] = 'i';
    textbuf [ i++] = 't';
    textbuf [ i++] = 'c';
    textbuf [ i++] = 'h';
    textbuf [ i++] = 'e';
    textbuf [ i++] = 's';
    textbuf [ i++] = ' ';
    textbuf [ i++] = 'b';
    textbuf [ i++] = 'e';
    textbuf [ i++] = ' ';
    textbuf [ i++] = 'c';
    textbuf [ i++] = 'r';
    textbuf [ i++] = 'a';
    textbuf [ i++] = 'z';
    textbuf [ i++] = 'y';
    textbuf [ i++ ] = '\0';
    lcd_puts( textbuf );			// display number right adjusted
  }
#endif

#if 0
  lcd_xy( 0, 1 );
  lcd_puts( "Hello Peter" );
#endif

#if 0
  for(;;){
    ultoa( val, s, 10 );
    lcd_xy( 0, 1 );
    lcd_blank( 10 - strlen( s ));	// insert spaces until number
    lcd_puts( s );			// display number right adjusted
    val++;
  }
#endif

  return ( 0 );
}
