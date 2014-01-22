
#ifndef h_dma_memcpy_h
#define h_dma_memcpy_h

void dma_setup ( void );

void dma_memcpy ( unsigned char *p, unsigned int len );

#define DMA_MEMCPY_INCSRC   1
#define DMA_MEMCPY_INCDEST  2
#define DMA_MEMCPY_WAIT     4 /* wait for transfer to complete; default is to just return, and let DMA do its thing in background */
void dma_memcpy_general ( unsigned char *src, unsigned char *dest, unsigned int len, unsigned char flags );

#endif
