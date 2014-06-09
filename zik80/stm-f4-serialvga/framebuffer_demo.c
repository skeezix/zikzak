
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/timer.h>

#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/exti.h>

#include <libopencm3/cm3/cortex.h>

#include "framebuffer.h"
#include "framebuffer_demo.h"

void fb_test_pattern ( uint8_t *fb ) {
  unsigned int i;

#if 1 // fill framebuffer with offset squares
  unsigned int x, y;
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
#endif

#if 0 // fill framebuffer with vertical stripes of all colours (1px per colour)
  //unsigned char i;
  unsigned int x, y;
  unsigned char v;

  for ( y = 0; y < FBHEIGHT; y++ ) {

    i = 0;
    for ( x = 0; x < FBWIDTH; x++ ) {

      //*( framebuffer + ( y * FBWIDTH ) + x ) = i / 6;
      *( fb + ( y * FBWIDTH ) + x ) = i;
      //*( framebuffer + ( y * FBWIDTH ) + x ) = (unsigned char)( GPIO0 | GPIO1 );

      i++;
    } // x

  } // y
#endif

#if 0 // vertical strip every X pixels
  //unsigned char i;
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
#endif

#if 0 // vertical strip 1 pixel wide
  //unsigned char i;
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
#endif

}
