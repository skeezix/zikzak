
#include "main.h"
#include "nop.h"
#include "sram.h"

#include <avr/pgmspace.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/delay.h>

void sram_reset ( void ) {
  CE_HIGH;     // disable chip
  WE_HIGH;     // disable write
  OE_LOW;      // enable read
}

void sram_write ( unsigned int address, unsigned char data ) {
  // OE - X, WE - L    .. does X mean 'don't care' or 'high impedance? Z should be impedance..'

#if 0 // do we need to bring high impedance up?
  DDRD = 0xFF;
  DDRD &= ~(1<<5); // set OE to read
  OE_HIGH;         // set OE to high (result: high impedence)
#endif

  // address
  PORTD &= (~0x0F);          // we're using only half a port here
  PORTD |= (address & 0x0F);

  // data
  DATABUS_DIR = DIR_OUT;            // set data to output
  DATABUS_PORT = ( data & 0x0F );   // write data out

  OE_HIGH;                   // set read high (disabled)
  WE_LOW;                    // set write low (enabled)
  CE_LOW;                    // enable chip
  NOP5;

  CE_HIGH;                   // disable chip
  NOP5;

  PORTA = 0;                 // clear the write-buffer

  return;
}

unsigned char sram_read ( unsigned int address ) {
  // OE - low, WE - high
  unsigned char data = 0;
   
  PORTA = 0;                 // clear the read-buffer

#if 0
  DDRD = 0xFF;
#endif

  // address
  PORTD &= (~0x0F);          // we're using only half a port here
  PORTD |= (address & 0x0F);

  // data
  DATABUS_DIR = DIR_IN;        //Set Data bus to input

  OE_LOW;                   // set read low (enabled)
  WE_HIGH;                    // set write high (disabled)
  CE_LOW;                    // enable chip
  NOP5;

  data = DATABUS_PORT & 0x0F;
  CE_HIGH;                    // disable chip
  NOP5;

  return data;
} 
