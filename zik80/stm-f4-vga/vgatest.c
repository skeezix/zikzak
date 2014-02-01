
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/timer.h>

#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/exti.h>

#include <libopencm3/cm3/cortex.h>

#include "dma_memcpy.h"
#include "framebuffer.h"
#include "gpio.h"
#include "pixelclock.h"

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

// vsync
unsigned int back_porch_togo = 0;  // remaining lines of back porch
unsigned int front_porch_togo = 0; // remaining lines of front porch
unsigned int vsync_togo = 0;       // remaining lines of vsync
// etc
volatile unsigned char vblank = 0;          // changed to 1 when end of screen starts
unsigned int line_count = 0;       // how many lines done so far this page
//#define VISIBLE_ROWS 600
#define VISIBLE_ROWS 480

static void gpio_setup ( void ) {

  /* Enable GPIOC clock. */
  rcc_peripheral_enable_clock ( &RCC_AHB1ENR, RCC_AHB1ENR_IOPBEN );
  rcc_peripheral_enable_clock ( &RCC_AHB1ENR, RCC_AHB1ENR_IOPCEN );

  // pixel clock?
  rcc_peripheral_enable_clock ( &RCC_AHB1ENR, RCC_AHB1ENR_IOPBEN );

  /* Blinky LED: Set GPIO3 to 'output push-pull'. */
  rcc_peripheral_enable_clock ( &RCC_AHB1ENR, RCC_AHB1ENR_IOPGEN );
  gpio_mode_setup ( GPIOG, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO13 );

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
  gpio_set_blinkenlight ( 1 ); // LED
  gpio_set ( GPIOB, GPIO10 ); // vsync
  gpio_set ( GPIOB, GPIO11 ); // hsync
  gpio_clear ( GPIOC, GPIO0 | GPIO1 ); // red
  gpio_clear ( GPIOC, GPIO2 | GPIO3 ); // green
  gpio_clear ( GPIOC, GPIO4 | GPIO5 ); // blue

}

static inline void vsync_go_high ( void ) {
  gpio_set ( GPIOB, GPIO10 );
}

static inline void vsync_go_low ( void ) {
  gpio_clear ( GPIOB, GPIO10 );
}

static inline void hsync_go_high ( void ) {
  gpio_set ( GPIOB, GPIO11 );
}

static inline void hsync_go_low ( void ) {
  gpio_clear ( GPIOB, GPIO11 );
}

static inline void rgb_go_level ( unsigned int rgb ) {
  //gpio_set ( GPIOC, rgb );
  gpio_port_write ( GPIOC, rgb );
}



static void nvic_setup(void) {
  /* Without this the timer interrupt routine will never be called. */
  nvic_enable_irq(NVIC_TIM2_IRQ);
  //nvic_set_priority(NVIC_TIM2_IRQ, 1);
}


static void timer2_setup ( void ) {

  //timer_reset ( TIM2 );
  //timer_set_mode
  //timer_continuous_mode ( TIM2 );

  /* Set timer start value. */
  TIM_CNT(TIM2) = 1;

  /* Set timer prescaler. 72MHz/1440 => 50000 counts per second. */
  TIM_PSC(TIM2) = 541; // 120M/2000 = 60k/second   ## 540

  /* End timer value. If this is reached an interrupt is generated. */
  TIM_ARR(TIM2) = 2; // ## 2

  // o-scope reports:
  // prescale 2000, 1->600 should be 100/sec; in fact, we're exactly 20ms between which is exactly 50 .. so callback is 60MHz, not 120MHz
  // manual says:
  //  The reference manual (see page 133) states that the GPIO is capable of:
  //  Fast toggle capable of changing every two clock cycles
  // --> okay so at 120MHz, the best we can do is 60MHz of GPIO. But thats different than here, where the timer seems halved..


  /* Update interrupt enable. */
  TIM_DIER(TIM2) |= TIM_DIER_UIE;

  //timer_set_repetition_counter ( TIM2, 100 );

  /* Start timer. */
  TIM_CR1(TIM2) |= TIM_CR1_CEN;

  return;
}

unsigned int i = 0;
unsigned char done_sync;
void tim2_isr ( void ) {

  //TIM2_SR &= ~TIM_SR_UIF;    //clearing update interrupt flag
  TIM_SR(TIM2) &= ~TIM_SR_UIF; /* Clear interrrupt flag. */

  /* ISR HAS TO DO SOMRTHING A LITTLE HEAVY
  // https://my.st.com/public/STe2ecommunities/mcu/Lists/cortex_mx_stm32/Flat.aspx?RootFolder=/public/STe2ecommunities/mcu/Lists/cortex_mx_stm32/Problem%20with%20DMA-USART%20Rx%20on%20STM32F407VG&FolderCTID=0x01200200770978C69A1141439FE559EB459D7580009C4E14902C3CDE46A77F0FFD06506F5B&currentviews=148
  // ie: after setting SR, you need to pause read/writes
  // ie: if the ISR is too quick, there may be a spurious re-invocation (tail chain), so you need to do a little something to avoid cpu race condition
  // -- another way to do it, is to do a SR read (which blocks until its 'set' finishes), thus stalling until the ISR is 'done':
  //void SPI2_IRQHandler(void)                                                                                                                                                              {                                                                                                                                                                                         volatile unsigned int dummy;                                                                                                                                                         ... some code...                                                                                                                                                                              SPI2_CR2 &= ~SPI_CR2_RXNEIE;  // Turn off RXE interrupt enable                                                                                                                    ...some code...                                                                                                                                                                              dummy = SPI2_SR; // Prevent tail-chaining.                                                                                                                                            return;                                                                                                                                                                        }         
  */
#if 0
  __asm__("nop");
  gpio_toggle(GPIOB, GPIO12); /* LED on/off. */
#endif


  // VGA line logic
  //
  // line1
  // line2
  // ..
  // line600
  // front porch blank            1 line
  // vsync pulse                  4 lines
  // back porch blank             23 lines
  // \__> back to top
  //

  // handle vblank stuff
  // - front porch, leads to
  //  - vsync, leads to
  //   - back porch
  // vsync is normally HIGH, but goes to LOW during pulse
  done_sync = 0;
  if ( front_porch_togo ) {
    vblank = 1;
    front_porch_togo --;

    if ( ! front_porch_togo ) { // on exit front porch, start vsync pulse
      vsync_go_low();
      vsync_togo = 4;
    }

    done_sync = 1;
    goto hsync;
    //return; // do nothing..
  }

  if ( vsync_togo ) {
    vsync_togo --;

    if ( ! vsync_togo ) { // on exit vsync pulse, start back porch
      vsync_go_high();
      back_porch_togo = 23;
    }

    done_sync = 1;
    goto hsync;
    //return;
  }

  if ( back_porch_togo ) {
    back_porch_togo --;

    if ( ! back_porch_togo ) {
      line_count = 1;
      vblank = 0;
    }

    done_sync = 1;
    goto hsync;
    //return; // do nothing..
  }

 hsync:
  // hsync period..
  // should use timer/interupt to 'end the line' and go hsync?
  //

  /* 1uS Front Porch */
  /* 1uS */
  i = 22;
  while ( i-- ) {
    __asm__("nop");
  }

  /* 3.2uS Horizontal Sync */
  hsync_go_low();
  i = 60;
  while ( i-- ) {
    __asm__("nop");
  }

  /* 2.2uS Back Porch */
  hsync_go_high();
  i = 45;
  while ( i-- ) {
    __asm__("nop");
  }

  if ( done_sync ) {
    return;
  }




  // actual line data
  //
  // line data on/off/on/off..
  // off for hsync/porch business!

#if 1 // pull from array
  //i = line_count % FBHEIGHT;
  i = line_count/2;
  unsigned char *p = framebuffer + ( i * FBWIDTH ); // 240
  //p = framebuffer + ( (line_count%240) * 240 );

#if 1
  dma_memcpy ( p, &(GPIO_ODR(GPIOC)) /*&GPIOC->ODR*/, 320 );
#else
  i = 320 / 20 / 4;
  while ( i-- ) {

    rgb_go_level ( (*p++) );
    rgb_go_level ( (*p++) );
    rgb_go_level ( (*p++) );
    rgb_go_level ( (*p++) );
    rgb_go_level ( (*p++) );
    rgb_go_level ( (*p++) );
    rgb_go_level ( (*p++) );
    rgb_go_level ( (*p++) );
    rgb_go_level ( (*p++) );
    rgb_go_level ( (*p++) );

    rgb_go_level ( (*p++) );
    rgb_go_level ( (*p++) );
    rgb_go_level ( (*p++) );
    rgb_go_level ( (*p++) );
    rgb_go_level ( (*p++) );
    rgb_go_level ( (*p++) );
    rgb_go_level ( (*p++) );
    rgb_go_level ( (*p++) );
    rgb_go_level ( (*p++) );
    rgb_go_level ( (*p++) );

    //gpio_set ( GPIOC, *p++ );
    //GPIO_BSRR(GPIOC) = *p++;
    //GPIO_BSRR(GPIOC) = 1<<6;
  }

  // disable all colour pins (dma does it itself in its isr)
  //GPIO_BSRR(GPIOC) = 0x00;

  gpio_clear ( GPIOC, GPIO0 | GPIO1 | GPIO2 | GPIO3 | GPIO4 | GPIO5 );

#endif

#endif

  // entering vblank period?
  //

  if ( line_count > VISIBLE_ROWS ) {
    front_porch_togo = 1;
    return; // entering front porch
  }

  line_count++;
}

int main ( void ) {

#if 1 // go for 120MHz, built into libopencm3
  // requires: external 8MHz crystal on pin5/6 with associated caps to ground
  rcc_clock_setup_hse_3v3 ( &hse_8mhz_3v3 [ CLOCK_3V3_168MHZ ] );
  //rcc_clock_setup_hse_3v3 ( &hse_8mhz_3v3 [ CLOCK_3V3_120MHZ ] );
#endif

  fb_setup();
  fb_test_pattern ( framebuffer );
  fb_clone ( framebuffer, offscreen );

  gpio_setup();

  /* Enable TIM2 clock. */
  rcc_peripheral_enable_clock(&RCC_APB1ENR, RCC_APB1ENR_TIM2EN);

  //__enable_irq();
  //cm_enable_interrupts();

  nvic_setup();

  timer2_setup();

  dma_setup();

  pixelclock_setup();

  //torture_setup();

  //while(1);

  uint16_t iter;

  while ( 1 ) {

    if ( vblank == 1 ) {
      vblank = 2;

      //gpio_toggle_blinkenlight();

      fb_lame_demo_animate ( offscreen );

      //gpio_toggle_blinkenlight();

#if 1
      for ( iter = 0; iter < 1500; iter++ ) { // if 100, no distortion.. finishes during vblank; if 1500 it bleeds past vblank so shows a turd-line
        fb_lame_demo_animate ( offscreen );
      }
#endif

      //gpio_toggle_blinkenlight();

      fb_clone ( offscreen, framebuffer );

      //gpio_toggle_blinkenlight();

    }

    __asm__("nop");

  } // while forever

  return 0;
}
