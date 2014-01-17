
#ifndef h_dma_memcpy_h
#define h_dma_memcpy_h

void dma_setup ( void );

void dma_memcpy ( unsigned char *p, unsigned int len );

void DMA2_Stream6_IRQHandler ( void );

#endif
