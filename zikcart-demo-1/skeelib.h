#ifndef h_skeelib_h
#define h_skeelib_h

// types
typedef UCHAR uint8_t;
typedef UINT16 uint16_t;

// string
char* lame_itoa ( int i, char b[] );
UINT16 lame_strlen ( char *p );

// numerics
UINT8 lame_randrange8 ( UINT8 min, UINT8 width );

// time / delay
#pragma noopt
static void delay_loop ( unsigned int c ) {
	while ( c ) {
		c--;
	}
}
#pragma noopt

void delay_ms_spin ( unsigned int ms );

#endif
