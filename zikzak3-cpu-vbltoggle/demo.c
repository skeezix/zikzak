
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

#define RAM_ADDR_LOW  PORTA
#define RAM_ADDR_HIGH PORTC
#define RAM_IO        PORTD
#define RAM_IO_READ   DDRD = 0x00 /* low = in; read mode */
#define RAM_IO_WRITE  DDRD = 0xFF /* high = write mode */

#define CPU_RAM_OE   PG1
#define CPU_RAM_WR   PG0

#define CPU_DIR      PG2
#define CPU_DIR_R    _BV(CPU_DIR) |= 1 /* high for read, low for write */

#define CPU_SET_CHANGING PORTG |= ( _BV(CPU_RAM_WR) | _BV(CPU_RAM_OE) ) /* set read and write off */
#define CPU_SET_READ PORTG |= ( _BV(CPU_DIR) ); PORTG &= ~ ( _BV(CPU_RAM_OE) ) /* set dir high(read) and clear OE (read) */
#define CPU_SET_WRITE PORTG &= ~( _BV(CPU_DIR) | _BV(CPU_RAM_WR) )

// direction
//   ADR (analog direction register) and DDR (digital direction register)
//   DDRx -> set value to 1, sets dir to write; low is read
//   PORTxn -> set high; if write, sets to high; if read, sets pull up resistor
// interupts
//   GIMSK = _BV (INT0); // int - Enable external interrupts int0
//   MCUCR = _BV (ISC01); // int - INT0 is falling edge
//   sei(); // int - Global enable interrupts

int main ( void ) {

  DDRG = 0xFF; // G is always out
  DDRA = 0xFF; // address line is always out
  DDRC = 0xFF; // address line is always out

  DDRE = 0;
  DDRE = _BV (PE2);               /* PC0 is digital output */

  unsigned char bank = 0;
  PORTG &= ~_BV(PG4); // set default bank to 0 (A)
  RAM_ADDR_HIGH = 0;
  RAM_ADDR_LOW = 0;

  unsigned char i, h, l;

  //CPU_SET_CHANGING;
  //RAM_IO_READ;
  //CPU_SET_READ;

  // toggle VRAM bank
  i = 0;
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


      // clear VRAMs
      CPU_SET_CHANGING;
      RAM_IO_WRITE;
      CPU_SET_WRITE;

      for ( h = 0; h < 0xFF; h++ ) {
        RAM_ADDR_HIGH = h;
        for ( l = 0; l < 0xFF; l++ ) {
          RAM_ADDR_LOW = l;
          //NOP;
          RAM_IO = 0xFF; // all IO pins should be high, easier for probing
          //NOP;
        } // for low
      } // for high



      // blink heartbeat
      if ( i == 60 ) {

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
