
#include <stm32f4xx.h>
#include <string.h>

#include "config.h"
#include "lib_serial.h"

#include "command.h"
#include "commandqueue.h"
#include "logger.h"
#include "framebuffer.h"
#include "framebuffer_demo.h"
#include "lib_bus_ram.h"
#include "skeelib.h"
#include "lib_i2c.h"
#include "HAL.h"

static void lamecopy ( uint8_t *dest, char *src, uint16_t len ) {

  while ( len ) {
    *dest++ = *src++;
    len--;
  }

  return;
}

void command_queue_run ( void ) {
  char *c;

  while ( ( c = queuepull() ) ) {

    //switch ( *((uint16_t*)c) ) {
    switch ( c[0] + (c[1]<<8) ) {

    case LF:
      usart_puts_optional_set ( 0 );
      break;

    case LO:
      usart_puts_optional_set ( 1 );
      break;

    case DL:
      zl_render_line ( fb_active, c [ 2 ], c [ 3 ], c [ 4 ], c [ 5 ], c [ 6 ] );
      USART_puts_optional ( USART2, "+OK DL\n" );
      break;

    case DF:
      fb_render_rect_filled ( fb_active, c [ 3 ], c [ 4 ], c [ 5 ], c [ 6 ], c [ 2 ] );
      USART_puts_optional ( USART2, "+OK DF\n" );
      break;

    case CB:
      fb_render_rect_filled ( fb_active, 0, 0, FBWIDTH - 1, FBHEIGHT - 1, 0x00 );
      USART_puts_optional ( USART2, "+OK CB\n" );
      break;

    case DP:
      fb_test_pattern ( fb_active, fbt_offset_squares );
      USART_puts_optional ( USART2, "+OK DP\n" );
      break;

    case RD:
      {
        char b [ 100 ] = "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0";
        uint32_t i;
        uint32_t addr = EXTRAM_BASE;
        uint8_t v;

        bus_grab_and_wait();
        bus_perform_read ( addr ); // discard .. just getting /CS set

        //addr += 1000;

        lame_itoa ( addr, b );

        USART_puts ( USART2, "addr " );
        USART_puts ( USART2, b );
        USART_puts ( USART2, "\n" );

        for ( i = 0; i < 10; i++ ) {
          v = bus_perform_read ( addr );

          b [ 0 ] = '\0';
          b [ 1 ] = '\0';
          b [ 2 ] = '\0';
          b [ 3 ] = '\0';
          b [ 4 ] = '\0';
          lame_itoa ( v, b );

          USART_puts ( USART2, b );
          USART_puts ( USART2, "\n" );

          addr ++;
        }

        bus_release();

        USART_puts ( USART2, "+OK RD\n" );
      }
      break;

    case ID:
      fb_render_rect_filled ( fb_active, 0, 0, FBWIDTH - 1, FBHEIGHT - 1, 0x00 ); // busy loop needed??!
      {
        char b [ 100 ] = "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0";
        int retval;
        char retvalb [ 30 ] = "rv: \0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0";

        I2C_BusInit();

        retval = I2C_ReadTransfer ( 0xe8, (uint8_t*)b, 5, 0, 0 );

        lame_itoa ( retval, retvalb + 4 );
        USART_puts_optional ( USART2, retvalb );

        USART_puts_optional ( USART2, b );

        USART_puts_optional ( USART2, "\n" );

      }
      break;

    case BD:
      //fb_render_rect_filled ( fb_active, 0, 0, FBWIDTH - 1, FBHEIGHT - 1, 0x00 ); // busy loop needed??!
      //USART_puts_optional ( USART2, "+OK BD\n" );
      {
        unsigned char sawblank = 0;
        unsigned char running = 60;
        unsigned char blankcounter = 0;

        while ( running ) {
          extern volatile unsigned char vblank;
          if ( ! vblank ) {
            sawblank = 1;
            continue;
          }

          if ( sawblank > 1 ) {
            continue;
          }
          sawblank = 2;
          running--;

#if 1
          if ( blankcounter < 2 ) {
            blankcounter++;
            continue;
          }
          blankcounter = 0;
#endif

          bus_grab_and_wait();

          //uint32_t addr = 0x1C0000;
          uint32_t addr = EXTRAM_BASE;
          uint8_t v;
          uint32_t i;

          bus_perform_read ( addr ); // discard .. just getting /CS set

          for ( i = 0; i < 45000; i++ ) {

            v = bus_perform_read ( addr );

            if ( v > 0 ) {
              fb_active [ i ] = v;
            }

            addr++;
          }

          bus_release();
        } // while running

      } // BD scope
      USART_puts ( USART2, "+OK BD\n" );
      break;

    case SR:
      {
        //unsigned char srid = c [ 2 ];
        unsigned int srw = c [ 3 ];
        unsigned int srh = c [ 4 ];
        int16_t srlen = srw * srh;
        unsigned int x, y;

        lamecopy ( fb_active, c + 5, 25 );
        srlen -= 25;
        x = 25;
        y = 0;

        USART_puts_optional ( USART2, "+OK SR 25\n" );

        while ( srlen > 0 ) {
          c = queuepull();
          USART_puts_optional ( USART2, "+REM 2\n" );

          if ( c ) {
            USART_puts_optional ( USART2, "+REM 3\n" );

            if ( x + 30 > srw ) {
              USART_puts_optional ( USART2, "+REM 4\n" );
              lamecopy ( fb_active + ( y * 256 ) + x, c, srw - x );
              y += 1;
              lamecopy ( fb_active + ( y * 256 ) + 0, c, 30 - ( srw - x ) );
              x = 30 - x;
            } else {
              USART_puts_optional ( USART2, "+REM 5\n" );
              lamecopy ( fb_active + ( y * 256 ) + x, c, 30 );
              x += 30;
            }

            USART_puts_optional ( USART2, "+OK SR 30\n" );
            srlen -= 30;
          }

        } // while

      }
      break;

    default:
      logit ( "+ERR\r\n" );

    } // switch on command

  } // while commands in queue

  return;
}
