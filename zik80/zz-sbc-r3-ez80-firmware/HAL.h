
/* zikzak sbc rev3
 * Jeff Mitchell 2014
 */
 
 // HAL.h is the basic hardware definitions

// zz sbc rev3:
// ROM cart spec limits cartridge to 4MB address space
 
// Chip Selects:
// CS0 e8 0 0c0000 0dffff -> ram (128KB)
//                 13FFFF -> ram (512KB)
// CS1 e8 0 1c0000 5bffff -> rom - space is 4MB regardless of cart size
// CS2 e8 0 5c0000 5cffff -> audio
//
// 2.457MHz oscilator in place
