
#include "main.h"
#include "nop.h"
#include "sram.h"

#include <avr/pgmspace.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/delay.h>

void sram_write ( unsigned int address, unsigned char data ) {
  DATABUS_DIR = DIR_OUT;     //Set Data bus to output

  //PORTA = address;         //Write address out to PORTA
  PORTD &= (~0x0F);          // we're using only half a port here
  PORTD |= (address & 0x0F);

  OE_HIGH;                   // set read high (disabled)
  WE_LOW;                    // set write low (enabled)
  CE_LOW;                    // enable chip
  NOP5;

  PORTA = ( data & 0x0F );    //Write data out to PORTC
  NOP5;                       // wait a sec
  CE_HIGH;                   // disable chip

  //SetBit(PORTB, BIT(1));      //Set RW back High to create edge. 
}

unsigned char sram_read ( unsigned int address ) {
  unsigned char data = 0;
   
  DATABUS_DIR = DIR_IN;        //Set Data bus to input

  //PORTA = address;         //Set the address
  PORTD &= (~0x0F);          // we're using only half a port here
  PORTD |= (address & 0x0F);

  OE_LOW;                   // set read low (enabled)
  WE_HIGH;                    // set write high (disabled)
  CE_LOW;                    // enable chip
  NOP5;

  data = PORTA & 0x0F;
  CE_HIGH;                    // disable chip

  return data;
} 
