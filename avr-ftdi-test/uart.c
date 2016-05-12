
#include "main.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>   // include interrupt support
#include <util/setbaud.h>
#include "uart.h"

/* to support stdio streams, see here:
 * http://www.appelsiini.net/2011/simple-usart-with-avr-libc
 */

void uart_putchar_prewait (char c) {
  loop_until_bit_is_set(UCSR0A, UDRE0); /* Wait until data register empty. */
  UDR0 = c;
}

void uart_putchar_postwait (char c) {
  UDR0 = c;
  loop_until_bit_is_set(UCSR0A, TXC0); /* Wait until transmission ready. */
}

char uart_getchar(void) {
  loop_until_bit_is_set(UCSR0A, RXC0); /* Wait until data exists. */
  return UDR0;
}
