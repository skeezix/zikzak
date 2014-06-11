
#include <stm32f4xx.h>

#include "command.h"
#include "commandqueue.h"
#include "logger.h"
#include "framebuffer.h"
#include "framebuffer_demo.h"

void command_queue_run ( void ) {
  char *c;

  while ( ( c = queuepull() ) ) {

    //switch ( *((uint16_t*)c) ) {
    switch ( c[0] + (c[1]<<8) ) {

    case LD:
      //logit ( "LD\r\n" );
      zl_render_line ( fb_active, c [ 2 ], c [ 3 ], c [ 4 ], c [ 5 ], c [ 6 ] );
      break;

    case CB:
      fb_render_rect_filled ( fb_active, 0, 0, FBWIDTH - 1, FBHEIGHT - 1, 0x00 );
      break;

    case DP:
      fb_test_pattern ( fb_active, fbt_offset_squares );
      break;

    default:
      logit ( "+ERR\r\n" );

    } // switch on command

  } // while commands in queue

  return;
}
