
#include <eZ80.h>
#include <uart.h>
#include <gpio.h>
#include <stdlib.h>
#include <string.h>

#include "lib_serial.h"
#include "skeelib.h"

void uart_putstring ( char *foo ) {
  write_UART0 ( foo, lame_strlen ( foo ) );
  return;
}

char uart_getchar_block ( void ) {
	char b [ 2 ];
	UINT24 nbytes = 1;
	read_UART0 ( b, &nbytes );
	return b [ 0 ];
}

void uart_putchar ( char c ) {
	char b [ 2 ];
	b [ 0 ] = c;
	b [ 1 ] = '\0';
	write_UART0 ( b, 1 );
}

// rush: test for echo
void uart_echo_loop_forever ( void ) {
  char buffer [ 30 ];
  char e;

  while ( 1 ) {

#if 1
    e = uart_getchar_block();
    sprintf ( buffer, "echo: %c\n", e );
    logit ( buffer );
#endif
#if 0
    if ( uart_is_getchar_avail() ) {
      e = uart_getchar_now();
      sprintf ( buffer, "echo: %c\n", e );
      logit ( buffer );
    }

    _delay_ms ( 1 );
#endif

  } // while

}
