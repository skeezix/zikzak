
#include "main.h"

#include <avr/pgmspace.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/delay.h>

#include <string.h>
#include <stdlib.h>

#include "sram.h"
#include "nop.h"

void main ( void ) {
  long unsigned int i = 0;

  cli();

  //DDRB = 0xFF; 
  DDRB = _BV (PB2); // B out -> LED
  DDRD = 0xFF;      // D out -> Address and Control lines

  LED_OFF;

  CE_HIGH;     // disable chip
  WE_HIGH;     // disable write
  OE_LOW;      // enable read

#if 1 // pre clear ram
  sram_write ( 0, 0 );
  sram_write ( 1, 0 );
  sram_write ( 2, 0 );
  sram_write ( 3, 0 );
#endif

#if 1
  sram_write ( 0, 1 );
  sram_write ( 1, 1 );
  sram_write ( 2, 1 );
#endif

  while ( 1 ) {

#if 0
    // LED blinker
    if ( i == (F_CPU/40) ) {

      if ( LEDPORT & LED ) {
        LED_OFF;
      } else {
        LED_ON;
      }

      i = 0;
    } // if counter is high
    i++;
#endif

#if 1 // sram test
    if ( ( sram_read ( 0 ) == 1 ) &&
         ( sram_read ( 1 ) == 1 ) )
    {
      LED_ON;
    }
#endif

#if 0 // sram test
    if ( sram_read ( 1 ) == 1 ) {
      LED_ON;
    }
#endif

    // SRAM business

  } // while forever

} // main
