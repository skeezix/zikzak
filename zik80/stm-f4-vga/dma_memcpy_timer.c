
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/timer.h>

#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/exti.h>

#include <libopencm3/cm3/cortex.h>

void pixelclock_setup ( void ) {

  // enable timer for pixel clock
  nvic_enable_irq(NVIC_TIM8_UP_TIM13_IRQ);

}


void pixelclock_start ( void ) {

  /* Set timer start value. */
  TIM_CNT(TIM8) = 1;

  /* Set timer prescaler. 72MHz/1440 => 50000 counts per second. */
  TIM_PSC(TIM8) = 1; // 120M/2000 = 60k/second   ## 540

  /* End timer value. If this is reached an interrupt is generated. */
  TIM_ARR(TIM8) = 200; // ## 2

  // o-scope reports:
  // prescale 2000, 1->600 should be 100/sec; in fact, we're exactly 20ms between which is exactly 50 .. so callback is 60MHz, not 120MHz
  // manual says:
  //  The reference manual (see page 133) states that the GPIO is capable of:
  //  Fast toggle capable of changing every two clock cycles
  // --> okay so at 120MHz, the best we can do is 60MHz of GPIO. But thats different than here, where the timer seems halved..


  /* Update interrupt enable. */
  TIM_DIER(TIM8) |= TIM_DIER_UDE; // TIM_DIER_UIE

  //timer_set_repetition_counter ( TIM2, 100 );

  /* Start timer. */
  TIM_CR1(TIM8) |= TIM_CR1_CEN;
  TIM_CR1(TIM8) |= TIM_CR1_ARPE;

}

void pixelclock_stop ( void ) {
  TIM_CR1(TIM8) &= ~TIM_CR1_CEN;
}
