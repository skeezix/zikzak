
#ifndef h_dma_memcpy_h
#define h_dma_memcpy_h

void dma_setup ( void );

#define DMA_MEMCPY_INCSRC   1
#define DMA_MEMCPY_INCDST   2
#define DMA_MEMCPY_M2M      4
#define DMA_MEMCPY_WAIT     8 /* wait for transfer to complete; default is to just return, and let DMA do its thing in background */
void dma_memcpy ( unsigned char *src, unsigned char *dst, unsigned int len, unsigned char flags );

#endif
