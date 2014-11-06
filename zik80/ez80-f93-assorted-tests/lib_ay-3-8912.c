
// install path: /usr/lib/avr/include/avr

#include <eZ80.h>
#include <stdio.h>

#include "skeelib.h"
#include "lib_ay-3-8912.h"

UINT16 tp[] = {//Frequencies related to MIDI note numbers
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

UINT16 song[][2] ={
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


void ym_play_demo ( void ) {

  // set up AY mixer
  ay_write ( 0x06, 0x00 );
  ay_write ( 0x07, 0x3e );
  ay_write ( 0x08, 0x0f );

  while ( 1 ) {

    // song-hack
#if 0
    int i, j, k;
    unsigned char *p = song_hacked_ym;

    // not interleaved or 'as is'
    for ( i=0; i < framelimit; i++ ) {

      for ( j = 0; j < 16; j++ ) {
        ay_write ( j & 0x0F, pgm_read_byte(p) );
        p++;
      }

      delay_ms_spin ( 10 ); // 20

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

  } // while forever

  return;
}

void ay_set_chA ( int i ) {
  ay_write ( 0x00, tp[i]&0xff );
  ay_write ( 0x01, (tp[i] >> 8)&0x0f );    
}

void ay_set_chB ( int i ) {
  ay_write ( 0x02, tp[i]&0xff );
  ay_write ( 0x03, (tp[i] >> 8)&0x0f );
}

#pragma noopt
#define BC1_0 ((UINT8*)0x2C0000)
#define BC1_1 ((UINT8*)0x2C0001)
UINT8 ay_write ( unsigned char address, unsigned char data ) {

	// latch operation: bc1=1 bdir=1
	// inactive operation: bc1=0 bdir=0
	// write operation: bc1=0 bdir=1
	// read operation: bc1=1 bdir=0
	
	// bc1 == A0
	// bdir = read/write, no worries
	// A8 is chipselect, no worries

	// so set _our_ bus address to 0x2C000[01] to do bc1 value
	// latch
	// 	 send address into data bus
	// inactive
	// send data into data bus
	// inactive

	UINT8 *psg;
	UINT8 dummy;
	
	// inactive
	psg = BC1_0;
	dummy = *psg;
	
	// latch and write address
	psg = BC1_1;
	*psg = address;
	
	// inactive
	psg = BC1_0;
	dummy = *psg;
	
	// write data
	psg = BC1_0;
	*psg = data;
	
	// inactive
	psg = BC1_0;
	dummy = *psg;
	
	return ( dummy );
}
