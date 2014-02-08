
#include "stm32f4xx.h"
#include "stm32f4xx_dma.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_exti.h"
#include "stm32f4xx_syscfg.h"
#include "misc.h"

#include "gpio.h"
#include "server.h"

uint8_t server_queue [ SERVER_QUEUE_MAX ];
volatile uint16_t server_itemcount = 0;

// B10 vsync, B11 hsync
// B12 vblank active (high when vblank up)
// C is for VGA data
// LED is G13

// listen on Port E (E0-7 for data)
// B0 is clock-in, that E0-7 are ready

void server_setup ( void ) {
  EXTI_InitTypeDef   EXTI_InitStructure;
  GPIO_InitTypeDef   GPIO_InitStructure;
  NVIC_InitTypeDef   NVIC_InitStructure;

  // clock for port B already set enabled
  // clock for port E already set enabled
  //RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

  /* Enable SYSCFG clock */
  RCC_APB2PeriphClockCmd ( RCC_APB2Periph_SYSCFG, ENABLE );

  /* Server CLK-IN: Configure PB0 pin as input floating */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN; // GPIO_PuPd_NOPULL GPIO_PuPd_UP GPIO_PuPd_DOWN
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_Init ( GPIOB, &GPIO_InitStructure );

  /* Connect EXTI Line0 to PA0 pin */
  SYSCFG_EXTILineConfig ( EXTI_PortSourceGPIOB, EXTI_PinSource0 );
 
  /* Configure EXTI Line0 */
  EXTI_InitStructure.EXTI_Line = EXTI_Line0;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;  
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init ( &EXTI_InitStructure );
 
  /* Enable and set EXTI Line0 Interrupt to the lowest priority */
  NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; // 0x0F; // 0highest 15lowest
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; // 0x0F; // 0highest 15lowest
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init ( &NVIC_InitStructure );

  /* Server DATA-IN: PortE as input floating */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN; // GPIO_PuPd_NOPULL GPIO_PuPd_UP GPIO_PuPd_DOWN
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3
                              | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
  GPIO_Init ( GPIOE, &GPIO_InitStructure );

}

void exti0_isr ( void ) {
  uint16_t i;

  /* Toggle LED1 */
  //gpio_toggle_blinkenlight();

  if ( EXTI_GetITStatus ( EXTI_Line0 ) != RESET ) {

    /* Toggle LED1 */
    gpio_toggle_blinkenlight();
     
    // append to queue
    if ( server_itemcount < SERVER_QUEUE_MAX ) {
      i = server_itemcount++;
      server_queue [ i ] = (uint8_t) GPIO_ReadInputData ( GPIOE );

      //if ( server_queue [ i ] > 0 ) {
      //gpio_toggle_blinkenlight();
      //}

    } // queue full?

    /* Clear the EXTI line 0 pending bit */
    EXTI_ClearITPendingBit ( EXTI_Line0 );

  } // exti0 status

  __asm__("nop");

} // exti0 isr

void server_clear ( void ) {
  server_itemcount = 0;
}

void server_insert ( uint8_t b ) {
  server_queue [ server_itemcount++ ] = b;
}
