
#define F_CPU 20000000UL  /* 20 MHz CPU clock */

#include <avr/pgmspace.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/delay.h>

#include <string.h>
#include <stdlib.h>

#include "main.h"
#include "nop.h"

// skee board
// PC 01 RR
// PC 23 GG
// PC 45 BB
// PB 0 vsync (pin 1)
// PB 1 hsync (pin 2)

#define LED_OFF      PORTB &= ~(1<<2)
#define LED_ON       PORTB |= (1<<2)

#define VSYNC_LOW    PORTB &= ~_BV(1) /* sync pulse is LOW */
#define VSYNC_HIGH   PORTB |= _BV(1)  /* should be high all the rest of the time */

#define HSYNC_LOW    PORTB &= ~_BV(0) /* sync pulse is LOW */
#define HSYNC_HIGH   PORTB |= _BV(0)  /* should be high all the rest of the time */

#define RED_OFF      PORTC &= ~_BV(1)
#define RED_ON       PORTC |= _BV(1)

#define GREEN_OFF    PORTC &= ~_BV(3)
#define GREEN_ON     PORTC |= _BV(3)

#define BLUE_OFF     PORTC &= ~_BV(5)
#define BLUE_ON      PORTC |= _BV(5)

#define CYCLE_ON     PORTC = i&0x3F
#define CYCLE_OFF    PORTC &= ~0x3F

#define RGBALL_OFF   PORTC = 0

#define REGION(ymax,on,off)                                                                   \
    i=0;                                                                                      \
    /* 200 Lines */                                                                           \
    while ( i < ymax ) {                                                                      \
                                                                                              \
      on;                                                                                     \
                                                                                              \
      /* 20uS Color Data */                                                                   \
      _delay_us ( 20 ); /* 1uS */                                                             \
                                                                                              \
      off;                                                                                    \
                                                                                              \
      /* 1uS Front Porch */                                                                   \
      _delay_us ( 1 ); /* 1uS */                                                              \
                                                                                              \
      /* 3.2uS Horizontal Sync */                                                             \
      HSYNC_LOW;                                                                              \
      _delay_us ( 3 );                                                                        \
      NOP4;                                                                                   \
                                                                                              \
      /* 2.2uS Back Porch */                                                                  \
      HSYNC_HIGH;                                                                             \
      _delay_us ( 2 );                                                                        \
      NOP4;                                                                                   \
                                                                                              \
      i++;                                                                                    \
      /* 26.4uS Total */                                                                      \
    }


void main ( void ) {
  int i = 0;

  cli();

  DDRB = 0xFF; // B out
  DDRC = 0xFF; // C out

  HSYNC_HIGH;
  VSYNC_HIGH;

  while ( 1 ) {

    // -------------------------------------------------------------------------------
    // Horizontal line business
    // -------------------------------------------------------------------------------

    REGION(150,RED_ON,RED_OFF);
    REGION(150,GREEN_ON,GREEN_OFF);
    REGION(150,BLUE_ON,BLUE_OFF);
    REGION(150,CYCLE_ON,RGBALL_OFF);

    // -------------------------------------------------------------------------------
    // Vertical sync business
    // -------------------------------------------------------------------------------

    REGION(1,NOP,NOP); /* front porch -> 1 line */
    VSYNC_LOW;
    REGION(4,NOP,NOP); /* sync pulse -> 4 lines */
    VSYNC_HIGH;
    REGION(23,NOP,NOP); /* back porch -> 23 lines */

  } // while forever

} // main
