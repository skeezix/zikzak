#ifndef h_system_h
#define h_system_h

// features
#define RENDER_ATALL 1 /* yes, we want rendering to occur, not just sync */
#define RENDER_DMA 1 /* use DMA dma_memcpy; otherwise, use brute force! */
//#define TORTURE_TIMER 1 /* enable a torture timer which does GPIOs */
#define SERVER_ON 1 /* if undefined, no input server present */

// Core Coupled Memory - no peripheral can see it, purely cpu itself
extern uint8_t *ccm_memory_base;      // 64KB

// SRAM2
extern uint8_t *sram2_16k;            // 16KB


#endif
