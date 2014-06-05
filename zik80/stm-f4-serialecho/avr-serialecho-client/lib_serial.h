#ifndef h_lib_serial_h
#define h_lib_serial_h

void uart_setup ( void );
void uart_stream_setup ( void ); // optional

void uart_putchar_pre ( char c );
void uart_putchar_post ( char c );
char uart_getchar ( void );
void uart_putchar_stream ( char c, FILE *stream );
char uart_getchar_stream ( FILE *stream );

#endif
