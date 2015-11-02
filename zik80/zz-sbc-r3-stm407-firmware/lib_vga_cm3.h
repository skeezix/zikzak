#ifndef h_lib_vga_h
#define h_lib_vga_h

extern volatile unsigned char vblank;                // is >0 when inside of vblank period
extern volatile unsigned long int _vblank_count;     // increments at end of each vblank

#define VGA_USE_DMA 1
#define VGA_NO_DMA  0
void vga_setup ( unsigned char use_dma );

static inline void _fb_spin_until_vblank ( void ) {
  while ( ! vblank ) {
    __asm__("nop");
  }

}

#endif
