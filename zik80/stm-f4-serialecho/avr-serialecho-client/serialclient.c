
#include "serialclient.h"

#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

// install path: /usr/lib/avr/include/avr

#include <util/delay.h>
#include <avr/io.h>
#include <util/setbaud.h>
#include <stdio.h>

#include "lib_serial.h"

// direction
//   ADR (analog direction register) and DDR (digital direction register)
//   DDRx -> set value to 1, sets dir to write; low is read
//   PORTxn -> set high; if write, sets to high; if read, sets pull up resistor
// interupts
//   GIMSK = _BV (INT0); // int - Enable external interrupts int0
//   MCUCR = _BV (ISC01); // int - INT0 is falling edge
//   sei(); // int - Global enable interrupts

int main ( void ) {

  DDRD = 0;
  //DDRD |= _BV (PD0);               /* PC0 is digital output */
  DDRD |= _BV (PD1);               /* PC0 is digital output */

  // blinker
  DDRD |= _BV (PD6);               /* PC0 is digital output */




  // serial set up
  uart_setup();
  uart_stream_setup();


#if 0
  // serial test to stm32
  puts ( "p" );
  char c;

  c = getchar();

  if ( c != 'p' ) {
    while(1); // fail
  }
#endif


  while ( 1 ) {

    /* set PC0 on PORTC (digital high) and delay for 500mS */
    PORTD &= ~_BV(PD6);
    _delay_ms ( 100 );

    /*  PC0 on PORTC (digital 0) and delay for 500mS */
    PORTD |= _BV(PD6);
    _delay_ms ( 100 );

    uart_putchar_pre ( 'X' );
    uart_putchar_pre ( '\r' );
    uart_putchar_pre ( '\n' );

  }

  return ( 0 );
}
