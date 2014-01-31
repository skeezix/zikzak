
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/timer.h>

#include "framebuffer.h"

uint8_t framebuffer [ FBWIDTH * FBHEIGHT ] /*__attribute((aligned (1024)))*/;
uint8_t offscreen [ FBWIDTH * FBHEIGHT ] /*__attribute((aligned (1024)))*/;

void fb_test_pattern ( void ) {
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

      *( framebuffer + ( y * FBWIDTH ) + x ) = v;

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
      *( framebuffer + ( y * FBWIDTH ) + x ) = i;
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
        *( framebuffer + ( y * FBWIDTH ) + x ) = GPIO1;
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
        *( framebuffer + ( y * FBWIDTH ) + x ) = GPIO1;
        i = 0;
      }

      i++;
    } // x
  } // y
#endif

}

void fb_clone ( uint8_t *fbsrc, uint8_t *fbdst ) {

  // memcpy ( fbdst, fbsrc, FBWIDTH * FBHEIGHT ); // screws up the DMA!?

  uint16_t i;
  uint8_t *s, *d;

#if 1
  i = FBWIDTH * FBHEIGHT;

  while ( i-- ) {
    *fbdst++ = *fbsrc++;
  }
#else
  i = FBWIDTH * FBHEIGHT;
  i /= 8;

  while ( i-- ) {
    *fbdst++ = *fbsrc++;
    *fbdst++ = *fbsrc++;
    *fbdst++ = *fbsrc++;
    *fbdst++ = *fbsrc++;
    *fbdst++ = *fbsrc++;
    *fbdst++ = *fbsrc++;
    *fbdst++ = *fbsrc++;
    *fbdst++ = *fbsrc++;
  }
#endif

}

inline void fb_render_rect_filled ( uint8_t *fb, uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t rgb ) {
  uint16_t tx, ty;
  uint16_t tx2, ty2;
  uint8_t *t;

  tx2 = x + w;
  ty2 = y + h;

  for ( ty = y; ty < ty2; ty++ ) {
    t = fb + ( ty * FBWIDTH ) + x; 
    for ( tx = 0; tx < w; tx++ ) {
      *t++ = rgb;
      //__asm__("nop");
    } // x
  } // y

}

static uint16_t dx = 10, dy = 65;
static uint8_t drgb = 0;
void fb_lame_demo_animate ( uint8_t *fb ) {

  dx++;
  dy++;

  if ( dy % 10 == 0 ) {
    drgb++;
  }

  fb_render_rect_filled ( fb, dx, dy, 10, 10, drgb );

  if ( dx > 300 ) {
    dx -= 292;
  }

  if ( dy > 160 ) {
    dy = 65;
    dx += 20;
  }

  if ( drgb == 0b00111111 ) {
    drgb = 0;
  }

}
