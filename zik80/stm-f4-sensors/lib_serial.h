#ifndef h_lib_serial_h
#define h_lib_serial_h

void init_usart1 ( uint32_t baudrate ); // for GPU bus to processor

void init_usart2 ( uint32_t baudrate ); // for console logging

void USART_puts ( USART_TypeDef* USARTx, volatile char *s );

#endif
