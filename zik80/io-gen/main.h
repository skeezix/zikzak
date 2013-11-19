
#ifndef h_iomain_h
#define h_iomain_h

//#define F_CPU 1000000UL  /* 1 MHz CPU clock */
#define F_CPU 8000000UL  /* 8 MHz CPU clock */
//#define F_CPU 20000000UL  /* 20 MHz CPU clock */


#define NOP          __asm__("nop\n\t") /* nop - 1 cycle */



void blinkit ( void );


#endif
