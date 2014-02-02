#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/timer.h>

#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/exti.h>

#include <libopencm3/cm3/cortex.h>

#include "torture.h"
#include "gpio.h"

extern volatile unsigned char vblank;

void torture_setup ( void ) {
  rcc_peripheral_enable_clock(&RCC_APB1ENR, RCC_APB1ENR_TIM3EN);

  timer_reset(TIM3);
  // 24Mhz / 10khz -1.
  timer_set_prescaler(TIM3, 20000); // 24Mhz/10000hz - 1
  // 10khz for 10 ticks = 1 khz overflow = 1ms overflow interrupts
  timer_set_period(TIM3, 1);


  /* Set timer start value. */
  TIM_CNT(TIM3) = 1;

  /* Set timer prescaler. 72MHz/1440 => 50000 counts per second. */
  TIM_PSC(TIM3) = 541; // 120M/2000 = 60k/second   ## 540

  /* End timer value. If this is reached an interrupt is generated. */
  TIM_ARR(TIM3) = 200; // ## 2



  nvic_enable_irq(NVIC_TIM3_IRQ);
  timer_enable_update_event(TIM3); // default at reset!
  timer_enable_irq(TIM3, TIM_DIER_UIE);
  timer_enable_counter(TIM3);

}

void tim3_isr(void) {
  TIM_SR(TIM3) &= ~TIM_SR_UIF;

  if ( 1||vblank ) {
    uint16_t a = gpio_port_read ( GPIOC );
    a = gpio_port_read ( GPIOC );
    a = gpio_port_read ( GPIOC );
    a = gpio_port_read ( GPIOC );
    a = gpio_port_read ( GPIOC );
    a = gpio_port_read ( GPIOC );
    a = gpio_port_read ( GPIOC );
    a = gpio_port_read ( GPIOC );
    a = gpio_port_read ( GPIOC );
    a = gpio_port_read ( GPIOC );
    a = gpio_port_read ( GPIOC );
    a = gpio_port_read ( GPIOC );
    a = gpio_port_read ( GPIOC );
    a = gpio_port_read ( GPIOC );
    a = gpio_port_read ( GPIOC );
    a = gpio_port_read ( GPIOC );
    a = gpio_port_read ( GPIOC );
    if ( a = 37 ) {
      //gpio_toggle_blinkenlight();
      __asm__("nop");
    }

  }

}
