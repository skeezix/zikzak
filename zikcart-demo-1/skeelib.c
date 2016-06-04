#include <eZ80.h>
#include <stdio.h>

#include "skeelib.h"

char* lame_itoa ( int i, char b[] ) {
	char const digit[] = "0123456789";
	int shifter;
	char* p = b;
	
	if ( i < 0 ){
		*p++ = '-';
		i *= -1;
	}
	
	shifter = i;
	
	do { //Move to where representation ends
		++p;
		shifter = shifter/10;
	} while ( shifter );
	
	*p = '\0';
	do { //Move back, inserting digits as u go
		*--p = digit[i%10];
		i = i/10;
	} while ( i );

	return ( b );
}

UINT16 lame_strlen ( char *p ) {
	char *e = p;
	
	while ( *e != '\0' ) {
		e++;
	}

	return ( e - p );
}



void delay_ms_spin ( unsigned int ms ) {
  //PC_DR ^= 1;
  while ( --ms ) {
    delay_loop ( 500 );
  }
}

UINT8 lame_randrange8 ( UINT8 min, UINT8 width ) {
	int r = rand() % width;
	return ( min + r );
}
