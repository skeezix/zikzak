
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
  unsigned int i = 0;
  unsigned int j = 0;

  cli();

  //DDRB = 0xFF; 
  DDRB = _BV (PB2); // B out -> LED
  DDRD = 0xFF;      // D out -> Address and Control lines

  LED_OFF;

  sram_reset();

#if 1
  // clear all ram to zero
  for ( i = 0; i < 0x0F; i++ ) {
    sram_write ( i, 0 );
  }
#endif

#if 1
  // store some test values
  for ( i = 0; i < 0x0F; i++ ) {
    sram_write ( i, i );
  }
#endif

#if 1
  // sram test - can we get back the expected values
  j = 0;
  for ( i = 0; i < 0x0F; i++ ) {
    if ( sram_read ( i ) != i ) {
      j++;
    }
  }
  if ( j == 0 ) {
    LED_ON;
  }
#endif

  while ( 1 ) {

#if 0 // TEST AS NEEDED
    // LED blinker -- works fine
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

  } // while forever

} // main
