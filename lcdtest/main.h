#ifndef _main_h_
#define _main_h_

#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include "mydefs.h"

#define	XTAL	20000000UL			// 20MHz

#define F_CPU   XTAL
#include <util/delay.h>


#define	LCD_D4		SBIT( PORTD, 6 )
#define	LCD_DDR_D4	SBIT( DDRD, 6 )

#define	LCD_D5		SBIT( PORTD, 5 )
#define	LCD_DDR_D5	SBIT( DDRD, 5 )

#define	LCD_D6		SBIT( PORTD, 4 )
#define	LCD_DDR_D6	SBIT( DDRD, 4 )

#define	LCD_D7		SBIT( PORTD, 3 )
#define	LCD_DDR_D7	SBIT( DDRD, 3 )

#define	LCD_RS		SBIT( PORTD, 1 )
#define	LCD_DDR_RS	SBIT( DDRD, 1 )

#define	LCD_E0		SBIT( PORTD, 2 )
#define	LCD_DDR_E0	SBIT( DDRD, 2 )


#endif
