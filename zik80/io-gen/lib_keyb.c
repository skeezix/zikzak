// pulled some of the code from:
//
//    keyboard.c
//    for NerdKits with ATmega168
//    hevans@nerdkits.com
//

#include "config.h"

#include <stdio.h>

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <inttypes.h>

#include "lib_keyb_map.h"
#include "lib_keyb.h"

// PC4  -  CLK
// PC3  -  DATA

volatile uint8_t _g_kbd_data;
volatile uint8_t _g_char_waiting;
uint8_t _g_started;
uint8_t _g_bit_count;
uint8_t _g_shifted;
uint8_t _g_release_event;

ISR(PCINT2_vect){

  // simple state machine to determine key-code and event

  // make sure clock line is low, if not ignore this transition
  if(PINC & (1<<PC4)){
    return;
  }

  // if we have not started, check for start bit on DATA line
  if ( ! _g_started ) {
    if ( (PINC & (1<<PC3)) == 0 ) {
      _g_started = 1;
      _g_bit_count = 0;
      _g_kbd_data = 0;
      return;
    }

  } else if ( _g_bit_count < 8 ) { // we started, read in the new bit
    //put a 1 in the right place of kdb_data if PC3 is high, leave
    //a 0 otherwise
    if ( PINC & (1<<PC3) ) {
      _g_kbd_data |= (1<<_g_bit_count);
    }
    _g_bit_count++;
    return;

  } else if ( _g_bit_count == 8 ) { // parity bit 
    // TODO NYI
    _g_bit_count++;
    return;

  } else { // stop bit
    // should check to make sure DATA line is high, what to do if not?
    _g_started = 0;
    _g_bit_count = 0;
  }

  if ( _g_kbd_data == 0xF0) { // release event
    _g_release_event = 1;
    _g_kbd_data = 0;
    return;

  } else if ( _g_kbd_data == 0x12 ) { // handle shift key

    if ( _g_release_event == 0 ) {
      _g_shifted = 1;
    } else {
      _g_shifted = 0;
      _g_release_event = 0;
    }

    return;

  } else { // not a special character

    if ( _g_release_event ) { // we were in release mode - exit release mode
      _g_release_event = 0;
      //ignore that character
    } else {
      _g_char_waiting = 1;
    }

  }

}

char map_scan_code ( uint8_t data ) {
  char to_ret = pgm_read_byte(&(keymap[data])); //grab character from array
  if(_g_shifted){
    to_ret -= 0x20;
  }
  return to_ret;
}

uint8_t read_packet_blocking() {
  while(!_g_char_waiting){
     //wait for a character
  }
  _g_char_waiting = 0;
  return _g_kbd_data;
}

void init_keyboard() {

  _g_started = 0;
  _g_kbd_data = 0;
  _g_bit_count = 0;

  //make PC4 input pin
  DDRC &= ~(1<<PC4);
  //turn on pullup resistor
  PORTC |= (1<<PC4);

  // enable data line
  DDRC &= ~(1<<PC3); // in

  //Enable PIN Change Interrupt 1 - This enables interrupts on pins
  //PCINT14...8 see p70 of datasheet
  PCICR |= (1<<PCIE2);
  
  //Set the mask on Pin change interrupt 1 so that only PCINT12 (PC4) triggers
  //the interrupt. see p71 of datasheet
  PCMSK2 |= (1<<PCINT20);

}

unsigned char keyb_fetch_nonblocking ( char *r_c, unsigned char *r_keycode ) {
  uint8_t key_code = 0;

  if ( ! _g_char_waiting ) {
    return ( 0 );
  }

  key_code = read_packet_blocking();

  if ( r_keycode ) {
    *r_keycode = key_code;
  }
  if ( r_c ) {
    *r_c = map_scan_code ( key_code );
  }

  return ( 1 );
}

#if 0
int keyb_main () {

  uint8_t key_code = 0;

  char str_buf[21];
  uint8_t buf_pos = 0;
  str_buf[0] = str_buf[1] = 0x00;
  while(1) {

    if(_g_char_waiting){
      key_code = read_packet_blocking();
      if(key_code == 0x5A){ // enter key, clear the line
        buf_pos = 0;
        str_buf[0] = str_buf[1] = 0x00;
      } else if (key_code == 0x66){ //backspace
        buf_pos--;
        str_buf[buf_pos] = 0x00;
      } else {
        str_buf[buf_pos] = map_scan_code(key_code);
        buf_pos++;
        str_buf[buf_pos] = 0x00;
      }
    } 
 
#if 0
    if(_g_shifted){
      lcd_line_four();
      fprintf_P(&lcd_stream,PSTR("SHIFT"));
    } else {
      lcd_line_four();
      fprintf_P(&lcd_stream,PSTR("          "));
    }
#endif
    
  }
  
  return 0;
}
#endif
