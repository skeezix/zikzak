
// install path: /usr/lib/avr/include/avr

#include "main.h"

#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/delay.h>
#include <util/delay.h>
#include <avr/io.h>
#include <stdio.h>

// direction
//   ADR (analog direction register) and DDR (digital direction register)
//   DDRx -> set value to 1, sets dir to write; low is read
//   PORTxn -> set high; if write, sets to high; if read, sets pull up resistor
// interupts
//   GIMSK = _BV (INT0); // int - Enable external interrupts int0
//   MCUCR = _BV (ISC01); // int - INT0 is falling edge
//   sei(); // int - Global enable interrupts

int main ( void ) {

  // blink test, so we're ready..
#if 1
  blinkit();
#endif





  // spin
  blinkit();

  return ( 0 );
}

void blinkit ( void ) {

  // blink test
  while ( 1 ) {
    PORTB ^= (1<<PB0);
    _delay_ms ( 200 );
  } // while forever

}
