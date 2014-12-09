
#include <stm32f4xx.h>
#include <string.h>

#include "config.h"
#include "lib_serial.h"

#include "logger.h"
#include "framebuffer.h"
#include "framebuffer_demo.h"
#include "lib_bus_ram.h"
#include "skeelib.h"
#include "lib_i2c.h"
#include "framebuffer_update.h"
#include "lib_textmode.h"
#include "HAL.h"

void framebuffer_update_from_ram_forever ( void ) {

  while ( 1 ) {
    framebuffer_update_from_ram();
  } // while

  return;
}

void framebuffer_update_from_ram ( void ) {

  unsigned char sawblank = 0;     // will be 1 when entering vblank code, so can vblank once; while still in vblank, goes to 2..
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

#if 0
    if ( blankcounter < 2 ) {
      blankcounter++;
      continue;
    }
    blankcounter = 0;
#endif

    // pull data from external RAM
    tm_fetch_ram();

    // render to framebuffer
#if 1
    tm_render_to_framebuffer ( fb_active );
#else
    tm_render_to_framebuffer ( fb_inactive );

    // swap framebuffers for smooth animation
    // -> swap only if still in vblank, otherwise wait till next frame and do it then
    //fb_swap();

    while ( ! vblank ) {
      __asm__("nop");
    }
    fb_swap();
#endif

    while ( vblank ) {
      __asm__("nop");
    }

  } // while running

  return;
}
