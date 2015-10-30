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

// map music notes to frequencies
#define A_FREQ 2034
#define Asharp_FREQ 1920
#define B_FREQ 1892
#define C_FREQ 3421
#define Csharp_FREQ 3228
#define D_FREQ 3047
#define Dsharp_FREQ 2876
#define E_FREQ 2715
#define F_FREQ 2562
#define Fsharp_FREQ 2419
#define G_FREQ 2283
#define Gsharp_FREQ 2155

#endif
