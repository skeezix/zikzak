#ifndef h_framebuffer_demo_h
#define h_framebuffer_demo_h

typedef enum {
  fbt_offset_squares = 0,
  fbt_full_palette_grid,
  fbt_full_palette_grid_ordered,
  fbt_vlines,
  fbt_vwlines,
  fbt_v1lines,
  fbt_onoff1,
  fbt_topbottom,
  fbt_spriteram,
  fbt_reds,
  fbt_greens,
  fbt_blues,
  // end
  fbt_max
} fbt_e;

void fb_test_pattern ( uint8_t *fb, fbt_e pattern );

#endif
