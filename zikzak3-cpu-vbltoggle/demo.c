
#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

//#include "iocompat.h"           /* Note [1] */

// install path: /usr/lib/avr/include/avr

//#define F_CPU 1000000UL  /* 1 MHz CPU clock */
#define F_CPU 8000000UL  /* 8 MHz CPU clock */
//#define F_CPU 20000000UL  /* 20 MHz CPU clock */

#include <util/delay.h>
#include <avr/io.h>

#define NOP          __asm__("nop\n\t") /* nop - 1 cycle */

// direction
//   ADR (analog direction register) and DDR (digital direction register)
//   DDRx -> set value to 1, sets dir to write; low is read
//   PORTxn -> set high; if write, sets to high; if read, sets pull up resistor
// interupts
//   GIMSK = _BV (INT0); // int - Enable external interrupts int0
//   MCUCR = _BV (ISC01); // int - INT0 is falling edge
//   sei(); // int - Global enable interrupts

int main ( void ) {
  DDRG = _BV (PG4);               /* PC0 is digital output */
  DDRE = 0;
  DDRE = _BV (PE2);               /* PC0 is digital output */

  unsigned char bank = 0;
  PORTG &= ~_BV(PG4);

  unsigned char i = 0;

  while ( 1 ) {

    // is vblank high? if so, switch and delay
    if ( PINE & _BV(PE3) ) {

      if ( PORTG & _BV(PG4) ) {
        PORTG &= ~_BV(PG4);
      } else {
        PORTG |= _BV(PG4);
      }

      // wait till vbl goes away
      while ( PINE & _BV(PE3) ) {
        // spin
        NOP;
      } // while

      i++;

      // blink heartbeat
      if ( i == 30 ) {

        if ( PINE & _BV(PE2) ) {
          PORTE &= ~_BV(PE2);
        } else {
          PORTE |= _BV(PE2);
        }

        i = 0;
      }

    } // if vbl

    // spin

  } // while forever

  return (0);
}
