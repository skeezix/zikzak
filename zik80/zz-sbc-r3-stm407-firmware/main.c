
#include <stm32f4xx.h>
#include <misc.h>
#include <stm32f4xx_usart.h> // under Libraries/STM32F4xx_StdPeriph_Driver/inc and src
#include <string.h>

#include "config.h"
#include "main.h"
#include "lib_serial.h"
#include "lib_vga_cm3.h"
#include "framebuffer.h"
#include "framebuffer_demo.h"
#include "system_cm3.h"
#include "logger.h"
#include "commandqueue.h"
#include "command.h"
#include "lib_bus_ram.h"
#include "framebuffer_update.h"
#include "lib_textmode.h"
#include "HAL.h"

void usart1_isr ( void ); // shut up compiler
void usart2_isr ( void ); // shut up compiler

#define MAX_STRLEN 80 // this is the maximum string length of our string in characters
volatile char received_string [ MAX_STRLEN + 1 ]; // this will hold the recieved string

int main(void) {

  system_cm3_setup();

  log_setup();
  queue_setup();

  init_usart1 ( 38400 ); // initialize USART1 @ 9600 baud
  init_usart2 ( 38400 );
  //USART_puts ( USART1, "Init usart1 complete! Hello World!\r\n" );
  USART_puts ( USART2, "Init usart2 complete! Hello World!\r\n" );

  fb_setup();

#ifdef VGA_DMA
  vga_setup ( VGA_USE_DMA );
#else
  vga_setup ( VGA_NO_DMA );
#endif

#ifdef STARTUP_WASTE // waste some time before we start savaging the bus
  {
    unsigned int i;
    for ( i = 0; i < 60*3; i++ ) {
      _fb_spin_until_vblank();
    }
  }
#endif

#ifdef BUS_FRAMEBUFFER
  bus_setup();
#endif

  /* last video setup
   */
#ifdef RUNMODE_FRAMEBUFFER_TEST
  {
    tm_setup();

    fb_clear ( fb_active );
    //fb_test_pattern ( fb_active, fbt_topbottom );
    fb_test_pattern ( fb_active, fbt_offset_squares );
    //fb_test_pattern ( fb_active, fbt_vlines );
    //fb_test_pattern ( fb_active, fbt_v1lines );
    //fb_test_pattern ( fb_active, fbt_onoff1 );
    //fb_test_pattern ( fb_active, fbt_spriteram );
    //fb_test_pattern ( fb_active, fbt_full_palette_grid );
    //fb_test_pattern ( fb_active, fbt_full_palette_grid_ordered );
    //fb_test_pattern ( fb_active, fbt_reds );
    //fb_test_pattern ( fb_active, fbt_greens );
    //fb_test_pattern ( fb_active, fbt_blues );
  }
#endif
#ifdef RUNMODE_FRAMEBUFFER_TEST_CYCLE
  {
    tm_setup();
  }
#endif
#ifdef RUNMODE_FRAMEBUFFER_FOREVER
  {
    tm_setup();
  }
#endif

  while ( 1 ) {
    // weeeeee!

    // any work for us to do?
#ifdef RUNMODE_COMMAND_SERIAL
    if ( vblank ) {

      if ( queueready() ) {
        command_queue_run();
      }

    }
#endif

#ifdef RUNMODE_FRAMEBUFFER_FOREVER
    {
      framebuffer_update_from_ram_forever();
    }
#endif

#ifdef RUNMODE_FRAMEBUFFER_TEST_CYCLE
    {
      unsigned char p;
      unsigned long int vbl_count = _vblank_count;
      unsigned wait;

      // cycle through demos
      for ( p = 0; p < fbt_max; p++ ) {

        // skip this one.. broken right now
        if ( p == fbt_spriteram ) {
          p++;
        }

        // render
        fb_clear ( fb_inactive );
        fb_test_pattern ( fb_inactive, p );

        // swap
        fb_swap();

        // wait...
        wait = 0;
        while ( wait < 180 ) {
          vbl_count = _vblank_count;
          while ( _vblank_count == vbl_count );
          wait++;
        } // wait loop

      } // which demo

    }
#endif

    // handle queued logs
#if 0
    if ( logready() ) {
      char *log;
      while ( ( log = logpull() ) ) {
        USART_puts ( USART2, log );
      }
    }
#endif

    __asm__("nop");
  }

} // main

// cpu <-> gpu serial
void usart1_isr ( void ) {

  // check if the USART1 receive interrupt flag was set
  if ( USART_GetITStatus ( USART1, USART_IT_RXNE ) ) {

    static uint8_t cnt = 0; // this counter is used to determine the string length
    char t = USART1->DR; // the character from the USART1 data register is saved in t

    /* check if the received character is not the LF character (used to determine end of string)
     * or the if the maximum string length has been been reached
     */
    if( (t != '\n') && (cnt < MAX_STRLEN) ){
      received_string[cnt] = t;
      cnt++;
    } else { // otherwise reset the character counter and print the received string
      cnt = 0;
      //USART_puts(USART1, received_string);
      USART_puts ( USART2, "avr: " );
      USART_puts ( USART2, received_string );
      USART_puts ( USART2, "\r\n" );
    }

  } // if USART received

} // func

// PC console serial
void usart2_isr ( void ) {

  // check if the USART1 receive interrupt flag was set
  if ( USART_GetITStatus ( USART2, USART_IT_RXNE ) ) {

    static uint8_t cnt = 0; // this counter is used to determine the string length
    char t = USART2->DR; // the character from the USART1 data register is saved in t

    /* check if the received character is not the LF character (used to determine end of string)
     * or the if the maximum string length has been been reached
     */
    if( (t != '\r') && (cnt < MAX_STRLEN) ){
      received_string[cnt] = t;
      cnt++;
    } else { // otherwise reset the character counter and print the received string
      cnt = 0;

#if 0 // echo
      //USART_puts ( USART2, "console: " );
      logit ( "console: " );
      logit ( (char*) received_string );
      logit ( "\r\n" );
#endif

#if 1
      queueit ( (char*) received_string );
#endif

      //memset ( (void*) received_string, '\0', MAX_STRLEN );
    }

  } // if USART received

} // func
