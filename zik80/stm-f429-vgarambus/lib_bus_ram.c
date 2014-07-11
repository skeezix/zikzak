#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/timer.h>

#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/exti.h>

#include <libopencm3/cm3/cortex.h>

#include "config.h"
#include "system.h"
#include "lib_bus_ram.h"

// Control bus
// OE      PB0
// WE      PB1
// BUSREQ  PB8
// BUSACK  PB9

// Address bus
// low      PE0-PE15
// highest  PD8-PD13

// Data bus
// PD0-PD7

void bus_setup ( void ) {

  // clocks
  //
  rcc_peripheral_enable_clock ( &RCC_AHB1ENR, RCC_AHB1ENR_IOPBEN ); // control bus
  rcc_peripheral_enable_clock ( &RCC_AHB1ENR, RCC_AHB1ENR_IOPEEN ); // address bus low
  rcc_peripheral_enable_clock ( &RCC_AHB1ENR, RCC_AHB1ENR_IOPDEN ); // address bus high, data bus

  // modes
  //

  // data bus
  gpio_mode_setup ( GPIOD, GPIO_MODE_INPUT, GPIO_PUPD_NONE, GPIO0 );
  gpio_mode_setup ( GPIOD, GPIO_MODE_INPUT, GPIO_PUPD_NONE, GPIO1 );
  gpio_mode_setup ( GPIOD, GPIO_MODE_INPUT, GPIO_PUPD_NONE, GPIO2 );
  gpio_mode_setup ( GPIOD, GPIO_MODE_INPUT, GPIO_PUPD_NONE, GPIO3 );
  gpio_mode_setup ( GPIOD, GPIO_MODE_INPUT, GPIO_PUPD_NONE, GPIO4 );
  gpio_mode_setup ( GPIOD, GPIO_MODE_INPUT, GPIO_PUPD_NONE, GPIO5 );
  gpio_mode_setup ( GPIOD, GPIO_MODE_INPUT, GPIO_PUPD_NONE, GPIO6 );
  gpio_mode_setup ( GPIOD, GPIO_MODE_INPUT, GPIO_PUPD_NONE, GPIO7 );

  // control bus, address bus, and release
  gpio_mode_setup ( GPIOB, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO8 ); // BUSREQ
  gpio_mode_setup ( GPIOB, GPIO_MODE_INPUT, GPIO_PUPD_NONE, GPIO9 ); // BUSACK
  bus_release();

  // should be all clear to survive
  //

}

void bus_grab_and_wait ( void ) {

  // send /BUSREQ low
  gpio_clear ( GPIOB, GPIO8 );

  // wait for /BUSACK
  //while ( gpio_get ( GPIOB, GPIO9 ) > 0 ) {
  //__asm__("nop");
  //}
  unsigned char i;
  for ( i = 0; i < 200; i++ ) {
    __asm__("nop");
  }

  // switch all the inputs around
  //

  // control bus
  gpio_mode_setup ( GPIOB, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, GPIO0 ); // OE
  gpio_mode_setup ( GPIOB, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, GPIO1 ); // WE

  // address bus
  gpio_mode_setup ( GPIOE, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLDOWN, GPIO0 );
  gpio_mode_setup ( GPIOE, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLDOWN, GPIO1 );
  gpio_mode_setup ( GPIOE, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLDOWN, GPIO2 );
  gpio_mode_setup ( GPIOE, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLDOWN, GPIO3 );
  gpio_mode_setup ( GPIOE, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLDOWN, GPIO4 );
  gpio_mode_setup ( GPIOE, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLDOWN, GPIO5 );
  gpio_mode_setup ( GPIOE, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLDOWN, GPIO6 );
  gpio_mode_setup ( GPIOE, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLDOWN, GPIO7 );
  gpio_mode_setup ( GPIOE, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLDOWN, GPIO8 );
  gpio_mode_setup ( GPIOE, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLDOWN, GPIO9 );
  gpio_mode_setup ( GPIOE, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLDOWN, GPIO10 );
  gpio_mode_setup ( GPIOE, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLDOWN, GPIO11 );
  gpio_mode_setup ( GPIOE, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLDOWN, GPIO12 );
  gpio_mode_setup ( GPIOE, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLDOWN, GPIO13 );
  gpio_mode_setup ( GPIOE, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLDOWN, GPIO14 );
  gpio_mode_setup ( GPIOE, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLDOWN, GPIO15 );
  gpio_mode_setup ( GPIOD, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLDOWN, GPIO8 );
  gpio_mode_setup ( GPIOD, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLDOWN, GPIO9 );
  gpio_mode_setup ( GPIOD, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLDOWN, GPIO10 );
  gpio_mode_setup ( GPIOD, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLDOWN, GPIO11 );
  gpio_mode_setup ( GPIOD, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLDOWN, GPIO12 );
  gpio_mode_setup ( GPIOD, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLDOWN, GPIO13 );

  gpio_clear ( GPIOE, GPIO0 | GPIO1 | GPIO2 | GPIO3 | GPIO4 | GPIO5 | GPIO6 | GPIO7 |
                      GPIO8 | GPIO9 | GPIO10 | GPIO11 | GPIO12 | GPIO13 | GPIO14 | GPIO15 );
  gpio_clear ( GPIOD, GPIO8 | GPIO9 | GPIO10 | GPIO11 | GPIO12 | GPIO13 );

}

void bus_release ( void ) {

  // release
  gpio_set ( GPIOB, GPIO8 ); // /BUSREQ high (not active)

  // control bus
  gpio_mode_setup ( GPIOB, GPIO_MODE_INPUT, GPIO_PUPD_NONE, GPIO0 ); // OE
  gpio_mode_setup ( GPIOB, GPIO_MODE_INPUT, GPIO_PUPD_NONE, GPIO1 ); // WE

  // address bus
  gpio_mode_setup ( GPIOE, GPIO_MODE_INPUT, GPIO_PUPD_NONE, GPIO0 );
  gpio_mode_setup ( GPIOE, GPIO_MODE_INPUT, GPIO_PUPD_NONE, GPIO1 );
  gpio_mode_setup ( GPIOE, GPIO_MODE_INPUT, GPIO_PUPD_NONE, GPIO2 );
  gpio_mode_setup ( GPIOE, GPIO_MODE_INPUT, GPIO_PUPD_NONE, GPIO3 );
  gpio_mode_setup ( GPIOE, GPIO_MODE_INPUT, GPIO_PUPD_NONE, GPIO4 );
  gpio_mode_setup ( GPIOE, GPIO_MODE_INPUT, GPIO_PUPD_NONE, GPIO5 );
  gpio_mode_setup ( GPIOE, GPIO_MODE_INPUT, GPIO_PUPD_NONE, GPIO6 );
  gpio_mode_setup ( GPIOE, GPIO_MODE_INPUT, GPIO_PUPD_NONE, GPIO7 );
  gpio_mode_setup ( GPIOE, GPIO_MODE_INPUT, GPIO_PUPD_NONE, GPIO8 );
  gpio_mode_setup ( GPIOE, GPIO_MODE_INPUT, GPIO_PUPD_NONE, GPIO9 );
  gpio_mode_setup ( GPIOE, GPIO_MODE_INPUT, GPIO_PUPD_NONE, GPIO10 );
  gpio_mode_setup ( GPIOE, GPIO_MODE_INPUT, GPIO_PUPD_NONE, GPIO11 );
  gpio_mode_setup ( GPIOE, GPIO_MODE_INPUT, GPIO_PUPD_NONE, GPIO12 );
  gpio_mode_setup ( GPIOE, GPIO_MODE_INPUT, GPIO_PUPD_NONE, GPIO13 );
  gpio_mode_setup ( GPIOE, GPIO_MODE_INPUT, GPIO_PUPD_NONE, GPIO14 );
  gpio_mode_setup ( GPIOE, GPIO_MODE_INPUT, GPIO_PUPD_NONE, GPIO15 );
  gpio_mode_setup ( GPIOD, GPIO_MODE_INPUT, GPIO_PUPD_NONE, GPIO8 );
  gpio_mode_setup ( GPIOD, GPIO_MODE_INPUT, GPIO_PUPD_NONE, GPIO9 );
  gpio_mode_setup ( GPIOD, GPIO_MODE_INPUT, GPIO_PUPD_NONE, GPIO10 );
  gpio_mode_setup ( GPIOD, GPIO_MODE_INPUT, GPIO_PUPD_NONE, GPIO11 );
  gpio_mode_setup ( GPIOD, GPIO_MODE_INPUT, GPIO_PUPD_NONE, GPIO12 );
  gpio_mode_setup ( GPIOD, GPIO_MODE_INPUT, GPIO_PUPD_NONE, GPIO13 );

}

uint8_t bus_perform_read ( uint32_t address ) {

  gpio_set ( GPIOE, address & 0xFFFF );        // address low-16
  gpio_set ( GPIOD, ( address & 0xFF0000 ) ); // address top

  gpio_set ( GPIOB, GPIO0 );                   // /OE

  unsigned char i;
  for ( i = 0; i < 10; i++ ) {
    __asm__("nop");
  }

  return ( gpio_port_read ( GPIOD ) & 0xFF );
}
