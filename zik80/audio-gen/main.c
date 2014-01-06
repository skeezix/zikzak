
// install path: /usr/lib/avr/include/avr

#include "config.h"

#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/delay.h>
#include <util/delay.h>
#include <avr/io.h>
#include <stdio.h>

#include "main.h"

// direction
//   ADR (analog direction register) and DDR (digital direction register)
//   DDRx -> set value to 1, sets dir to write; low is read
//   PORTxn -> set high; if write, sets to high; if read, sets pull up resistor
// interupts
//   GIMSK = _BV (INT0); // int - Enable external interrupts int0
//   MCUCR = _BV (ISC01); // int - INT0 is falling edge
//   sei(); // int - Global enable interrupts




int main ( void ) {

  /*
   * setup
   */

  //
  DDRB = 0x00; // all input
  DDRB |= (1<<PB0); // LED out
  DDRC = 0xFF; // all output

  /*
   * doit
   */

  // enable interupts
  sei();

  // blink test, so we're ready..
#if 1
  blink_forever();
#endif

  while ( 1 ) {


  } // while forever


  // spin
  blink_forever();

  return ( 0 );
}

void blink_forever ( void ) {

  // blink test
  while ( 1 ) {
    PORTB ^= (1<<PB0);
    _delay_ms ( 200 );
  } // while forever

}
