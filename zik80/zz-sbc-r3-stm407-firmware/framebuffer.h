
#ifndef h_framebuffer_h
#define h_framebuffer_h

// 320x240 * .8 = 256x192 = 49152bytes
#define FBWIDTH 256
#define FBHEIGHT 192

extern uint8_t *fb_active;
extern uint8_t *fb_inactive;
extern uint8_t fb_1 [ FBWIDTH * FBHEIGHT ] /*__attribute((aligned (1024)))*/;
//extern uint8_t *fb_1;
extern uint8_t fb_2 [ FBWIDTH * FBHEIGHT ] /*__attribute((aligned (1024)))*/;

void fb_setup ( void );

void fb_clone ( uint8_t *fbsrc, uint8_t *fbdst );

void fb_render_rect_filled ( uint8_t *fb, uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t rgb );
void fb_render_rect8_filled ( uint8_t *fb, uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t rgb );
void fb_clear ( uint8_t *fb );

void zl_render_line ( uint8_t *bits, uint8_t c, uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1 );
void zl_render_blit32 ( uint8_t *from, uint8_t *to,
                        uint16_t x1, uint16_t y1, uint16_t w, uint16_t h,
                        uint16_t to_x, uint16_t to_y );
void zl_render_blit8 ( uint8_t *from, uint8_t *to,
                       uint16_t x1, uint16_t y1, uint16_t w, uint16_t h,
                       uint16_t to_x, uint16_t to_y );

void fb_swap ( void );

#endif
