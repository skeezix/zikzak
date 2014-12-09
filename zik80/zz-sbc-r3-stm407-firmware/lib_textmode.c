
#include <stm32f4xx.h>
#include <string.h>

#include "config.h"
#include "lib_serial.h"

#include "system.h"
#include "logger.h"
#include "framebuffer.h"
#include "framebuffer_demo.h"
#include "lib_bus_ram.h"
#include "skeelib.h"
#include "lib_i2c.h"
#include "lib_textmode.h"
#include "font_vincent.h"
#include "HAL.h"

static unsigned char *charset = SRAM2_MEMORY_BASE;
static unsigned char *sprite = SRAM2_MEMORY_BASE + (5*8*FBWIDTH);

static uint8_t ram_textmode [ FBWIDTH * FBHEIGHT / 8 / 8 ];
static uint8_t ram_textattr [ FBWIDTH * FBHEIGHT / 8 / 8 ];
static uint8_t ram_spritelist [ TM_SPRITE_MAX * TM_SPRITE_CELL_STRIDE ];
static uint8_t ram_spritecount = 0;

void tm_setup ( void ) {
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

  } // expand sprite

  return;
}

void tm_fetch_ram ( void ) {
  bus_grab_and_wait();
  bus_read_block ( ram_textmode, TM_VRAM_FB, TM_PAGE_STRIDE );
  bus_read_block ( ram_textattr, TM_VRAM_ATTR, TM_PAGE_STRIDE );
  bus_read_block ( &ram_spritecount, TM_SPRITE_ACTIVE, 1 );
  bus_read_block ( ram_spritelist, TM_SPRITE_LIST_BASE, ( ram_spritecount * TM_SPRITE_CELL_STRIDE) );
  bus_release();
  return;
}

void tm_render_to_framebuffer ( uint8_t *fb ) {
  char b [ 3 ] = "\0\0\0";
  unsigned char tilex, tiley;

  // render tilemap
#if 1
  for ( tiley = 0; tiley < TM_TEXTCOL_STRIDE; tiley++ ) {
    for ( tilex = 0; tilex < TM_TEXTLINE_STRIDE; tilex++ ) {
      b [ 0 ] = ram_textmode [ ( tiley * TM_TEXTLINE_STRIDE ) + tilex ];
      //b [ 0 ] = 'A';
      render_font_8x8 ( fb, tilex * 8, tiley * 8, b, ram_textattr [ ( tiley * TM_TEXTLINE_STRIDE ) + tilex ] / 2 );
    } // tilex
  } // tiley
#endif

      // render sprites
#if 1
  {
    unsigned char *spam = ram_spritelist;

    if ( ram_spritecount > TM_SPRITE_MAX ) {
      ram_spritecount = TM_SPRITE_MAX;
    }

    for ( tiley = 0; tiley < ram_spritecount; tiley++ ) {

      if ( spam [ 2 ] & TM_SPRITE_SHOW ) {
        _demo_blit_16x16 ( fb, spam [ 0 ], spam [ 1 ] );
      }

      spam += TM_SPRITE_CELL_STRIDE;
    } // for

  }
#endif

  return;
}
