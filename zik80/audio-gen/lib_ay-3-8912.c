
#include "config.h"

#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/delay.h>
#include <util/delay.h>
#include <avr/io.h>
#include <stdio.h>

#include "lib_ay-3-8912.h"

void ay_set_bc1 ( unsigned char on ) {

  if ( on ) {
    PORTD |= ( 1<<PD0 );
  } else {
    PORTD &= ~( 1<<PD0 );
  }

  return;
}

void ay_set_bdir ( unsigned char on ) {

  if ( on ) {
    PORTD |= ( 1<<PD1 );
  } else {
    PORTD &= ~( 1<<PD1 );
  }

  return;
}

void ay_mode_latch() {
  ay_set_bc1 ( 1 );
  ay_set_bdir ( 1 );
}

void ay_mode_write(){
  ay_set_bc1 ( 0 );
  ay_set_bdir ( 1 );
}

void ay_mode_read(){
  ay_set_bc1 ( 1 );
  ay_set_bdir ( 0 );
}

void ay_mode_inactive(){
  ay_set_bc1 ( 0 );
  ay_set_bdir ( 0 );
}

// a fun little arduino hack someone did:
// http://playground.arduino.cc/Main/AY38910
// --> I've swiped and rewritten a couple small functions, possible remnants survive (he uses '595 shift reg due to arduino pin limits)

void ay_write ( unsigned char address, unsigned char data ) {

  // signal all clear
  ay_mode_inactive();  
  _delay_us ( 2 );

  // set bus to address
  PORTA = address & 0x0F; // bottom 4 bits only; top 4+ bits are 'chip select' and must be 0000

  // tell Pokey to latch the address on the bus
  ay_mode_latch();  
  _delay_us ( 2 );


  // go inactive so Pokey knows that one is done
  ay_mode_inactive();
  _delay_us ( 2 );

  // set data to bus
  PORTA = data;

  // set up Pokey to take data for given address
  ay_mode_write();  
  _delay_us ( 2 );


  // go inactive so Pokey knows transmission is done
  ay_mode_inactive(); 
  _delay_us ( 2 );

  return;
}
