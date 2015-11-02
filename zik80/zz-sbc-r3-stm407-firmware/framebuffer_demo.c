
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

    // render title
    render_font_8x8 ( fb, 10, 10, "Offset Squares", 0xFF, 0 );

  } else if ( pattern == fbt_full_palette_grid ) {
    // ordered implied by palette value

    unsigned int p;       // palette iterator

    unsigned int x, y;    // dest cell top left (x,y)
    unsigned int xi, yi;  // dest cell incrementing through the box

    unsigned int _w = 14; // width
    unsigned int _h = 10; // height

    // one square for each palette entry (RRGGBBII -> 8bits -> 256 colours/intensities)
    for ( p = 0; p != 0xFF; p++ ) {

      // given a colour, figure out where to place it
      x = ( p % 16 ) * _w;
      y = ( p / 16 ) * _h;

      for ( xi = 0; xi < _w; xi++ ) {
        for ( yi = 0; yi < _h; yi++ ) {
          *( fb + ( (y+yi) * FBWIDTH ) + (x+xi) ) = p;
        } // yi
      } // xi

    } // p (palette incrementer)

    // render title
    render_font_8x8 ( fb, 10, 10, "Palette Incremental Grid", 0xFF, 0 );

  } else if ( pattern == fbt_full_palette_grid_ordered ) {
    // ordered by rules, not just implied by palette value

    unsigned char r, g, b, i;
    unsigned char rh; // redhack -> red iter mapped to red; I put resistors wrong on r3 test board, so brightness order is funky
    unsigned char bh; // redhack -> red iter mapped to red; I put resistors wrong on r3 test board, so brightness order is funky

    unsigned char p; // palette entry to use
    unsigned int cell; // grid cell to use

    unsigned int x, y;    // dest cell top left (x,y)
    unsigned int xi, yi;  // dest cell incrementing through the box

    unsigned int _w = 16; // width
    unsigned int _h = 12; // height

    // one square for each palette entry (RRGGBBII -> 8bits -> 256 colours/intensities)
    cell = 0;
    for ( i = 0; i < 4; i++ ) {
      for ( bh = 0; bh < 4; bh++ ) {
        for ( g = 0; g < 4; g++ ) {
          for ( rh = 0; rh < 4; rh++ ) {

            // map red iterator to colour
            switch ( rh ) {
            case 0: r = 0; break;
            case 1: r = 2; break;
            case 2: r = 1; break;
            case 3: r = 3; break;
            }

            // map blue iterator to colour
            switch ( bh ) {
            case 0: b = 0; break;
            case 1: b = 2; break;
            case 2: b = 1; break;
            case 3: b = 3; break;
            }

            // assemble a palette entry
            p = 0;
            p |= ( r );
            p |= ( g << 2 );
            p |= ( b << 4);
            p |= ( i << 6);

            // given a colour, figure out where to place it
            x = ( cell % 16 ) * _w;
            y = ( cell / 16 ) * _h;

            for ( xi = 0; xi < _w - 1; xi++ ) {
              for ( yi = 0; yi < _h - 1; yi++ ) {
                *( fb + ( (y+yi) * FBWIDTH ) + (x+xi) ) = p;
              } // yi
            } // xi

            cell++;

          } // r
        } // g
      } // b
    } // i

    // render title
    render_font_8x8 ( fb, 10, 10, "Full Palette", 0xFF, 0 );

  } else if ( pattern == fbt_reds ) {

    unsigned char r, rh;
    unsigned char i;

    // rh -> redhack
    // - since I put resistors swapped, gotta swap some colours or ordering is funny :)

    unsigned char cell = 0, p;

    unsigned int x, y;    // dest cell top left (x,y)
    unsigned int xi, yi;  // dest cell incrementing through the box

    unsigned int _w = 64; // width
    unsigned int _h = 48; // height

    for ( rh = 0; rh < 4; rh++ ) {
      for ( i = 0; i < 4; i++ ) {

        // map red iterator to colour
        switch ( rh ) {
        case 0: r = 0; break;
        case 1: r = 2; break;
        case 2: r = 1; break;
        case 3: r = 3; break;
        }

        // assemble a palette entry
        p = 0;
        p |= ( r );
        p |= ( i << 6);

        // given a colour, figure out where to place it
        x = ( cell % 4 ) * _w;
        y = ( cell / 4 ) * _h;

        for ( xi = 0; xi < _w - 1; xi++ ) {
          for ( yi = 0; yi < _h - 1; yi++ ) {
            *( fb + ( (y+yi) * FBWIDTH ) + (x+xi) ) = p;
          } // yi
        } // xi

        // next
        cell++;

      } // i
    } // r

    // render title
    render_font_8x8 ( fb, 10, 10, "Reds", 0xFF, 0 );

  } else if ( pattern == fbt_greens ) {

    unsigned char r, rh;
    unsigned char i;

    // rh -> redhack
    // - since I put resistors swapped, gotta swap some colours or ordering is funny :)

    unsigned char cell = 0, p;

    unsigned int x, y;    // dest cell top left (x,y)
    unsigned int xi, yi;  // dest cell incrementing through the box

    unsigned int _w = 64; // width
    unsigned int _h = 48; // height

    for ( rh = 0; rh < 4; rh++ ) {
      for ( i = 0; i < 4; i++ ) {

        // map red iterator to colour
        switch ( rh ) {
        case 0: r = 0; break;
        case 1: r = 2; break;
        case 2: r = 1; break;
        case 3: r = 3; break;
        }

        // assemble a palette entry
        p = 0;
        p |= ( r << 2 );
        p |= ( i << 6 );

        // given a colour, figure out where to place it
        x = ( cell % 4 ) * _w;
        y = ( cell / 4 ) * _h;

        for ( xi = 0; xi < _w - 1; xi++ ) {
          for ( yi = 0; yi < _h - 1; yi++ ) {
            *( fb + ( (y+yi) * FBWIDTH ) + (x+xi) ) = p;
          } // yi
        } // xi

        // next
        cell++;

      } // i
    } // r

    // render title
    render_font_8x8 ( fb, 10, 10, "Greens", 0xFF, 0 );

  } else if ( pattern == fbt_blues ) {

    unsigned char r, rh;
    unsigned char i;

    // rh -> redhack
    // - since I put resistors swapped, gotta swap some colours or ordering is funny :)

    unsigned char cell = 0, p;

    unsigned int x, y;    // dest cell top left (x,y)
    unsigned int xi, yi;  // dest cell incrementing through the box

    unsigned int _w = 64; // width
    unsigned int _h = 48; // height

    for ( rh = 0; rh < 4; rh++ ) {
      for ( i = 0; i < 4; i++ ) {

        // map red iterator to colour
        switch ( rh ) {
        case 0: r = 0; break;
        case 1: r = 2; break;
        case 2: r = 1; break;
        case 3: r = 3; break;
        }

        // assemble a palette entry
        p = 0;
        p |= ( r << 4 );
        p |= ( i << 6);

        // given a colour, figure out where to place it
        x = ( cell % 4 ) * _w;
        y = ( cell / 4 ) * _h;

        for ( xi = 0; xi < _w - 1; xi++ ) {
          for ( yi = 0; yi < _h - 1; yi++ ) {
            *( fb + ( (y+yi) * FBWIDTH ) + (x+xi) ) = p;
          } // yi
        } // xi

        // next
        cell++;

      } // i
    } // r

    // render title
    render_font_8x8 ( fb, 10, 10, "Blues", 0xFF, 0 );

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

    // render title
    render_font_8x8 ( fb, 10, 10, "TopBottom", 0xFF, 0 );

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

    // render title
    render_font_8x8 ( fb, 10, 10, "VLines", 0xFF, 0 );

  } else if ( pattern == fbt_onoff1 ) {
    //unsigned char i;
    unsigned int x, y;

    for ( y = 0; y < FBHEIGHT; y++ ) {
      for ( x = 0; x < FBWIDTH; x++ ) {
        *( fb + ( y * FBWIDTH ) + x ) = x & 0x01;
      } // x
    } // y

    // render title
    render_font_8x8 ( fb, 10, 10, "OnOff1", 0xFF, 0 );

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

    // render title
    render_font_8x8 ( fb, 10, 10, "VWLines", 0xFF, 0 );

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

    // render title
    render_font_8x8 ( fb, 10, 10, "V1Lines", 0xFF, 0 );

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

    char b [ 3 ] = "\0\0\0";

#if 1
    unsigned char *charset = SRAM2_MEMORY_BASE;
    //unsigned char charset [ 128 * 8 * 8 ];
    unsigned char *sprite = SRAM2_MEMORY_BASE + (5*8*FBWIDTH);

    // expand font into sram2 buffer
    {
      unsigned int c;

      for ( c = 0; c < 128; c++ ) {
        b [ 0 ] = c;
        render_font_8x8 ( charset, (c % 32) * 8, 0 + ((c / 32) * 8), b, 0xFF, 1 );
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
#endif

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
      render_font_8x8 ( fb_inactive, 50, 80, "SCORE", 0xFF, 1 );
#endif

      // render tilemap
#if 1
      for ( tiley = 0; tiley < 24; tiley++ ) {
        for ( tilex = 0; tilex < 32; tilex++ ) {
          b [ 0 ] = sram3 [ ( tiley * 32 ) + tilex ];
          //b [ 0 ] = 'A';
          render_font_8x8 ( fb_inactive, tilex * 8, tiley * 8, b, 35, 1 );
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

      // render title
      render_font_8x8 ( fb, 10, 10, "SpriteRAM", 0xFF, 0 );

    } // while forever

  } // pattern?

  return;
}

void _demo_blit_16x16 ( unsigned char *to, unsigned int x, unsigned int y ) {
  unsigned char row;
  unsigned char *sprite = SRAM2_MEMORY_BASE + (5*8*FBWIDTH);

  unsigned char *p = to + ( y * FBWIDTH ) + x;

  unsigned char n, nn; // transparency
  unsigned char *np, *nsrc; // transparency

  for ( row = 0; row < 16; row++ ) {

#if 0 // no transparency, faster
    lame_memcpy ( p, sprite + ( row * 16 ), 16 );
#else // transparency, slower
    n = 16; np = p; nsrc = sprite + ( row * 16 );
    while ( n ) {
      nn = *nsrc++;
      if ( nn != 1 ) {
        *np = nn;
      } else {
        // nada
      }
      np++;
      n--;
    }
#endif

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
