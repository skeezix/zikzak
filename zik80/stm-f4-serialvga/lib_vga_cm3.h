#ifndef h_lib_vga_h
#define h_lib_vga_h

extern volatile unsigned char vblank;          // is >0 when inside of vblank period

#define VGA_USE_DMA 1
#define VGA_NO_DMA  0
void vga_setup ( unsigned char use_dma );

#endif
