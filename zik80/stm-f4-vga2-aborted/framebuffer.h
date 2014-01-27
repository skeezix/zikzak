
#ifndef h_framebuffer_h
#define h_framebuffer_H

#define FBWIDTH 320
#define FBHEIGHT 200

extern unsigned char _g_framebuffer [ FBWIDTH * FBHEIGHT ];

void framebuffer_setup ( void );

#endif
