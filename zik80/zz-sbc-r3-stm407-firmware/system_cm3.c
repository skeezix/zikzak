
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/timer.h>

#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/exti.h>

#include <libopencm3/cm3/cortex.h>

#include "system_cm3.h"

void system_cm3_setup ( void ) {

#if 1 // go for 120MHz, built into libopencm3
  // requires: external 8MHz crystal on pin5/6 with associated caps to ground
  //rcc_clock_setup_hse_3v3 ( &hse_8mhz_3v3 [ CLOCK_3V3_168MHZ ] );
  //rcc_clock_setup_hse_3v3 ( &hse_8mhz_3v3 [ CLOCK_3V3_120MHZ ] );

  rcc_clock_setup_hse_3v3 ( &hse_16mhz_3v3 [ CLOCK_3V3_168MHZ ] );

#endif

}
