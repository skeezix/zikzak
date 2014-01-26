
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/timer.h>

#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/exti.h>

#include <libopencm3/cm3/cortex.h>

#include "framebuffer.h"
#include "gpio.h"
#include "timers.h"

//#include <stm32f2xx_rcc.h>

// VGA
//
// 800x600 60hz, see timings:
// http://tinyvga.com/vga-timing/800x600@60Hz
//
// milli: .001          (m)
// micro: .000 001      (u)
// nano:  .000 000 001  (n)
//
// Remember that VGA is line-oriented.
//
// Each line is line-data plus a horizontal pulse at the end (or begin) of each line (not per pixel)
// Each page ends with a vertical pulse (expressed in lines of duration).
//
// ie: line/hpulse/line/hpulse/.../line/hpulse/vpulse/...repeat
// vpulse is front porch, pluse, back porch
//
// 800x600 mode:
// - a line is 26.4uS in total (800 pixels at 20uS, plus front porch 40->1, sync 128 -> 3.2, back porch 88->2.2)
//   --> a pixel is 20uS/800 = 0.000 000 025/px or 25ns/px
//   --> 120Mhz = 0.000 000 008 or 8ns/px
// - 600 lines * 26.4uS == 15840uS or 16mS
//   --> 1000ms / 16mS == 62.5fps
// theres actually 24 lines in the vblank period.. so 624 long.
//   --> 624 * 26.4uS = 16474uS or 16.5mS
//   --> 1000ms / 16.5mS == 60.5fps .. nice and close to 60!
//
// sync are normally high, and drop to gnd during pulse
// 5V and 3.3V are okay for sync levels!
//
// colour is 0v through 0.7v for each analog brightness level

// wiring:
// LED    PB12 -> pin 33
// vsync  PB10 -> pin 29
// hsync  PB11 -> pin 30
// red1   PC0 -> pin 8
// red2   PC1 -> pin 9
// green1 PC2 -> pin 10
// green2 PC3 -> pin 11
// blue1  PC4 -> pin 24
// blue2  PC5 -> pin 25

//
// at 120MHz..
// 1 cycle is 0.000000008 or 8ns
// line = 15840uS or 15840000ns means 15840000/8 = 1,980,000cycles per line
// lines per second is 624*60 == 37440

// So we need a line counter, so we know when to cut into vsync and so on
// .. keeping a line counter in code is easy enough, but may cost too much time
// .. may be better to have a vsync interupt that resets counter..


int main ( void ) {

  /* setup
   */

  // main system clock
  // REQ: external 8MHz crystal on pin5/6 with associated caps to ground
  rcc_clock_setup_hse_3v3 ( &hse_8mhz_3v3 [ CLOCK_3V3_168MHZ ] );
  //rcc_clock_setup_hse_3v3 ( &hse_8mhz_3v3 [ CLOCK_3V3_120MHZ ] );

  // framebuffer
  framebuffer_setup();

  // timers
  nvic_setup();
  timers_setup();

  // gpios
  gpio_setup();

  // vga
  vga_setup();

  //__enable_irq();
  //cm_enable_interrupts();

  gpio_set_blinkenlight ( 1 );

  TIM_SR(TIM2) &= ~TIM_SR_UIF; /* Clear interrrupt flag. */

  while ( 1 ) {
    __asm__("nop");
  } // while forever

  return ( 0 );
}
