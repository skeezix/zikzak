#include "main.h"
#include "lcd_drv.h"


uint8_t lcd_pos = LCD_LINE1;


static void lcd_nibble( uint8_t d )
{
  LCD_D7 = 0; if( d & 1<<7 ) LCD_D7 = 1;
  LCD_D6 = 0; if( d & 1<<6 ) LCD_D6 = 1;
  LCD_D5 = 0; if( d & 1<<5 ) LCD_D5 = 1;
  LCD_D4 = 0; if( d & 1<<4 ) LCD_D4 = 1;
  LCD_E0 = 1;
  _delay_us( LCD_TIME_ENA );
  LCD_E0 = 0;
}


static void lcd_byte( uint8_t d )
{
  lcd_nibble( d );
  lcd_nibble( d<<4 );
  _delay_us( LCD_TIME_DAT );
}


void lcd_command( uint8_t d )
{
  LCD_RS = 0;
  lcd_byte( d );
  switch( d ){
    case 0 ... 3:                       // on longer commands
      _delay_us( LCD_TIME_CLR );
      d = LCD_LINE1;
    case 0x80 ... 0xFF:                 // set position
      lcd_pos = d;
  }
}


void lcd_putchar( uint8_t d )
{
  LCD_RS = 1;
  lcd_byte( d );
  switch( ++lcd_pos ){
    case LCD_LINE1 + LCD_COLUMN:
      d = LCD_LINE2;
      break;
    case LCD_LINE2 + LCD_COLUMN:
      d = LCD_LINE1;
      break;
    default: 
      return;
  }
  lcd_command( d );
}


void lcd_puts( void *s )                        // display string from SRAM
{
  uint8_t *s1;
  for( s1 = s; *s1; s1++ )             // until zero byte
    lcd_putchar( *s1 );
}


void lcd_blank( uint8_t len )                   // blank n digits
{
  while( len-- )
    lcd_putchar( ' ' );
}


void lcd_init( void )
{
  LCD_DDR_D4 = 1;                               // enable output pins
  LCD_DDR_D5 = 1;
  LCD_DDR_D6 = 1;
  LCD_DDR_D7 = 1;
  LCD_DDR_RS = 1;
  LCD_DDR_E0 = 1;
  LCD_E0 = 0;
  LCD_RS = 0;                                   // send commands

  _delay_ms( 15 );
  lcd_nibble( 0x30 );
  _delay_ms( 4.1 );
  lcd_nibble( 0x30 );
  _delay_us( 100 );
  lcd_nibble( 0x30 );
  _delay_us( LCD_TIME_DAT );
  lcd_nibble( 0x20 );                           // 4 bit mode
  _delay_us( LCD_TIME_DAT );
  lcd_command( 0x28 );                          // 2 lines 5*7
  lcd_command( 0x08 );                          // display off
  lcd_command( 0x01 );                          // display clear
  lcd_command( 0x06 );                          // cursor increment
  lcd_command( 0x0C );                          // on, no cursor, no blink
}
