
#include "stm32f4xx.h"
#include "core_cm4.h"

#include "rcc_swipe.h"
#include "int_swipe.h"
#include "timers.h"

void nvic_setup ( void ) {
  /* Without this the timer interrupt routine will never be called. */
  //nvic_enable_irq(NVIC_TIM2_IRQ);
}

void timers_setup ( void ) {

  SysTick_Config ( HCLKFrequency()/100 );

  /* Enable TIM2 clock. */
  //rcc_peripheral_enable_clock(&RCC_APB1ENR, RCC_APB1ENR_TIM2EN);

}

void tim2_isr ( void ) {

  //TIM_SR(TIM2) &= ~TIM_SR_UIF; /* Clear interrrupt flag. */

#if 0
  __asm__("nop");
  gpio_toggle(GPIOB, GPIO12); /* LED on/off. */
#endif

}
