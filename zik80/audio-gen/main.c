
// install path: /usr/lib/avr/include/avr

#include "config.h"

#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/delay.h>
#include <util/delay.h>
#include <avr/io.h>
#include <stdio.h>
#include <avr/pgmspace.h> 

#include "main.h"


void ay_set_chA ( int i );
void ay_set_chB ( int i );
void delay_ms_spin ( unsigned int ms );
extern unsigned char song_hacked_ym[];

int tp[] = {//Frequencies related to MIDI note numbers
  15289, 14431, 13621, 12856, 12135, 11454, 10811, 10204,//0-o7
  9631, 9091, 8581, 8099, 7645, 7215, 6810, 6428,//8-15
  6067, 5727, 5405, 5102, 4816, 4545, 4290, 4050,//16-23
  3822, 3608, 3405, 3214, 3034, 2863, 2703, 2551,//24-31
  2408, 2273, 2145, 2025, 1911, 1804, 1703, 1607,//32-39
  1517, 1432, 1351, 1276, 1204, 1136, 1073, 1012,//40-47
  956, 902, 851, 804, 758, 716, 676, 638,//48-55
  602, 568, 536, 506, 478, 451, 426, 402,//56-63
  379, 358, 338, 319, 301, 284, 268, 253,//64-71
  239, 225, 213, 201, 190, 179, 169, 159,//72-79
  150, 142, 134, 127, 119, 113, 106, 100,//80-87
  95, 89, 84, 80, 75, 71, 67, 63,//88-95
  60, 56, 53, 50, 47, 45, 42, 40,//96-103
  38, 36, 34, 32, 30, 28, 27, 25,//104-111
  24, 22, 21, 20, 19, 18, 17, 16,//112-119
  15, 14, 13, 13, 12, 11, 11, 10,//120-127
  0//off
};

int song[][2] ={
  {60,500},
  {62,500},
  {64,500},
  {65,500},
  {64,500},
  {62,500},
  {60,500},
  {128,500},
  {64,500},
  {65,500},
  {67,500},
  {69,500},
  {67,500},
  {65,500},
  {64,500},
  {128,500},  
  {60,500},
  {128,500},
  {60,500},
  {128,500},
  {60,500},
  {128,500},
  {60,500},
  {128,500},  
  {60,128},
  {128,128},
  {60,128},
  {128,128},
  {62,128},
  {128,128},
  {62,128},
  {128,128},
  {64,128},
  {128,128},
  {64,128},
  {128,128},
  {65,128},
  {128,128},
  {65,128},
  {128,128},
  {64,250},
  {128,250},
  {62,250},
  {128,250},
  {60,250},
  {128,1000}  
};


// direction
//   ADR (analog direction register) and DDR (digital direction register)
//   DDRx -> set value to 1, sets dir to write; low is read
//   PORTxn -> set high; if write, sets to high; if read, sets pull up resistor
// interupts
//   GIMSK = _BV (INT0); // int - Enable external interrupts int0
//   MCUCR = _BV (ISC01); // int - INT0 is falling edge
//   sei(); // int - Global enable interrupts




int main ( void ) {

  /*
   * setup
   */

  // directions
  //
  DDRB = 0x00; // all input
  DDRB |= (1<<PB0); // LED out

  DDRA = 0xFF; // all output - data bus to sound chip
  DDRD = 0xFF; // all output - control bus to sound chip

  //DDRC = 0xFF; // all output // unused


  /*
   * doit
   */

  // enable interupts
  sei();

  // blink test, so we're ready..
#if 0
  blink_forever();
#endif

  // set up AY mixer
  ay_write ( 0x06, 0x00 );
  ay_write ( 0x07, 0x3e );
  ay_write ( 0x08, 0x0f );

  while ( 1 ) {

    // song-hack
#if 0
    int i, j, k;
    unsigned char *p = song_hacked_ym;

    // not interleaved
    for ( i=0; i< 500; i++ ) {

      for ( j = 0; j < 16; j++ ) {
        ay_write ( j & 0x0F, pgm_read_byte(p) );
        p++;
      }

      delay_ms_spin ( 16 ); // 20

    }

#endif

    // test song
#if 0
    int i;
    for ( i=0; i< 47; i++ ) {
      ay_set_chA ( song[i][0] );
     delay_ms_spin ( song[i][1]);   
    }
#endif

#if 1
    int i;

    for(i=0;i<8;i++){
      ay_set_chA(48);
      delay_ms_spin(100);
      ay_set_chA(60);   
      delay_ms_spin(100);
    }  

    for( i=0;i<4;i++){
      ay_set_chA(53);
      delay_ms_spin(100);
      ay_set_chA(65);
      delay_ms_spin(100);
    }

    for( i=0;i<4;i++){
      ay_set_chA(55);
      delay_ms_spin(100);
      ay_set_chA(67);
      delay_ms_spin(100);
    }
#endif

#if 0
    //Gunshot like sound KICK 
    ay_write ( 0x06, 0x11);
    ay_write ( 0x07, 0x07);
    ay_write ( 0x08, 0x10);
    ay_write ( 0x09, 0x10);
    ay_write ( 0x0a, 0x10);
    ay_write ( 0x0c, 0x10);
    ay_write ( 0x0d, 0x00);
    _delay_ms ( 500 ); 

    ay_write ( 0x06, 0x00);
    ay_write ( 0x07, 0x07);
    ay_write ( 0x08, 0x10);
    ay_write ( 0x09, 0x10);
    ay_write ( 0x0a, 0x10);
    ay_write ( 0x0c, 0x38);
    ay_write ( 0x0d, 0x00);
    _delay_ms ( 500 );  
#endif

    PORTB ^= (1<<PB0);

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




void ay_set_chA ( int i ) {
  ay_write ( 0x00, tp[i]&0xff );
  ay_write ( 0x01, (tp[i] >> 8)&0x0f );    
}

void ay_set_chB ( int i ) {
  ay_write ( 0x02, tp[i]&0xff );
  ay_write ( 0x03, (tp[i] >> 8)&0x0f );
}

void delay_ms_spin ( unsigned int ms ) {
  while ( --ms ) {
    _delay_ms ( 1 );
  }
}
