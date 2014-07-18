#include <eZ80.h>

#include "skeelib.h"
#include "framebuffer.h"

#include <string.h> // memcpy

void fb_render_rect_filled ( UINT8 *fb, UINT8 x, UINT8 y, UINT8 w, UINT8 h, UINT8 rgb ) {
  register UINT8 tx, ty;
  register UINT8 ty2;
  register UINT8 *t;

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
void zl_render_line ( UINT8 *bits, UINT8 c, UINT16 x0, UINT16 y0, UINT16 x1, UINT16 y1 ) {
  INT16 dy = y1 - y0;
  INT16 dx = x1 - x0;
  INT16 stepx, stepy;
  INT16 fraction;

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

void zl_render_blit32 ( UINT8 *from, UINT8 *to,
                        UINT8 x1, UINT8 y1, UINT8 w, UINT8 h,
                        UINT8 to_x, UINT8 to_y )
{
  UINT8 x2 = x1 + w;
  UINT8 y2 = y1 + h;
  UINT8 x, y;
  UINT8 *vfromline = from;
  UINT32 *vfromiter;
  UINT8 *vtoline = to;
  UINT32 *vtoiter;

  vfromline += ( y1 * FBWIDTH );
  vfromline += x1;

  vtoline += ( to_y * FBWIDTH );
  vtoline += to_x;

  x1 >>= 2;
  x2 >>= 2;

  for ( y = y1; y < y2; y++ ) {
    vtoiter = (UINT32*) vtoline;
    vfromiter = (UINT32*) vfromline;

    for ( x = x1; x < x2; x++ ) {
      *vtoiter++ = *vfromiter++;
    }

    vtoline += FBWIDTH;
    vfromline += FBWIDTH;
  }

  return;
}

void zl_render_blit8 ( UINT8 *from, UINT8 *to,
                       UINT8 x1, UINT8 y1, UINT8 w, UINT8 h,
                       UINT8 to_x, UINT8 to_y )
{
  UINT8 x2 = x1 + w;
  UINT8 y2 = y1 + h;
  UINT8 x, y;
  UINT8 *vfromline = from;
  UINT8 *vfromiter;
  UINT8 *vtoline = to;
  UINT8 *vtoiter;

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
