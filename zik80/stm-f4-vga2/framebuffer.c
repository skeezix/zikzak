
#include "framebuffer.h"

unsigned char _g_framebuffer [ FBWIDTH * FBHEIGHT ];

void framebuffer_setup ( void ) {

#if 1 // vertical strip every 10 pixels
  unsigned char i;
  unsigned int x, y;

  for ( y = 0; y < FBHEIGHT; y++ ) {

    i = 0;
    for ( x = 0; x < FBWIDTH; x++ ) {

      if ( i >= 9 ) {
        *( _g_framebuffer + ( y * FBWIDTH ) + x ) = 0xFF;
      }

      if ( i == 12 ) {
        i = 0;
      }

      i++;
    } // x
  } // y
#endif

}
