#include "main.h"
#include <avr/io.h>
#include <avr/interrupt.h>   // include interrupt support
#include <util/setbaud.h>
#include <avr/sleep.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "uart.h"
#include "avrutil.h"

// globals: timer
volatile unsigned int g_timer_us = 0; // will overflow and cycle after 6 seconds
volatile unsigned int g_time_s = 0;   // will overflow after 16 hours
volatile unsigned int _g_time_us_tick = 0; // internal ticker to count seconds passage; ms accrue since last sec bump

void serial_setup ( void );
void uart0_send ( char *s );
char textbuf [ 90 ];

unsigned char g_do_test_cycle = 0; // if set to !0, will run a test cycle

int main ( void ) {
  uint32_t val = 0;

  /* setup
   */

  // Timer: enable a timer so we can measure passage of time
  //
  // Given: 20MHz clock
  // --> if we want resolution of ms (1/1000th second) .. actually, we want us (1/10th of a ms) so we can measure partial ms
  // --> and we have 1/20000000 clock resolution
  // -----> 2000 ticks will get us there (20,000 will get us ms)
  //
  // Goal: Use CTC interupt mode (CTC -> Clear on Timer Compare)
  // So a compare matches, it clears to zero and triggers interupt
  TCCR1B |= (1 << WGM12); // Configure timer 1 for CTC mode 
  OCR1A = 800 /*2000*/; // number to compare against
  TIMSK1 |= (1 << OCIE1A); // Enable CTC interrupt 
  TCCR1B |= (1 << CS10); // Set up timer , with no prescaler (works at full MHz of clock)

  // setup done - kick up interupts
  sei();

  // lets burn the first couple of seconds
  {
    unsigned int start_sec = g_time_s;
    while ( g_time_s - start_sec < 1 ) {
      nop();
    }
  }

  // LED
  DDRD |= (1<<6 );
  PORTD |= (1<<6); // on

  // serial setup - logging
  serial_setup();

  sprintf ( textbuf, "test hello\n--\n" );
  uart0_send ( textbuf );

#if 1 // timer test .. show per-second counter update on lcd
  if ( 1 ) {
    unsigned int last_sec = g_time_s;
    unsigned int last_us = _g_time_us_tick;

    while(1) {


      // one second has past?
#if 1
      if ( g_time_s != last_sec ) {

        // something
        //

        // toggle LED
        PORTD ^= (1<<6);

        // text
        sprintf ( textbuf, "." );
        uart0_send ( textbuf );


        // continue
        last_sec = g_time_s;
      } // 1 sec tick
#endif


      // spin, avoid cpu constant hit
      _delay_ms ( 1 );

    } // while forever

  } // if 1
#endif

  /* churn forever
   */
  while(1);

  return ( 0 );
}

// ISR for timer1 ticking away
ISR(TIMER1_COMPA_vect)
{
  // bump ms timer counter
  g_timer_us++;

  // bump 'seconds' counter as well
  _g_time_us_tick++;
  if ( _g_time_us_tick > 10000 ) {
    _g_time_us_tick = 0;
    g_time_s ++;
  }

} // ISR for timer1

void serial_setup ( void ) {

  // set up the UART to the right baud rate (as defined in BAUD header)
  UBRR0H = UBRRH_VALUE;
  UBRR0L = UBRRL_VALUE;
#if USE_2X
  UCSR0A |= (1 << U2X0);
#else
  UCSR0A &= ~(1 << U2X0);
#endif

  // data size
  UCSR0C = _BV(UCSZ01) | _BV(UCSZ00); /* 8-bit data */ 

  // supported functions
  UCSR0B = _BV(RXEN0) | _BV(TXEN0);   /* Enable RX and TX */
  //UCSR0B |= ( 1 << TXEN0 );   /* Enable TX */

  return;
}

void uart0_send ( char *s ) {

  while ( *s ) {
    uart_putchar_prewait ( *s++ );
  }

  return;
}
