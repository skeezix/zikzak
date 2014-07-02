
#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

//#include "iocompat.h"           /* Note [1] */

// install path: /usr/lib/avr/include/avr

#if 1
//#define F_CPU 1000000UL  /* 1 MHz CPU clock */
#define F_CPU 8000000UL  /* 8 MHz CPU clock */
//#define F_CPU 20000000UL  /* 20 MHz CPU clock */

#include <util/delay.h>
#include <avr/io.h>





void startup ( void ) {

  // ensure all ports are tristated
  DDRA = 0;
  PORTA = 0;

  DDRB = 0;
  PORTB = 0;

  DDRC = 0;
  PORTC = 0;

  DDRD = 0;
  PORTD = 0;

  return;
}

void setup_for_write ( uint16_t address, uint8_t value ) {

  // address lines open, set to 0
  DDRD = 0xFF;
  DDRC = 0xFF;
  PORTD = ( address >> 8 ) & 0xFF;
  PORTC = ( address & 0xFF );

  // data lines open to send
  DDRA = 0xFF;
  PORTA = value;

  // ctrl lines
  DDRB =  0b10010111;
  PORTB = 0b00001110;

  return;
}

void setup_for_read ( uint16_t address ) {

  // address lines open, set to 0
  DDRD = 0xFF;
  DDRC = 0xFF;
  PORTD = ( address >> 8 ) & 0xFF;
  PORTC = ( address & 0xFF );

  // data lines open to read
  DDRA = 0x00;
  PORTA = 0xFF; // pull up, default read value is 1

  // ctrl lines
  DDRB =  0b10010111;
  PORTB = 0b00001101;

  return;
}












// direction
//   ADR (analog direction register) and DDR (digital direction register)
//   DDRx -> set value to 1, sets dir to write; low is read
//   PORTxn -> set high; if write, sets to high; if read, sets pull up resistor or low, high-Z tristate
// interupts
//   GIMSK = _BV (INT0); // int - Enable external interrupts int0
//   MCUCR = _BV (ISC01); // int - INT0 is falling edge
//   sei(); // int - Global enable interrupts

int main ( void ) {

  startup();

  _delay_ms ( 1000 );
  uint16_t a = 0;
  uint8_t v = 0;

  setup_for_write ( a, v );

  DDRB = _BV (PB0);               /* PC0 is digital output */

  while ( 1 ) {

    if ( PINB

    /* set PC0 on PORTC (digital high) and delay for 500mS */
    PORTB &= ~_BV(PB0);
    _delay_ms ( 100 );

    /*  PC0 on PORTC (digital 0) and delay for 500mS */
    PORTB |= _BV(PB0);
    _delay_ms ( 100 );
  }

  return (0);
}
#endif
