
#ifndef h_main_h
#define h_main_h

//#define F_CPU 1000000UL  /* 1 MHz CPU clock */
//#define F_CPU 8000000UL  /* 8 MHz CPU clock */
//#define F_CPU 20000000UL  /* 20 MHz CPU clock */
#define F_CPU 16000000UL  /* 20 MHz CPU clock */

#define nop asm volatile ("nop\n\t")

void doOneScanLine ();

#endif
