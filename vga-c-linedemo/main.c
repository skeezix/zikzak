
#define F_CPU 20000000UL  /* 20 MHz CPU clock */

#include <avr/pgmspace.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/delay.h>

#include <string.h>
#include <stdlib.h>

#include "main.h"

// skee board
// PC 01 RR
// PC 23 GG
// PC 45 BB
// PB 0 vsync (pin 1)
// PB 1 hsync (pin 2)

#define VSYNC_LOW    PORTB &= ~_BV(0)
#define VSYNC_HIGH   PORTB |= _BV(0)

#define HSYNC_LOW    PORTB &= ~_BV(1)
#define HSYNC_HIGH   PORTB |= _BV(1)

#define RED_OFF      PORTC &= ~_BV(1)
#define RED_ON       PORTC |= _BV(1)

#define GREEN_OFF    PORTC &= ~_BV(3)
#define GREEN_ON     PORTC |= _BV(3)

#define BLUE_OFF     PORTC &= ~_BV(5)
#define BLUE_ON      PORTC |= _BV(5)

void main ( void ) {
  int i = 0;

  cli();

  DDRB = 0xFF; // B out
  DDRC = 0xFF; // C out
  
  //Loop Over-And-Over Again
  while ( 1 ) {

    i=0;
    //VSYNC Low
    VSYNC_LOW;
    //200 Lines Of Red
    while ( i < 200 ) {

      RED_ON;
          
      // 2.2uS Back Porch
      _delay_us ( 2 );  
      __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t");
          
      //20uS Color Data
      _delay_us ( 20 ); // 1uS        
          
      RED_OFF;
          
      // 1uS Front Porch
      _delay_us(1); // 1uS 

      i++;
          
      // 3.2uS Horizontal Sync
      HSYNC_HIGH;

      _delay_us(3);
      __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t");

      HSYNC_LOW;
          
      // 26.4uS Total
    }


    i = 0;
    // VSYNC Low 
    VSYNC_LOW;
    // 200 Lines Of Green
    while ( i < 200 ) {

      GREEN_ON;      
            
      // 2.2uS Back Porch
      _delay_us(2);
      __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t");
          
      // 20uS Color Data
      _delay_us(20); // 1uS        
          
      GREEN_OFF;
          
      // 1uS Front Porch
      _delay_us ( 1 ); // 1uS 
      i++;
          
      // 3.2uS Horizontal Sync
      HSYNC_HIGH;

      _delay_us(3);
      __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t");

      HSYNC_LOW;
          
      // 26.4uS Total
    }    
  
    i = 0;
    // VSYNC Low 
    VSYNC_LOW;
    // 200 Lines Of Blue
    while ( i < 200 ) {

      BLUE_ON;

      // 2.2uS Back Porch
      _delay_us(2);
      __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t");
          
      // 20uS Color Data
      _delay_us ( 20 ); // 1uS        
          
      BLUE_OFF;
          
      // 1uS Front Porch
      _delay_us ( 1 ); // 1uS 
      i++;
          
      // 3.2uS Horizontal Sync
      HSYNC_HIGH;
      _delay_us ( 3 );
      __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t");

      HSYNC_LOW;
          
      //26.4uS Total
    }    

    i = 0;
    // VSYNC High
    VSYNC_HIGH;
    // 4 Lines Of VSYNC   
    while ( i < 4 ) {         
      // 2.2uS Back Porch    
      _delay_us(2);
      __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t");
          
      // 20 uS Of Color Data
      _delay_us(20);// 20uS
          
      // 1uS Front Porch
      _delay_us(1); // 1uS
      i++;
          
      // HSYNC for 3.2uS
      HSYNC_HIGH;

      _delay_us(3);
      __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t");

      HSYNC_LOW;
          
      //26.4uS Total
    }
      
    i = 0;
    //VSYNC Low
    VSYNC_LOW;
    // 23 Lines Of Vertical Back Porch + 1 Line Of Front Porch
    // This is very, very flexible...I'm setting it to 22 Lines!
    while ( i < 22 ) {
      // 2.2uS Back Porch
      _delay_us(2);
      __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t");
    
      // 20uS Color Data
      _delay_us ( 20 );
            
      // 1uS Front Porch
      _delay_us(1); // 1uS
      i++;
          
      // HSYNC for 3.2uS
      HSYNC_HIGH;

      _delay_us(3);
      __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t");

      HSYNC_LOW;  //Low  
          
      // 26.4uS Total
    }

  } // while forever

} // main
