
#include <stm32f4xx.h>
#include <misc.h>
#include <stm32f4xx_usart.h> // under Libraries/STM32F4xx_StdPeriph_Driver/inc and src
#include <string.h>

#include "config.h"
#include "main.h"
#include "lib_serial.h"
//#include "/home/skeezix/archive/devo/toolkits/stm32/libopencm3/include/libopencm3/stm32/f4/nvic.h"
#include "lib_vga_cm3.h"
#include "framebuffer.h"
#include "framebuffer_demo.h"
#include "system_cm3.h"
#include "logger.h"
#include "commandqueue.h"
#include "command.h"
#include "lib_bus_ram.h"

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

#ifdef BUS_FRAMEBUFFER
  bus_setup();
#endif

  //fb_test_pattern ( fb_active, fbt_topbottom );
  //fb_test_pattern ( fb_active, fbt_offset_squares );
  //fb_test_pattern ( fb_active, fbt_vlines );
  //fb_test_pattern ( fb_active, fbt_v1lines );
  //fb_test_pattern ( fb_active, fbt_onoff1 );
  fb_test_pattern ( fb_active, fbt_spriteram );

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
    if ( vblank ) {
      queueit ( "BD" );
      command_queue_run();
    }
#endif

    // update framebuffers
#if 0
    if ( vblank ) {
      unsigned char i = 0;
      fb_clone ( fb_2, fb_active );
      while ( vblank ) {
        __asm__("nop");
      }
      fb_test_pattern ( fb_2, i & 0x03 );
      i++;
    }
#endif

    // check for external RAM updates?
#ifdef zzBUS_FRAMEBUFFER
    static uint16_t _done = 0;
    _done++;
    if ( vblank && _done > 30 && _done < 40  ) {

      bus_grab_and_wait();

      uint32_t addr = 0x1C0000;
      uint8_t v;
      uint8_t i;
      char b [ 2 ];

      USART_puts_optional ( USART2, "+REM cart dump: " );

      for ( i = 0; i < 20; i++ ) {
        v = bus_perform_read ( addr );

        b [ 0 ] = v;
        b [ 1 ] = '\0';
        USART_puts_optional ( USART2, b );

        addr++;
      }

      USART_puts_optional ( USART2, "+++\n" );

      bus_release();

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
