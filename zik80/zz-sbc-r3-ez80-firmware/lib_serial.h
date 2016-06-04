
#ifndef h_lib_serial_h
#define h_lib_serial_h

void uart_putstring ( char *foo );
char uart_getchar_block ( void );
void uart_putchar ( char c );
void serial_echo_loop_forever ( void );

#endif
