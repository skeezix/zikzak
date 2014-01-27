
#ifndef h_framebuffer_h
#define h_framebuffer_h

#define FBWIDTH 320 /* 256 */
#define FBHEIGHT 200 /* 256 */

extern volatile unsigned char framebuffer [ FBWIDTH * FBHEIGHT ] /*__attribute((aligned (1024)))*/;

void fb_test_pattern ( void );

#endif
