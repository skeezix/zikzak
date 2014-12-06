#ifndef h_lib_ay_3_8912_h
#define h_lib_ay_3_8912_h

void ay_gpio_setup ( void );

void ym_play_demo ( void );

void ay_set_chA ( int i );
void ay_set_chB ( int i );
void delay_ms_spin ( unsigned int ms );

void ay_set_chA ( int i );
void ay_set_chB ( int i );
UINT8 ay_write ( unsigned char address, unsigned char data );

#endif
