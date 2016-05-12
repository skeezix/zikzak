#ifndef h_uart_h
#define h_uart_h

void uart_putchar_prewait (char c);
void uart_putchar_postwait (char c);
char uart_getchar(void);

#endif
