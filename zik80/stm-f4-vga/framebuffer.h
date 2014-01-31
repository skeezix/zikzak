
#ifndef h_framebuffer_h
#define h_framebuffer_h

#define FBWIDTH 320 /* 256 */
#define FBHEIGHT 200 /* 256 */

extern uint8_t framebuffer [ FBWIDTH * FBHEIGHT ] /*__attribute((aligned (1024)))*/;
extern uint8_t offscreen [ FBWIDTH * FBHEIGHT ] /*__attribute((aligned (1024)))*/;

void fb_test_pattern ( void );
void fb_lame_demo_animate ( uint8_t *fb );

void fb_clone ( uint8_t *fbsrc, uint8_t *fbdst );
void fb_render_rect_filled ( uint8_t *fb, uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t rgb );


#endif
