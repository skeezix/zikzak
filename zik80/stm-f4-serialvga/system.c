
#include "stm32f4xx.h"

#include "system.h"

/* STM32 F4 -> 112+16+64 = 192KB
 * = base SRAM is 112KB
 * + CCM (Core Coupled Memory)
 * + SRAM bank 2
 */
uint8_t *ccm_memory_base = (uint8_t *)(0x10000000); // 64KB - Core Coupled Memory - no peripheral can see it, purely cpu itself
uint8_t *sram2_16k = (uint8_t*) (0x2001C000);       // 16KB - Extra sram bank
