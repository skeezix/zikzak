#ifndef h_skeelib_h
#define h_skeelib_h

// string
char* lame_itoa ( int i, char b[] );
UINT16 lame_strlen ( char *p );

// time / delay
static void delay_loop ( unsigned int c ) {
	while ( c ) {
		c--;
	}
}

void delay_ms_spin ( unsigned int ms );

#endif
