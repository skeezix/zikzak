/************************************************************************/
/*                                                                      */
/*                      Several helpful definitions                     */
/*                                                                      */
/*              Author: Peter Dannegger                                 */
/*                                                                      */
/************************************************************************/
#ifndef _mydefs_h_

#define _mydefs_h_
#include<avr/io.h>

// Access bits like variables
struct bits {
  uint8_t b0:1, b1:1, b2:1, b3:1, b4:1, b5:1, b6:1, b7:1;
} __attribute__((__packed__));

#define SBIT_(port,pin) ((*(volatile struct bits*)&port).b##pin)
#define	SBIT(x,y)       SBIT_(x,y)

// force access of interrupt variables
#define IVAR(x)         (*(volatile typeof(x)*)&(x))

// always inline function x
#define AIL(x)          static x __attribute__ ((always_inline)); static x

// NOP
#define nop()           __asm__ volatile("nop"::)

#endif
