#include <eZ80.h>
#include <stdio.h>

#include <gpio.h>
//#include <intvect.h>

#include "skeelib.h"
#include "lib_ps2_keyboard.h"
#include "lib_ps2_keymap.h"

// ---[ ISR follows ]-------------------------------------------
//#define LOGRAM_ON

uint8_t _g_started = 0;
uint8_t _g_bit_count = 0;
uint8_t _g_shifted = 0;
uint8_t _g_release_event = 0;

uint8_t _g_kbd_data = 0;
uint8_t _g_char_waiting = 0;
uint8_t _g_databit = 0;

#if 0 // hacking around
#define GPIO_DATAPORT    PC_DR
#define GPIO_CLOCK_ISR   PORTC1_IVECT
#define GPIO_CLOCK_PIN   PORTPIN_ONE
#define GPIO_DATA_PIN    PORTPIN_TWO
#define GPIO_OPEN_F		 open_PortC
#define GPIO_CONTROL_F	 control_PortC
#define GPIO_SETMODE_F	 setmode_PortC
#endif

#if 1 // zikzak sbc rev3 intended
#define GPIO_DATAPORT    PD_DR
#define GPIO_CLOCK_ISR   PORTD7_IVECT
#define GPIO_CLOCK_PIN   PORTPIN_SEVEN
#define GPIO_DATA_PIN    PORTPIN_SIX
#define GPIO_OPEN_F		 open_PortD
#define GPIO_CONTROL_F	 control_PortD
#define GPIO_SETMODE_F	 setmode_PortD
#endif

#ifdef LOGRAM_ON // DEBUG: populate a received keycode bits into RAM
uint8_t *_g_logram = (uint8_t*) 0x0D0000;
#endif

static interrupt void keyb_clock_isr ( void ) {
  // simple state machine to determine key-code and event

  _g_databit = GPIO_DATAPORT & GPIO_DATA_PIN;
	
  // let ISR trigger again
  GPIO_DATAPORT |= (GPIO_CLOCK_PIN);
  GPIO_DATAPORT &= ~(GPIO_CLOCK_PIN);

  // make sure clock line is low, if not ignore this transition
  // WTF: If we set to interupt on falling edge, or active low why the heck are we seeing high value on pin EVER?
#if 0
  if ( GPIO_DATAPORT & (GPIO_CLOCK_PIN)) {
    return;
  }
#endif

  // if we have not started, check for start bit on DATA line
  if ( ! _g_started ) {
      if ( ! _g_databit ) {
		_g_started = 1;
		_g_bit_count = 0;
		_g_kbd_data = 0;
		
#ifdef LOGRAM_ON
		_g_logram [ 0 ] = 0; _g_logram [ 1 ] = 0; _g_logram [ 2 ] = 0; _g_logram [ 3 ] = 0;
		_g_logram [ 4 ] = 0; _g_logram [ 5 ] = 0; _g_logram [ 6 ] = 0; _g_logram [ 7 ] = 0;
#endif
		return;
	  }

  } else if ( _g_bit_count < 8 ) { // we started, read in the new bit
    //put a 1 in the right place of kdb_data if PC3 is high, leave
    //a 0 otherwise
    if ( _g_databit ) {
      _g_kbd_data |= (1<<_g_bit_count);
#ifdef LOGRAM_ON
		_g_logram [ _g_bit_count ] = 1;
#endif
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

#ifdef LOGRAM_ON
	  _g_logram [ 9 ] = 0;
#endif

	}

  }

  return;
}


// ---[ setup follows ]-------------------------------------------

// Z PD7 -> CLK -> need ISR so it knows when to detect a packet
// Z PD6 -> DATA
void keyb_setup ( void ) {
	UCHAR err;

	_set_vector ( GPIO_CLOCK_ISR, keyb_clock_isr );
	
#ifdef LOGRAM_ON
  _g_logram [ 10 ] = 0;
#endif

	// GPIOMODE_INTERRUPTACTVLOW
	// GPIOMODE_INTERRUPTACTVHIGH
	// GPIOMODE_INTERRUPTFALLEDGE
	// GPIOMODE_INTERRUPTRISEEDGE
	// GPIOMODE_INTERRUPTDUALEDGE
	GPIO_DATAPORT |= (GPIO_CLOCK_PIN);
	err = GPIO_SETMODE_F ( GPIO_CLOCK_PIN, GPIOMODE_INTERRUPTFALLEDGE );
	GPIO_DATAPORT &= ~(GPIO_CLOCK_PIN);
	err = GPIO_SETMODE_F ( GPIO_DATA_PIN, GPIOMODE_INPUT );
	
	return;
}


char map_scan_code ( uint8_t data ) {
  char to_ret = keymap[data]; //grab character from array
  if(_g_shifted){
    to_ret -= 0x20;
  }
  return to_ret;
}

uint8_t packet_fetch_blocking() {
  while(!_g_char_waiting){
     //wait for a character
  }
  _g_char_waiting = 0;
  return _g_kbd_data;
}

unsigned char keyb_fetch_nonblocking ( char *r_c, unsigned char *r_keycode ) {
  uint8_t key_code = 0;

  if ( ! _g_char_waiting ) {
    return ( 0 );
  }

  key_code = packet_fetch_blocking();

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
