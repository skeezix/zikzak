
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/timer.h>

#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/exti.h>

#include <libopencm3/cm3/cortex.h>

#include "gpio.h"

void gpio_setup ( void ) {

  /* Enable GPIOC clock, and set modes
   */

  // blinkenlight
  rcc_peripheral_enable_clock ( &RCC_AHB1ENR, RCC_AHB1ENR_IOPGEN );

  gpio_mode_setup ( GPIOG, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO13 );

  // syncs and colours
  rcc_peripheral_enable_clock ( &RCC_AHB1ENR, RCC_AHB1ENR_IOPBEN );
  rcc_peripheral_enable_clock ( &RCC_AHB1ENR, RCC_AHB1ENR_IOPCEN );

  // VGA
  //
  // sync lines
  gpio_mode_setup ( GPIOB, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO10 ); // vsync
  gpio_mode_setup ( GPIOB, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO11 ); // hsync
  // colour
  gpio_mode_setup ( GPIOC, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO0 ); // red
  gpio_mode_setup ( GPIOC, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO1 ); // red
  gpio_mode_setup ( GPIOC, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO2 ); // green
  gpio_mode_setup ( GPIOC, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO3 ); // green
  gpio_mode_setup ( GPIOC, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO4 ); // blue
  gpio_mode_setup ( GPIOC, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO5 ); // blue
  // speed
  gpio_set_output_options ( GPIOB, GPIO_OTYPE_PP, GPIO_OSPEED_100MHZ, GPIO10 | GPIO11 );
  gpio_set_output_options ( GPIOC, GPIO_OTYPE_PP, GPIO_OSPEED_100MHZ, GPIO0 | GPIO1 | GPIO2 | GPIO3 | GPIO4 | GPIO5 );

  // reset
  //
  gpio_set ( GPIOG, GPIO13 ); // LED
  gpio_set ( GPIOB, GPIO10 ); // vsync
  gpio_set ( GPIOB, GPIO11 ); // hsync
  gpio_clear ( GPIOC, GPIO0 | GPIO1 ); // red
  gpio_clear ( GPIOC, GPIO2 | GPIO3 ); // green
  gpio_clear ( GPIOC, GPIO4 | GPIO5 ); // blue

}

void gpio_toggle_blinkenlight ( void ) {
  gpio_toggle (GPIOG, GPIO13 ); /* LED on/off. */
}
void gpio_set_blinkenlight ( unsigned char onoff ) {
  if ( onoff ) {
    gpio_set ( GPIOG, GPIO13 ); // LED
    return;
  }
  gpio_clear ( GPIOG, GPIO13 ); // LED
}
