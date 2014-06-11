
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/timer.h>

#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/exti.h>

#include <libopencm3/cm3/cortex.h>

#include "framebuffer.h"
#include "framebuffer_demo.h"

void fb_test_pattern ( uint8_t *fb, fbt_e pattern ) {

  if ( pattern == fbt_offset_squares ) {
    unsigned int x, y, i;
    unsigned char v;
    for ( y = 0; y < FBHEIGHT; y++ ) {

      //i = 0;
      i = ( y / 10 ) % 5;

      for ( x = 0; x < FBWIDTH; x++ ) {

        if ( x % 10 == 0 ) {
          i++;
        }

        if ( i == 0 ) {
          v = (unsigned char) GPIO0;
        } else if ( i == 1 ) {
          v = (unsigned char) GPIO1;
        } else if ( i == 2 ) {
          v = (unsigned char) GPIO2;
        } else if ( i == 3 ) {
          v = (unsigned char) GPIO3;
        } else if ( i == 4 ) {
          v = (unsigned char) GPIO4;
        } else if ( i == 5 ) {
          v = (unsigned char) GPIO5;
        } else {
          i = 0;
          v = (unsigned char) GPIO0;
        }

        if ( y % 10 == 0 ) {
          v = 0; // black
        }

        *( fb + ( y * FBWIDTH ) + x ) = v;

        //*( framebuffer + ( y * FBWIDTH ) + x ) = (unsigned char) 0;
        //*( framebuffer + ( y * FBWIDTH ) + x ) = (unsigned char)( GPIO3 );
        //*( framebuffer + ( y * FBWIDTH ) + x ) = (unsigned char)( GPIO5 );
        //*( framebuffer + ( y * FBWIDTH ) + x ) = (unsigned char)( GPIO1 | GPIO3 );
        //*( framebuffer + ( y * FBWIDTH ) + x ) = (unsigned char)( GPIO1 | GPIO0 );
        //*( framebuffer + ( y * FBWIDTH ) + x ) = (unsigned char)( GPIO0 | GPIO1 );
        //*( framebuffer + ( y * FBWIDTH ) + x ) = (unsigned char)( GPIO0 | GPIO1 | GPIO2 | GPIO3 );
        //*( framebuffer + ( y * FBWIDTH ) + x ) = (unsigned char)( GPIO2 | GPIO3 );
        //*( framebuffer + ( y * FBWIDTH ) + x ) = (unsigned char)( GPIO4 | GPIO5 );

      } // x

    } // y

  } else if ( pattern == fbt_vlines ) {
    unsigned char i;
    unsigned int x, y;

    for ( y = 0; y < FBHEIGHT; y++ ) {

      i = 0;
      for ( x = 0; x < FBWIDTH; x++ ) {

        //*( framebuffer + ( y * FBWIDTH ) + x ) = i / 6;
        *( fb + ( y * FBWIDTH ) + x ) = i & 0x3F; //&0x03;
        //*( framebuffer + ( y * FBWIDTH ) + x ) = (unsigned char)( GPIO0 | GPIO1 );

        i++;
      } // x

    } // y

  } else if ( pattern == fbt_onoff1 ) {
    //unsigned char i;
    unsigned int x, y;

    for ( y = 0; y < FBHEIGHT; y++ ) {
      for ( x = 0; x < FBWIDTH; x++ ) {
        *( fb + ( y * FBWIDTH ) + x ) = x & 0x01;
      } // x
    } // y

  } else if ( pattern == fbt_vwlines ) {
    unsigned char i;
    unsigned int x, y;

    for ( y = 0; y < FBHEIGHT; y++ ) {

      i = 0;
      for ( x = 0; x < FBWIDTH; x++ ) {

        if ( i >= 9 ) {
          *( fb + ( y * FBWIDTH ) + x ) = GPIO1;
        }

        if ( i == 10 ) {
          i = 0;
        }

        i++;
      } // x
    } // y

  } else if ( pattern == fbt_v1lines ) {
    unsigned char i;
    unsigned int x, y;

    for ( y = 0; y < FBHEIGHT; y++ ) {
      i = 0;

      for ( x = 0; x < FBWIDTH; x++ ) {

        if ( i == 9 ) {
          *( fb + ( y * FBWIDTH ) + x ) = GPIO1;
          i = 0;
        }

        i++;
      } // x

    } // y

  } // pattern?

  return;
}

#if 0
void fb_plasma_diamond ( uint8_t *fb ) {
  int x, y;

  for ( x = 0; x < FBWIDTH; x++) {
    for ( y = 0; y < FBHEIGHT; y++)  {

      uint8_t cc = abs ( 128.0 + (128.0 * sin(x / 8.0))
                         + 128.0 + (128.0 * sin(y / 8.0))
                       ) / 2;

      fb [ ( y * FBWIDTH ) + x ] = RGB(c,c,c);

    } // y
  } // x

  return;
}
#endif
