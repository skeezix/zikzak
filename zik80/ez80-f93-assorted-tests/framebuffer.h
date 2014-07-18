#ifndef h_framebuffer_h
#define h_framebuffer_h

#define FBWIDTH 256
#define FBHEIGHT 192

void fb_render_rect_filled ( UINT8 *fb, UINT8 x, UINT8 y, UINT8 w, UINT8 h, UINT8 rgb );
void zl_render_line ( UINT8 *bits, UINT8 c, UINT16 x0, UINT16 y0, UINT16 x1, UINT16 y1 );
void zl_render_blit32 ( UINT8 *from, UINT8 *to,
                        UINT8 x1, UINT8 y1, UINT8 w, UINT8 h,
                        UINT8 to_x, UINT8 to_y );
void zl_render_blit8 ( UINT8 *from, UINT8 *to,
                       UINT8 x1, UINT8 y1, UINT8 w, UINT8 h,
                       UINT8 to_x, UINT8 to_y );


#endif
