
#include "stm32f4xx.h"

#include "system.h"

// Core Coupled Memory - no peripheral can see it, purely cpu itself
uint8_t *ccm_memory_base = (uint8_t *)(0x10000000); // 64KB
uint8_t *sram2_16k = (uint8_t*) (0x2001C000);       // 16KB
