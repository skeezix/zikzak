
#include <stm32f4xx.h>
#include <misc.h>
#include <stm32f4xx_usart.h> // under Libraries/STM32F4xx_StdPeriph_Driver/inc and src
#include <string.h>

#include "main.h"
#include "lib_serial.h"
//#include "/home/skeezix/archive/devo/toolkits/stm32/libopencm3/include/libopencm3/stm32/f4/nvic.h"
#include "lib_vga_cm3.h"
#include "framebuffer.h"
#include "system_cm3.h"

#define MAX_STRLEN 12 // this is the maximum string length of our string in characters
volatile char received_string[MAX_STRLEN+1]; // this will hold the recieved string

int main(void) {

  system_cm3_setup();

#if 1
  init_usart1 ( 9600 ); // initialize USART1 @ 9600 baud
  init_usart2 ( 9600 );

  //USART_puts ( USART1, "Init usart1 complete! Hello World!\r\n" );
  USART_puts ( USART2, "Init usart2 complete! Hello World!\r\n" );
#endif

#if 1

  fb_setup();

  vga_setup();
#endif

  fb_test_pattern ( fb_active );

  while ( 1 ) {
    // weeeeee!
  }

} // main

// this is the interrupt request handler (IRQ) for ALL USART1 interrupts
void usart1_isr ( void ) {

  // check if the USART1 receive interrupt flag was set
  if ( USART_GetITStatus ( USART1, USART_IT_RXNE ) ) {

    static uint8_t cnt = 0; // this counter is used to determine the string length
    char t = USART1->DR; // the character from the USART1 data register is saved in t

    /* check if the received character is not the LF character (used to determine end of string)
     * or the if the maximum string length has been been reached
     */
    if( (t != '\n') && (cnt < MAX_STRLEN) ){
      received_string[cnt] = t;
      cnt++;
    } else { // otherwise reset the character counter and print the received string
      cnt = 0;
      //USART_puts(USART1, received_string);
      USART_puts ( USART2, "avr: " );
      USART_puts ( USART2, received_string );
      USART_puts ( USART2, "\r\n" );
    }

  } // if USART received

} // func

// this is the interrupt request handler (IRQ) for ALL USART1 interrupts
void usart2_isr ( void ) {

  // check if the USART1 receive interrupt flag was set
  if ( USART_GetITStatus ( USART2, USART_IT_RXNE ) ) {

    static uint8_t cnt = 0; // this counter is used to determine the string length
    char t = USART2->DR; // the character from the USART1 data register is saved in t

    /* check if the received character is not the LF character (used to determine end of string)
     * or the if the maximum string length has been been reached
     */
    if( (t != '\r') && (cnt < MAX_STRLEN) ){
      received_string[cnt] = t;
      cnt++;
    } else { // otherwise reset the character counter and print the received string
      cnt = 0;
      USART_puts ( USART2, "console: " );
      USART_puts ( USART2, received_string );
      USART_puts ( USART2, "\r\n" );
      memset ( (void*) received_string, '\0', MAX_STRLEN );
    }

  } // if USART received

} // func
