
#include "stm32f4xx.h"

#include "system.h"

/* STM32 F407 -> 112+16+64 = 192KB
 * = base SRAM is               112KB
 * + CCM (Core Coupled Memory)  64KB
 * + SRAM bank 2                16KB
 *
 * STM32 F429 -> 256KB ..  112+64sram3+16+64CCM
 */
uint8_t *ccm_memory_base = CCM_MEMORY_BASE;   // 64KB - Core Coupled Memory - no peripheral can see it, purely cpu itself
uint8_t *sram2_16k = SRAM2_MEMORY_BASE;       // 16KB - Extra sram bank
// normal sram1 112KB -> 0x2000000
uint8_t *sram3_64k_f429 = SRAM3_MEMORY_BASE;  // 16KB - Extra sram bank

