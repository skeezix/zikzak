#ifndef h_lib_serial_h
#define h_lib_serial_h

void init_usart1 ( uint32_t baudrate ); // for GPU bus to processor

void init_usart2 ( uint32_t baudrate ); // for console logging


void USART_puts ( USART_TypeDef* USARTx, volatile char *s );

void usart_puts_optional_set (uint8_t onoff );
/* static inline */ void USART_puts_optional ( USART_TypeDef* USARTx, volatile char *s );


void suspend_usart2 ( void );
void resume_usart2 ( void );

#endif
