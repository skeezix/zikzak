
#define F_CPU 20000000UL  /* 20 MHz CPU clock */

#include <avr/pgmspace.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/delay.h>

#include <string.h>
#include <stdlib.h>

#include "nop.h"

#include "main.h"

// skee board
// PC 01 RR
// PC 23 GG
// PC 45 BB
// PB 0 vsync (pin 1)
// PB 1 hsync (pin 2)


#define LED_OFF      PORTA &= ~(1<<7)
#define LED_ON       PORTA |= (1<<7)
#define LED          (PORTA & (1<<7))


#define VSYNC_LOW    PORTA &= ~_BV(2) /* sync pulse is LOW */
#define VSYNC_HIGH   PORTA |= _BV(2)  /* should be high all the rest of the time */

#define HSYNC_LOW    PORTA &= ~_BV(1) /* sync pulse is LOW */
#define HSYNC_HIGH   PORTA |= _BV(1)  /* should be high all the rest of the time */

#define VGA_ON       PORTA &= ~_BV(0) /* sync pulse is LOW */
#define VGA_OFF      PORTA |= _BV(0)  /* should be high all the rest of the time */


static inline void LINE ( void ) {

  // permit time for colour data from sram
  VGA_ON;
  _delay_us ( 15 ); // 20

  /* h-sync time
   */

  // make sure VGA is not emitting anything, colour or otherwise
  // (alternatively, set address to 0xFFFF (say), where we pre-store RGB(0,0,0)? )
  VGA_OFF;

  /* 1uS Front Porch */
  _delay_us ( 1 ); /* 1uS */

  /* 3.2uS Horizontal Sync */
  HSYNC_LOW;
  _delay_us ( 3 );
  NOP4;

  /* 2.2uS Back Porch */
  HSYNC_HIGH;
  _delay_us ( 1 );
  NOP4;

} // inline

static inline void LINENOP ( void ) {

  _delay_us ( 14 );

  /* 1uS Front Porch */
  _delay_us ( 1 ); /* 1uS */

  /* 3.2uS Horizontal Sync */
  HSYNC_LOW;
  _delay_us ( 3 );
  NOP4;

  /* 2.2uS Back Porch */
  HSYNC_HIGH;
  _delay_us ( 1 );
  NOP4;

} // inline

void main ( void ) {

  cli();

  DDRA = 0xFF; // A out -> control
  DDRC = 0xFF; // C out -> high address
  DDRD = 0xFF; // D out -> low address

  HSYNC_HIGH;
  VSYNC_HIGH;

  // 800 columns by 600 lines
  int i, j;

  PORTC = 0;
  PORTD = 0;

  LED_ON;

  while ( 1 ) {

    // -------------------------------------------------------------------------------
    // Main graphic content field (full line + hsync, repeat for each Y)
    // -------------------------------------------------------------------------------

    i = 0;
    while ( i < 600 ) {
      LINE();
      i++;
      PORTD = i;
    } // while

    // -------------------------------------------------------------------------------
    // Vertical sync business (and end of page, do a vsync)
    // -------------------------------------------------------------------------------

    LINENOP(); /* front porch -> 1 line */
    VSYNC_LOW;
    LINENOP(); /* 1 - sync pulse -> 4 lines */
    LINENOP(); /* 2 */
    LINENOP(); /* 3 */
    LINENOP(); /* 4 */
    VSYNC_HIGH;
    /* back porch -> 23 lines */
    i = 0;
    while ( i < 23 ) {
      LINENOP();
      i++;
    } // while

#if 0
    j++;
    if ( j == 60 ) {
      if ( LED ) {
        LED_OFF;
      } else {
        LED_ON;
      }
      j = 0;
    }
#endif

  } // while forever

} // main
