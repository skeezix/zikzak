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
  gpio_mode_setup ( GPIOD, GPIO_MODE_INPUT, GPIO_PUPD_PULLDOWN,
                    GPIO0 | GPIO1 | GPIO2 | GPIO3 | GPIO4 | GPIO5 | GPIO6 | GPIO7 );

  // control bus, address bus, and release
  gpio_mode_setup ( GPIOB, GPIO_MODE_INPUT, GPIO_PUPD_NONE, GPIO5 ); // MREQ
  gpio_mode_setup ( GPIOB, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO8 ); // BUSREQ
  gpio_mode_setup ( GPIOB, GPIO_MODE_INPUT, GPIO_PUPD_NONE, GPIO9 ); // BUSACK
  gpio_mode_setup ( GPIOB, GPIO_MODE_INPUT, GPIO_PUPD_NONE, GPIO3 ); // /CS1
  bus_release();

  // should be all clear to survive
  //

}

void bus_grab_and_wait ( void ) {

  // send /BUSREQ low
  gpio_clear ( GPIOB, GPIO8 );

  // wait for /BUSACK
  uint16_t v = 0;
#if 1
  while ( gpio_get ( GPIOB, GPIO9 ) > v ) {
    __asm__("nop");
  }
#endif
  //uint16_t i;
  //for ( i = 0; i < 2000; i++ ) {
  //  __asm__("nop");
  //}

  // switch all the inputs around
  //

  // control bus
  gpio_mode_setup ( GPIOB, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLDOWN, GPIO0 ); // OE
  gpio_mode_setup ( GPIOB, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, GPIO1 ); // WE
  gpio_mode_setup ( GPIOB, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO5 ); // MREQ

  //gpio_mode_setup ( GPIOB, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLDOWN, GPIO3 ); // /CS1
  //gpio_clear ( GPIOB, GPIO3 );                   // /CS1

  //gpio_clear ( GPIOB, GPIO5 );                   // /MREQ
  gpio_set ( GPIOB, GPIO1 );                     // /WE
  //gpio_clear ( GPIOB, GPIO0 );                   // /OE

  // address bus
  gpio_mode_setup ( GPIOE, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE,
                    GPIO0 | GPIO1 | GPIO2 | GPIO3 | GPIO4 | GPIO5 | GPIO6 | GPIO7 |
                    GPIO8 | GPIO9 | GPIO10 | GPIO11 | GPIO12 | GPIO13 | GPIO14 | GPIO15 );
  gpio_mode_setup ( GPIOD, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE,
                    GPIO8 | GPIO9 | GPIO10 | GPIO11 | GPIO12 | GPIO13 );

  gpio_set_output_options ( GPIOE, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,
                            GPIO0 | GPIO1 | GPIO2 | GPIO3 | GPIO4 | GPIO5 | GPIO6 | GPIO7 |
                            GPIO8 | GPIO9 | GPIO10 | GPIO11 | GPIO12 | GPIO13 | GPIO14 | GPIO15 );
  gpio_set_output_options ( GPIOD, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,
                            GPIO8 | GPIO9 | GPIO10 | GPIO11 | GPIO12 | GPIO13 );

  gpio_set_output_options ( GPIOD, GPIO_OTYPE_PP, GPIO_OSPEED_100MHZ,
                            GPIO0 | GPIO1 | GPIO2 | GPIO3 | GPIO4 | GPIO5 | GPIO6 | GPIO7 );

  gpio_set_output_options ( GPIOB, GPIO_OTYPE_PP, GPIO_OSPEED_2MHZ,
                            GPIO0 | GPIO1 ); // OE WE

  gpio_clear ( GPIOE, GPIO0 | GPIO1 | GPIO2 | GPIO3 | GPIO4 | GPIO5 | GPIO6 | GPIO7 |
                      GPIO8 | GPIO9 | GPIO10 | GPIO11 | GPIO12 | GPIO13 | GPIO14 | GPIO15 );
  gpio_clear ( GPIOD, GPIO8 | GPIO9 | GPIO10 | GPIO11 | GPIO12 | GPIO13 );

}

void bus_release ( void ) {

  // release
  //gpio_set ( GPIOB, GPIO5 );                   // /MREQ
  gpio_set ( GPIOB, GPIO8 ); // /BUSREQ high (not active)
  //gpio_set ( GPIOB, GPIO0 );                   // /OE

  // control bus
  gpio_mode_setup ( GPIOB, GPIO_MODE_INPUT, GPIO_PUPD_NONE, GPIO0 ); // OE
  gpio_mode_setup ( GPIOB, GPIO_MODE_INPUT, GPIO_PUPD_PULLUP, GPIO1 ); // WE
  gpio_mode_setup ( GPIOB, GPIO_MODE_INPUT, GPIO_PUPD_NONE, GPIO5 ); // MREQ
  //gpio_mode_setup ( GPIOB, GPIO_MODE_INPUT, GPIO_PUPD_NONE, GPIO3 ); // /CS1

  // address bus
  gpio_mode_setup ( GPIOE, GPIO_MODE_INPUT, GPIO_PUPD_NONE,
                    GPIO0 | GPIO1 | GPIO2 | GPIO3 | GPIO4 | GPIO5 | GPIO6 | GPIO7 |
                    GPIO8 | GPIO9 | GPIO10 | GPIO11 | GPIO12 | GPIO13 | GPIO14 | GPIO15 );
  gpio_mode_setup ( GPIOD, GPIO_MODE_INPUT, GPIO_PUPD_NONE,
                    GPIO8 | GPIO9 | GPIO10 | GPIO11 | GPIO12 | GPIO13 );

#if 1
  uint16_t v = 0;
  while ( gpio_get ( GPIOB, GPIO9 ) == v ) {
    __asm__("nop");
  }
#endif

}

uint8_t bus_perform_read ( uint32_t address ) {
  uint8_t v;
  uint32_t a;
  uint16_t i;

  a = address & 0xFF0000;
  a >>= 8;

  GPIO_ODR(GPIOE) = address & 0xFFFF;
  GPIO_ODR(GPIOD) &= 0xFF;
  GPIO_ODR(GPIOD) |= a;

  gpio_clear ( GPIOB, GPIO5 );                   // /MREQ
  gpio_clear ( GPIOB, GPIO0 );                   // /OE

#if 1
  for ( i = 0; i < 5; i++ ) {
    __asm__("nop");
  }
#endif

  //v = ( gpio_port_read ( GPIOD ) & 0xFF );
  v =  GPIO_IDR ( GPIOD );

  gpio_set ( GPIOB, GPIO5 );                   // /MREQ
  gpio_set ( GPIOB, GPIO0 );                   // /OE

  return ( v );
}

uint8_t bus_check_cs1 ( void ) {
  return ( gpio_port_read ( GPIOB ) & GPIO3 );
}

void bus_read_block ( uint8_t *dest, uint32_t address, uint16_t len ) {
  uint8_t v;
  uint32_t i;

  bus_perform_read ( address ); // discard .. just getting /CS set

  for ( i = 0; i < len; i++ ) {

    v = bus_perform_read ( address++ );

    //if ( v > 0 ) {
    *dest++ = v;
    //}

  } // for

  return;
}
