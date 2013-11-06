
// derived from zikzak3-gpu-addrtest
// --> working pretty well, having hard time keeping up with VGA as usual
// .. you're either rendering (making up data on the fly, or static data) fine,
// OR .. you're pulling data from SRAM or local RAM, and having very resitricted bandwidth
// --> what to do, for this limited project

// see in thread on openpandora boards for ideas

// Oct 22 -> retooling, see how this approach from Nick Gammon helps; I've avoided interupts
// and timing so far, using pure code in ASM and C, on the assumption the cycle overhead for
// interupt context switch itself would eat too many pixels; but his approach below is using
// the interupts more during vblanks and per line, and seems to work... on arduino, 16mhz
// --> .. lets see if its any better than mine:
//
// http://www.gammon.com.au/forum/?id=11608
// -> result:
//    - same waveyness as my C approaches .. so really need to do asm, but I knew that ;) I like to see what I can pull off..
//    - same resolution limit .. just can't get past the basic memory fetch cost killing us here
//   --> maybe can go monochrome .. less RAM, though the shift/masking may kill us.. but less memory fetch perhaps? Need to toy with it..
//  --> or maybe go propelleer with a higher res text mode and possible graphics, one cog talking to main board, others doing artwork..
// --> or go to a more modern chip, or a faster one (SX 80MHz?), or a dedicated board, or discrete VGA board, or old PC VGA card..

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

#include "TimerHelpers.h"

// direction
//   ADR (analog direction register) and DDR (digital direction register)
//   DDRx -> set value to 1, sets dir to write; low is read
//   PORTxn -> set high; if write, sets to high; if read, sets pull up resistor
// interupts
//   GIMSK = _BV (INT0); // int - Enable external interrupts int0
//   MCUCR = _BV (ISC01); // int - INT0 is falling edge
//   sei(); // int - Global enable interrupts

/* Inspired by code from Nick Gammon - using USART to spit out data at well timed
 * intervals -- genius :)
 */

/*
 D1 : Pixel output (180 ohms in series) (connect to R, G, B tied together)
 D3 : Horizontal Sync (68 ohms in series)
 D10 : Vertical Sync (68 ohms in series)
 
 Gnd : --> Pins 5, 6, 7, 8, 10 on DB15 socket
*/

#include <avr/pgmspace.h>
#include "screen-font.h"
//#include "screen-font-rev.h"
#include <avr/sleep.h>

const byte MSPIM_SCK = 4;    // <-- we aren't using it directly

const int horizontalBytes = 20;  // 160 pixels wide
const int verticalPixels = 480;  // 480 pixels high

// Timer 1 - Vertical sync

// output    OC1B   pin 16  (D10) <------- VSYNC

//   Period: 16.64 mS (60 Hz)
//      1/60 * 1e6 = 16666.66 uS
//   Pulse for 64 uS  (2 x HSync width of 32 uS)
//    Sync pulse: 2 lines
//    Back porch: 33 lines
//    Active video: 480 lines
//    Front porch: 10 lines
//       Total: 525 lines

// Timer 2 - Horizontal sync

// output    OC2B   pin 5  (D3)   <------- HSYNC

//   Period: 32 uS (31.25 kHz)
//      (1/60) / 525 * 1e6 = 31.74 uS
//   Pulse for 4 uS (96 times 39.68 nS)
//    Sync pulse: 96 pixels
//    Back porch: 48 pixels
//    Active video: 640 pixels
//    Front porch: 16 pixels
//       Total: 800 pixels

// Pixel time =  ((1/60) / 525 * 1e9) / 800 = 39.68  nS
//  frequency =  1 / (((1/60) / 525 * 1e6) / 800) = 25.2 MHz

// However in practice, it is the SPI speed, namely a period of 125 nS
//     (that is 2 x system clock speed)
//   giving an 8 MHz pixel frequency. Thus the characters are about 3 times too wide.
// Thus we fit 160 of "our" pixels on the screen in what usually takes 3 x 160 = 480

const byte screenFontHeight = 8;
const byte screenFontWidth = 8;

const int verticalLines = verticalPixels / screenFontHeight / 2;  // double-height characters
const int horizontalPixels = horizontalBytes * screenFontWidth;

const byte verticalBackPorchLines = 35;  // includes sync pulse?
const int verticalFrontPorchLines = 525 - verticalBackPorchLines;

volatile int vLine;
volatile int messageLine;
volatile byte backPorchLinesToGo;

char textbuf [verticalLines]  [horizontalBytes];

// ISR: Vsync pulse
volatile unsigned char _g_blankcount;
ISR (TIMER1_OVF_vect) {
  vLine = 0;
  messageLine = 0;
  backPorchLinesToGo = verticalBackPorchLines;

  _g_blankcount++;
  if ( _g_blankcount == 60 ) {
    textbuf [ 0 ][ 2 ] = '_';
    textbuf [ 0 ][ 4 ] = '_';
    _g_blankcount = 0;
  }

} // end of TIMER1_OVF_vect
  
// ISR: Hsync pulse ... this interrupt merely wakes us up
ISR (TIMER2_OVF_vect) {
} // end of TIMER2_OVF_vect
    
void setup() {
  
  // initial message ... change to suit
  for (int i = 0; i < verticalLines; i++) {
    sprintf (textbuf [i], "%02i pandora foo", i);
  }
   
  // disable Timer 0
  TIMSK0 = 0;  // no interrupts on Timer 0
  OCR0A = 0;   // and turn it off
  OCR0B = 0;
  
  // Timer 1 - vertical sync pulses
  DDRD |= (1<<PD4); // vsync
  Timer1::setMode (15, Timer1::PRESCALE_1024, Timer1::CLEAR_B_ON_COMPARE);
  OCR1A = 259;  // 16666 / 64 uS = 260 (less one)
  OCR1B = 0;    // 64 / 64 uS = 1 (less one)
  TIFR1 = bit (TOV1);   // clear overflow flag
  TIMSK1 = bit (TOIE1);  // interrupt on overflow on timer 1

  // Timer 2 - horizontal sync pulses
  DDRD |= (1<<PD6); // hsync
  Timer2::setMode (7, Timer2::PRESCALE_8, Timer2::CLEAR_B_ON_COMPARE);
  OCR2A = 63;   // 32 / 0.5 uS = 64 (less one)
  OCR2B = 7;    // 4 / 0.5 uS = 8 (less one)
  TIFR2 = bit (TOV2);   // clear overflow flag
  TIMSK2 = bit (TOIE2);  // interrupt on overflow on timer 2
 
  // Set up USART in SPI mode (MSPIM)
  
  // baud rate must be zero before enabling the transmitter
  UBRR0 = 0;  // USART Baud Rate Register
  //pinMode (MSPIM_SCK, OUTPUT);   // set XCK pin as output to enable master mode
  DDRB |= (1<<PB0);
  UCSR0B = 0; 
  UCSR0C = bit (UMSEL00) | bit (UMSEL01) | bit (UCPHA0) | bit (UCPOL0);  // Master SPI mode

  // prepare to sleep between horizontal sync pulses  
  set_sleep_mode ( SLEEP_MODE_IDLE );  

  DDRD |= (1<<PD0); // red
  DDRD |= (1<<PD1); // green
  DDRD |= (1<<PD2); // blue

  // JOY: set up pin change interupts
  EICRA &= ~ ( (1 << ISC11) | (1 << ISC10) ); // reset: clear ISC11+ISC10 (ISC1x for vect1 which we need for joy)
  EICRA |= ( (1 << ISC10) ); // 00 set and 01 unset means any edge will make event
  PCMSK1 |= ( (1 << PCINT10) | (1 << PCINT11) | (1 << PCINT12) | (1 << PCINT13) | (1 << PCINT14) ); // Pins to monitor
  PCICR |= (1 << PCIE1); // PB is monitored
  DDRB = 0x00; // all input

  // KEYB: set up pin change interupts
  EICRA &= ~ ( (1 << ISC21) | (1 << ISC20) ); // reset: clear ISC11+ISC10 (ISC1x for vect1 which we need for joy)
  //EICRA |= ( (1 << ISC20) ); // 00 set and 01 unset means any edge will make event
  EICRA |= ( (1 << ISC21) ); // x1 set and x0 unset means falling edge will trigger
  PCMSK2 |= ( (1 << PCINT22) ); // Pins to monitor
  PCICR |= (1 << PCIE2); // PC is monitored
  DDRC = 0x00; // all input

  // enable interupts or waking from sleep won't happen
  sei();
 
}  // end of setup

// draw a single scan line
void doOneScanLine ( void ) {
    
  // after vsync we do the back porch
  if (backPorchLinesToGo) {
    backPorchLinesToGo--;
    return;   
  }  // end still doing back porch
    
  // if all lines done, do the front porch
  if ( vLine >= verticalPixels ) {
    return;
  }
    
  // pre-load pointer for speed
  const register byte * linePtr = &screen_font [ (vLine >> 1) & 0x07 ] [0];
  register char * messagePtr =  & (textbuf [messageLine] [0] );

  // how many pixels to send
  register byte i = horizontalBytes;

  // turn transmitter on 
  UCSR0B = bit (TXEN0);  // transmit enable (starts transmitting white)

  // blit pixel data to screen    
  while (i--) {
    UDR0 = pgm_read_byte (linePtr + (* messagePtr++));
  }

  // wait till done    
  while (!(UCSR0A & bit(TXC0))) {
  }
  
  // disable transmit
  UCSR0B = 0;   // drop back to black
  
  // this line done
  vLine++;  
  
  // every 16 pixels it is time to move to a new line in our text
  //  (because we double up the characters vertically)
  if ((vLine & 0xF) == 0) {
    messageLine++;
  }

  return;
}  // end of doOneScanLine

int main ( void ) {

  setup();

  while ( 1 ) {
    // sleep to ensure we start up in a predictable way
    sleep_mode ();
    doOneScanLine ();
  }

}  // end of loop

volatile unsigned char _g_joy_state = 0; // so we can know which pins changed since last interupt
#define JOYMASK ( (1<<PB2) | (1<<PB3) | (1<<PB4) | (1<<PB5) | (1<<PB6) )
ISR(PCINT1_vect)
{
  unsigned char delta = _g_joy_state ^ PINB;

  if ( delta & (1<<PB2) ) {

    // CH1: edge has gone up, or down?
    if ( PINB & ( 1 << PB2 ) ) {
      // JOY UP: edge has gone high (nolonger pointing up)
      textbuf [ 0 ][ 0 ] = ' ';

    } else {
      // JOY UP: edge has gone low - joy is pointing up!
      textbuf [ 0 ][ 0 ] = 'X';

    }

  } // changed?

  // store current pin state
  _g_joy_state = PINB;

} // PCINT1_vect

ISR(PCINT2_vect) {
  // ps/2 protocol is: http://www.computer-engineering.org/ps2keyboard/

  // no delta/state is needed here; we're only triggered on falling edge of clock
  // ps/2 protocol has clock only when its sending us data (or when we're sending it data,
  // which we don't bother with); as such, when we start getting interupted, we know a
  // piece of data is inbound, just need to collect it up.

  textbuf [ 0 ][ 4 ] = 'C';

  if ( PINC & ( 1 << PC5 ) ) {
    textbuf [ 0 ][ 2 ] = 'K';
  }

} // PCINT2_vect
