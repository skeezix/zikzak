
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/timer.h>

#include "dma_memcpy.h"
#include "framebuffer.h"
#include "system.h"

#include <string.h> // memcpy

uint8_t fb_1 [ FBWIDTH * FBHEIGHT ] /*__attribute((aligned (1024)))*/;
//uint8_t *fb_1 = (uint8_t *)(0x10000000);
uint8_t fb_2 [ FBWIDTH * FBHEIGHT ] /*__attribute((aligned (1024)))*/;
uint8_t *fb_active;
uint8_t *fb_inactive;

void fb_setup ( void ) {
  fb_active = fb_1;
  fb_inactive = fb_2;
  fb_render_rect_filled ( fb_1, 0, 0, FBWIDTH - 1, FBHEIGHT - 1, 0x00 );
  fb_render_rect_filled ( fb_2, 0, 0, FBWIDTH - 1, FBHEIGHT - 1, 0x00 );
}

void fb_clone ( uint8_t *fbsrc, uint8_t *fbdst ) {
  memcpy ( fbdst, fbsrc, FBWIDTH * FBHEIGHT ); // screws up the DMA!?
  return;
}

inline void fb_render_rect_filled ( uint8_t *fb, uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t rgb ) {
  register uint16_t tx, ty;
  register uint16_t ty2;
  register uint8_t *t;

  ty2 = y + h;

  for ( ty = y; ty < ty2; ty++ ) {
    t = fb + ( ty * FBWIDTH ) + x; 
    for ( tx = 0; tx < w; tx++ ) {
      *t++ = rgb;
      //__asm__("nop");
    } // x
  } // y

}

/* bresenham based line draw */
// ripped from old zotlib and adapted to 8bpp
void zl_render_line ( uint8_t *bits, uint8_t c, uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1 ) {
  int16_t dy = y1 - y0;
  int16_t dx = x1 - x0;
  int16_t stepx, stepy;
  int16_t fraction;

  if (dy < 0) {
    dy = -dy;
    stepy = - ( FBWIDTH );
  } else {
    stepy = ( FBWIDTH );
  }

  if (dx < 0) {
    dx = -dx;
    stepx = -1;
  } else {
    stepx = 1;
  }

  dy <<= 1;
  dx <<= 1;

  y0 *= ( FBWIDTH );
  y1 *= ( FBWIDTH );

  bits [ x0 + y0 ] = c;

  if (dx > dy) {
    fraction = dy - (dx >> 1);
    while (x0 != x1) {
      if (fraction >= 0) {
	y0 += stepy;
	fraction -= dx;
      }
      x0 += stepx;
      fraction += dy;
      bits [ x0 + y0 ] = c;
    }
  } else {
    fraction = dx - (dy >> 1);
    while (y0 != y1) {
      if (fraction >= 0) {
	x0 += stepx;
	fraction -= dy;
      }
      y0 += stepy;
      fraction += dx;
      bits [ x0 + y0 ] = c;
    }
  }

  return;
}

void zl_render_blit32 ( uint8_t *from, uint8_t *to,
                        uint16_t x1, uint16_t y1, uint16_t w, uint16_t h,
                        uint16_t to_x, uint16_t to_y )
{
  uint16_t x2 = x1 + w;
  uint16_t y2 = y1 + h;
  uint16_t x, y;
  uint8_t *vfromline = from;
  uint32_t *vfromiter;
  uint8_t *vtoline = to;
  uint32_t *vtoiter;

  vfromline += ( y1 * FBWIDTH );
  vfromline += x1;

  vtoline += ( to_y * FBWIDTH );
  vtoline += to_x;

  x1 >>= 2;
  x2 >>= 2;

  for ( y = y1; y < y2; y++ ) {
    vtoiter = (uint32_t*) vtoline;
    vfromiter = (uint32_t*) vfromline;

    for ( x = x1; x < x2; x++ ) {
      *vtoiter++ = *vfromiter++;
    }

    vtoline += FBWIDTH;
    vfromline += FBWIDTH;
  }

  return;
}

void zl_render_blit8 ( uint8_t *from, uint8_t *to,
                       uint16_t x1, uint16_t y1, uint16_t w, uint16_t h,
                       uint16_t to_x, uint16_t to_y )
{
  uint16_t x2 = x1 + w;
  uint16_t y2 = y1 + h;
  uint16_t x, y;
  uint8_t *vfromline = from;
  uint8_t *vfromiter;
  uint8_t *vtoline = to;
  uint8_t *vtoiter;

  vfromline += ( y1 * FBWIDTH );
  vfromline += x1;

  vtoline += ( to_y * FBWIDTH );
  vtoline += to_x;

  for ( y = y1; y < y2; y++ ) {
    vtoiter = vtoline;
    vfromiter = vfromline;

    for ( x = x1; x < x2; x++ ) {
      *vtoiter++ = *vfromiter++;
    }

    vtoline += FBWIDTH;
    vfromline += FBWIDTH;
  }

  return;
}

void fb_swap ( void ) {

  if ( fb_active == fb_1 ) {
    fb_active = fb_2;
    fb_inactive = fb_1;
  } else {
    fb_active = fb_1;
    fb_inactive = fb_2;
  }

  return;
}
