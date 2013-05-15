#ifndef _main_h_
#define _main_h_

#ifndef F_CPU
#define F_CPU 20000000UL  /* 20 MHz CPU clock */
#endif

// skee board
// PD 0-3 -> SRAM A0-A3
// PD 5   -> SRAM WE (write enable)   \ mutually exclusive
// PD 6   -> SRAM OE (read enable)    /
// PA 0-3 -> SRAM I/O 0-3

#define LEDPORT      PORTB
#define LED          (1<<2)
#define LED_OFF      PORTB &= ~(1<<2)
#define LED_ON       PORTB |= (1<<2)

#define WE_HIGH      PORTD |= (1<<4)
#define WE_LOW       PORTD &= ~(1<<4)

#define OE_HIGH      PORTD |= (1<<5)
#define OE_LOW       PORTD &= ~(1<<5)

#define CE_HIGH      PORTD |= (1<<6)
#define CE_LOW       PORTD &= ~(1<<6)

#define DATABUS_DIR  DDRA
#define DATABUS_PORT PORTA
#define DIR_OUT      0xFF
#define DIR_IN       0x00

#endif
