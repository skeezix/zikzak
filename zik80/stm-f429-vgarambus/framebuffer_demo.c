
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/timer.h>

#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/exti.h>

#include <libopencm3/cm3/cortex.h>

#include "framebuffer.h"
#include "framebuffer_demo.h"
#include "system.h"
#include "skeelib.h"

#include "font_vincent.h"

void _demo_blit_16x16 ( unsigned char *to, unsigned int x, unsigned int y );

void fb_test_pattern ( uint8_t *fb, fbt_e pattern ) {

  if ( pattern == fbt_offset_squares ) {
    unsigned int x, y, i;
    unsigned char v;
    for ( y = 0; y < FBHEIGHT; y++ ) {

      //i = 0;
      i = ( y / 10 ) % 5;

      for ( x = 0; x < (FBWIDTH/12); x++ ) {

        i++;

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

        { // fake memset:  memset ( fb + ( y * FBWIDTH ) + (x*16) ), v, 16 );
          unsigned char z;
          for ( z = 0; z < 12; z++ ) {
            * ( fb + ( y * FBWIDTH ) + (x*12) + z ) = v;
          }
        }

      } // x

    } // y

  } else if ( pattern == fbt_topbottom ) {
    unsigned int x, y;

    for ( y = 0; y < FBHEIGHT; y++ ) {

      for ( x = 0; x < FBWIDTH; x++ ) {

        if ( y < ( FBHEIGHT/2) ) {
          *( fb + ( y * FBWIDTH ) + x ) = GPIO0;
        } else {
          *( fb + ( y * FBWIDTH ) + x ) = GPIO1;
        }

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

  } else if ( pattern == fbt_spriteram ) {
    // given current code - fb_1 and fb_2 are in main sram (112KB chunk), not leaving much
    // sram2 16KB -> unused
    // sram3 f429 64KB -> unused
    // ccm 64KB -> unused
    //   -> correction, moved fb_1 to CCM

    // demo goals:
    // - expand vincent font into 8x8 character data              -> sram2
    // - expand vincent font char into 16x16 for use as sprite    -> sram2
    // - main loop
    //   - read 1K from external RAM                              -> sram3
    //   - render ram read as tilemap                             -> fb_inactive
    //   - render some number of sprites                          -> fb_inactive (spritelist from sram3)
    //   - move sprites around smoothly                           ++
    //   - swap buffers

    // consider:
    // - should fb_1 and/or fb_2 be in CCM RAM?

    unsigned char *charset = SRAM2_MEMORY_BASE;
    //unsigned char charset [ 128 * 8 * 8 ];
    unsigned char *sprite = SRAM2_MEMORY_BASE + (5*8*FBWIDTH);
    char b [ 3 ] = "\0\0\0";

    // expand font into sram2 buffer
    {
      unsigned int c;

      for ( c = 0; c < 128; c++ ) {
        b [ 0 ] = c;
        render_font_8x8 ( charset, (c % 32) * 8, 0 + ((c / 32) * 8), b, 0xFF );
      }

    } // expand font

    // expand a character from 8x8 to 16x16
    {
      // use font character 15 ("*")
      unsigned char x, y;

      for ( y = 0; y < 8; y++ ) {
        for ( x = 0; x < 8; x++ ) {
          unsigned char *src = charset + 120 + x + ( y * FBWIDTH );
          unsigned char *target = sprite + ( x * 2 ) + ( y * 16 * 2 );
          unsigned char colour = *src;
          if ( colour > 128 ) {
            colour = x * y;
          } else {
            colour = 1;
          }
          *target++ = colour;
          *target++ = colour;
          target -= 2;
          target += 16;
          *target++ = colour;
          *target++ = colour;
        } // x
      } // y

    }

    // demo loop
    unsigned char tilex, tiley;
    unsigned char *sram3 = SRAM3_MEMORY_BASE;
    extern volatile unsigned char vblank;

    while ( 1 ) {

#if 0
      while ( ! vblank ) {
        __asm__("nop");
      }
#endif

      // render score
#if 1
      render_font_8x8 ( fb_inactive, 50, 80, "SCORE", 0xFF );
#endif

      // render tilemap
#if 1
      for ( tiley = 0; tiley < 24; tiley++ ) {
        for ( tilex = 0; tilex < 32; tilex++ ) {
          b [ 0 ] = sram3 [ ( tiley * 32 ) + tilex ];
          //b [ 0 ] = 'A';
          render_font_8x8 ( fb_inactive, tilex * 8, tiley * 8, b, 35 );
        } // tilex
      } // tiley
#endif

      // render sprites
#if 1
      {
        unsigned char *spam = SRAM3_MEMORY_BASE;
        unsigned char *curr;
        // x, y, dir, speed
        for ( tiley = 0; tiley < 100; tiley++ ) {
          curr = spam + (4*tiley);

          _demo_blit_16x16 ( fb_inactive, curr [ 0 ], curr [ 1 ] );

          if ( curr [ 2 ] < 64 ) {
            // go left
            if ( curr [ 0 ] > 10 ) {
              curr [ 0 ] -= curr [ 3 ] / 64;
            } else {
              curr [ 2 ] = 100;
            }
          } else if ( curr [ 2 ] > 64 && curr [ 2 ] < 128 ) {
            // go right
            if ( curr [ 0 ] < FBWIDTH - 40 ) {
              curr [ 0 ] += curr [ 3 ] / 64;
            } else {
              curr [ 2 ] = 50;
            }
          } else if ( curr [ 2 ] > 128 && curr [ 2 ] < 192 ) {
            // up
            if ( curr [ 1 ] > 40 ) {
              curr [ 1 ] -= curr [ 3 ] / 64;
            } else {
              curr [ 2 ] = 250;
            }
          } else {
            // down
            if ( curr [ 1 ] < FBHEIGHT - 40 ) {
              curr [ 1 ] += curr [ 3 ] / 64;
            } else {
              curr [ 2 ] = 150;
            }
          }

        }

      }
#endif

      // swap
      //fb_swap();
      //while ( 1 );

      while ( ! vblank ) {
        __asm__("nop");
      }
      fb_swap();
      while ( vblank ) {
        __asm__("nop");
      }

      // move sprites

    } // while forever

  } // pattern?

  return;
}

void _demo_blit_16x16 ( unsigned char *to, unsigned int x, unsigned int y ) {
  unsigned char row;
  unsigned char *sprite = SRAM2_MEMORY_BASE + (5*8*FBWIDTH);

  unsigned char *p = to + ( y * FBWIDTH ) + x;

  for ( row = 0; row < 16; row++ ) {

    lame_memcpy ( p, sprite + ( row * 16 ), 16 );

    p += FBWIDTH;
  }

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
