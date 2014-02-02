#ifndef h_system_h
#define h_system_h

// Core Coupled Memory - no peripheral can see it, purely cpu itself
extern uint8_t *ccm_memory_base;      // 64KB

// SRAM2
extern uint8_t *sram2_16k;            // 16KB

#endif
