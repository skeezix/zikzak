#ifndef _main_h_
#define _main_h_

#include <avr/interrupt.h>
#include <avr/pgmspace.h>

// avoid push in main
int main( void )        __attribute__((OS_main));

#include "avrutil.h"

//#define F_CPU   20000000UL /* 20MHz */
#define F_CPU   8000000UL /* 20MHz */
#include <util/delay.h>

#define BAUD 9600 /* for sabertooth motor controller */

#endif
