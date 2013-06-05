#ifndef h_nop_h
#define h_nop_h

#define NOP          __asm__("nop\n\t") /* nop - 1 cycle */
#define NOP2         __asm__("nop\n\t""nop\n\t")
#define NOP3         __asm__("nop\n\t""nop\n\t""nop\n\t")
#define NOP4         __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t")
#define NOP5         __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t")
#define NOP10        NOP5;NOP5
#define NOP20        NOP5;NO5;NOP5;NOP5

#endif
