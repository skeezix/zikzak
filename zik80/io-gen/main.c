
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


extern volatile unsigned char _g_joy_state;



int main ( void ) {

  /*
   * setup
   */

  // JOY: set up pin change interupts
  EICRA &= ~ ( (1 << ISC11) | (1 << ISC10) ); // reset: clear ISC11+ISC10 (ISC1x for vect1 which we need for joy)
  EICRA |= ( (1 << ISC10) ); // 00 set and 01 unset means any edge will make event
  PCMSK1 |= ( (1 << PCINT10) | (1 << PCINT11) | (1 << PCINT12) | (1 << PCINT13) | (1 << PCINT14) ); // Pins to monitor
  PCICR |= (1 << PCIE1); // PB is monitored
  //
  DDRB = 0x00; // all input




  /*
   * doit
   */

  // enable interupts
  sei();

  // blink test, so we're ready..
#if 0
  blink_forever();
#endif

  while ( 1 ) {

    if ( _g_joy_state & (1<<PB2) ) {
      PORTB &= ~(1<<PB0);
    } else {
      PORTB |= (1<<PB0);
    }

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

volatile unsigned char _g_joy_state = 0; // so we can know which pins changed since last interupt
#define JOYMASK ( (1<<PB2) | (1<<PB3) | (1<<PB4) | (1<<PB5) | (1<<PB6) )
ISR(PCINT1_vect)
{
  unsigned char delta = _g_joy_state ^ PINB;

  if ( delta & (1<<PB2) ) {

    // CH1: edge has gone up, or down?
    if ( PINB & ( 1 << PB2 ) ) {
      // UP -> off: edge has gone high (nolonger pointing up)

    } else {
      // UP -> on: edge has gone low - joy is pointing up!

    }

  } // changed?

  // store current pin state
  _g_joy_state = PINB;

} // PCINT1_vect
