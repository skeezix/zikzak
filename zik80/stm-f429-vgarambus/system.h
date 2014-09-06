#ifndef h_system_h
#define h_system_h

#define CCM_MEMORY_BASE (uint8_t *)(0x10000000)
#define SRAM2_MEMORY_BASE (uint8_t*) (0x2001C000)
#define SRAM3_MEMORY_BASE (uint8_t*) (0x20020000)

// Core Coupled Memory - no peripheral can see it, purely cpu itself
extern uint8_t *ccm_memory_base;      // 64KB

// SRAM2
extern uint8_t *sram2_16k;            // 16KB

#endif
