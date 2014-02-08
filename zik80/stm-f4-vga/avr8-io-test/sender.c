
#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

// install path: /usr/lib/avr/include/avr

#if 1
//#define F_CPU 1000000UL  /* 1 MHz CPU clock */
#define F_CPU 8000000UL  /* 8 MHz CPU clock */
//#define F_CPU 20000000UL  /* 20 MHz CPU clock */

#include <util/delay.h>
#include <avr/io.h>

// direction
//   ADR (analog direction register) and DDR (digital direction register)
//   DDRx -> set value to 1, sets dir to write; low is read
//   PORTxn -> set high; if write, sets to high; if read, sets pull up resistor
// interupts
//   GIMSK = _BV (INT0); // int - Enable external interrupts int0
//   MCUCR = _BV (ISC01); // int - INT0 is falling edge
//   sei(); // int - Global enable interrupts

// PB0 is vblank in
// PB1 is data-clock-out

#define NOP __asm__("nop\n\t")

static inline protocol_push_byte ( unsigned char b ) {
  PORTD = b;
  PORTB |= (1<<1); // bring clock up (send rising edge)
  PORTB &= ~(1<<1); // ensure clock is down
}


int main ( void ) {
  unsigned char vblank;

  DDRD = 0xFF; // data - all output

  DDRB = 0; // default input
  DDRB |= (1<<1); // PB1 out
  DDRB |= (1<<2); // PB2 out, led toggle

  PORTD = 0; // data default 0
  PORTB = 0; // clock, led default off, and no-impedance on vblank

  vblank = 0;
  unsigned int x = 20;
  signed char dx = 1;
  unsigned int i;
  while ( 1 ) {

    // got a vblank?
    if ( PINB & (1<<0) ) {

      // not in vblank state, but got vblank, so start off a vblank handler
      if ( ! vblank ) {

        // toggle LED
        PORTB ^= _BV(PB2);

        /* do some crimes
         */
        // *** assume clock starts down */

        // NOP so receiver wakes up? fighting with hsync
        protocol_push_byte ( 0 ); // NOP

        // spam 'sprite moves' for fun, just to see how many we can do before it can't fit vblank
        for ( i = 0; i < 20; i++ ) {
          protocol_push_byte ( 1 ); // move sprite
          protocol_push_byte ( 0 ); // sprite 0
          protocol_push_byte ( 10 ); // X
          protocol_push_byte ( 10 ); // Y

          protocol_push_byte ( 1 ); // move sprite
          protocol_push_byte ( 0 ); // sprite 0
          protocol_push_byte ( 10 ); // X
          protocol_push_byte ( 10 ); // Y

          protocol_push_byte ( 0 ); // NOP // fighting with hsync
          protocol_push_byte ( 0 ); // NOP
          protocol_push_byte ( 0 ); // NOP
        }

        if ( PINB & (1<<0) ) {
          protocol_push_byte ( 1 ); // move sprite
          protocol_push_byte ( 0 ); // sprite 0
          protocol_push_byte ( x ); // X
          protocol_push_byte ( 64 ); // Y

          protocol_push_byte ( 1 ); // move sprite
          protocol_push_byte ( 0 ); // sprite 0
          protocol_push_byte ( x ); // X
          protocol_push_byte ( 64+32 ); // Y
        }

        // animate
        if ( dx > 0) {
          if ( x < 200 ) {
            x ++;
          } else {
            dx = -1;
          }
        } else {
          if ( x > 10 ) {
            x--;
          } else {
            dx = 1;
          }
        }

      } // got new vblank!

      // just got a new vblank
      vblank = 1;

    } else { // no vblank

      if ( vblank ) {
        // toggle LED
        PORTB ^= _BV(PB2);

        // had vblank, but actual vblank is now gone
        vblank = 0;
      }

    }

  } // while

  return (0);
}
#endif
