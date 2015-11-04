
// install path: /usr/lib/avr/include/avr

#include <eZ80.h>
#include <stdio.h>
#include <gpio.h>

#include "skeelib.h"
#include "lib_ay-3-8912.h"


// frequencies related to MIDI note numbers
UINT16 tp[] = {
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

#if 1 // stairway
	{
		uint16_t progression [ 20 ] = {
			A_FREQ, Asharp_FREQ,
			B_FREQ,
			C_FREQ, Csharp_FREQ,
			D_FREQ, Dsharp_FREQ,
			E_FREQ,
			F_FREQ, Fsharp_FREQ,
			G_FREQ, Gsharp_FREQ,
			0 /* end */
		};
		
		uint8_t i = 0;
		
		while (1) {

			// play note
			
			ay_write ( 0x08, 0x0F );                        // register 8, full volume
			ay_write ( 0x00, progression [ i ] & 0x0FF );   // register 0, value (freq bottom half)
			ay_write ( 0x01, progression [ i ] >> 8 );      // register 1, value (freq top half)
			ay_write ( 0x07, 0x3E );                        // register 7, Enable output Channel A (0011 1110)
			
			delay_ms_spin(70);

			// next
			i++;
			
			if ( progression [ i ] == 0 ) {
				i = 0;
			}
			
		} // while
		
	}
#endif
	
#if 0
    int i;

    for(i=0;i<8;i++){
      ay_set_chA(48);
      delay_ms_spin(70);
      ay_set_chA(60);   
      delay_ms_spin(70);
    }  

    for( i=0;i<4;i++){
      ay_set_chA(53);
      delay_ms_spin(70);
      ay_set_chA(65);
      delay_ms_spin(70);
    }

    for( i=0;i<4;i++){
      ay_set_chA(55);
      delay_ms_spin(70);
      ay_set_chA(67);
      delay_ms_spin(70);
    }
#endif

#if 0 // effect .. laser shot?
	while (1) {
		// decaying laser shot; more like a ball bouncing down into floor
		ay_write ( 0x06, 0x11);
		ay_write ( 0x07, 0x07);
		ay_write ( 0x08, 0x10);
		ay_write ( 0x09, 0x10);
		ay_write ( 0x0a, 0x10);
		ay_write ( 0x0c, 0x10);
		ay_write ( 0x0d, 0x00);
		delay_ms_spin ( 200 ); 

		// echoey noise ba ba ba baaa..
		ay_write ( 0x06, 0x00);
		ay_write ( 0x07, 0x07);
		ay_write ( 0x08, 0x10);
		ay_write ( 0x09, 0x10);
		ay_write ( 0x0a, 0x10);
		ay_write ( 0x0c, 0x38);
		ay_write ( 0x0d, 0x00);
		delay_ms_spin ( 200 );  
		
		// next
		delay_ms_spin ( 400 );  
				
	} // while
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

void ay_gpio_setup ( void ) {

	PORT pc;
	UCHAR err;
		
	pc.dr = 0x01;
	pc.ddr = 0; // 0 ddr is output
	//pc.alt0 = 0; // does not exist for F93
	pc.alt1 = 0;
	pc.alt2 = 0;
		
	open_PortB ( &pc );
	//control_PortC ( & pc);

	err = setmode_PortB ( PORTPIN_ALL, GPIOMODE_OUTPUT );
		
	SETDR_PORTB ( 1 );

	return;
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

	//PB_DR ^= 1; // TEST

	// inactive
	psg = BC1_0;
	dummy = *psg;
    delay_loop ( 20 );
	
	// latch and write address
	PB_DR = address & 0x0F;
	psg = BC1_1;
	*psg = address & 0x0F;
    delay_loop ( 20 );
	
	// inactive
	psg = BC1_0;
	dummy = *psg;
    delay_loop ( 20 );
	
	// write data
	PB_DR = data;
	psg = BC1_0;
	*psg = data;
    delay_loop ( 20 );
	
	// inactive
	psg = BC1_0;
	dummy = *psg;
    delay_loop ( 20 );
	
	//PB_DR = 0;
	
	return ( dummy );
}
