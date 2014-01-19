
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/timer.h>

#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/exti.h>

#include <libopencm3/cm3/cortex.h>

#include "dma_memcpy.h"

//#include <stm32f2xx_rcc.h>

// VGA
//
// 800x600 60hz, see timings:
// http://tinyvga.com/vga-timing/800x600@60Hz
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
// - a line is 26.4uS in total
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
// LED   PB12 -> pin 33
// vsync PB10 -> pin 29
// hsync PB11 -> pin 30
// red   PC0 -> pin 8
// green PC1 -> pin 9
// blue  PC2 -> pin 10

//
// at 120MHz..
// 1 cycle is 0.000000008 or 8ns
// line = 15840uS or 15840000ns means 15840000/8 = 1,980,000cycles per line
// lines per second is 624*60 == 37440

// So we need a line counter, so we know when to cut into vsync and so on
// .. keeping a line counter in code is easy enough, but may cost too much time
// .. may be better to have a vsync interupt that resets counter..

// vblank
volatile unsigned int back_porch_togo = 0;  // remaining lines of back porch
volatile unsigned int front_porch_togo = 0; // remaining lines of front porch
volatile unsigned int vsync_togo = 0;       // remaining lines of vsync
// etc
volatile unsigned int line_count = 0;      // how many lines done so far this page
#define VISIBLE_ROWS 640


static void gpio_setup ( void ) {

  /* Enable GPIOC clock. */
  rcc_peripheral_enable_clock ( &RCC_AHB1ENR, RCC_AHB1ENR_IOPBEN );
  rcc_peripheral_enable_clock ( &RCC_AHB1ENR, RCC_AHB1ENR_IOPCEN );

  /* Blinky LED: Set GPIO3 to 'output push-pull'. */
  gpio_mode_setup ( GPIOB, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO12 );

  // VGA
  //
  // sync lines
  gpio_mode_setup ( GPIOB, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO10 ); // vsync
  gpio_mode_setup ( GPIOB, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO11 ); // hsync
  // colour
  gpio_mode_setup ( GPIOC, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO0 ); // red pin37
  gpio_mode_setup ( GPIOC, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO1 ); // green pin38
  gpio_mode_setup ( GPIOC, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO2 ); // blue pin39
  // speed
  gpio_set_output_options ( GPIOB, GPIO_OTYPE_PP, GPIO_OSPEED_100MHZ, GPIO10 | GPIO11 );
  gpio_set_output_options ( GPIOC, GPIO_OTYPE_PP, GPIO_OSPEED_100MHZ, GPIO0 | GPIO1 | GPIO2 );

  // reset
  //
  gpio_set ( GPIOB, GPIO12 ); // LED
  gpio_set ( GPIOB, GPIO10 );
  gpio_set ( GPIOB, GPIO11 );
  gpio_clear ( GPIOC, GPIO0 );
  gpio_clear ( GPIOC, GPIO1 );
  gpio_clear ( GPIOC, GPIO2 );

}

static void vsync_go_high ( void ) {
  gpio_set ( GPIOB, GPIO10 );
}

static void vsync_go_low ( void ) {
  gpio_clear ( GPIOB, GPIO10 );
}

static void hsync_go_high ( void ) {
  gpio_set ( GPIOB, GPIO11 );
}

static void hsync_go_low ( void ) {
  gpio_clear ( GPIOB, GPIO11 );
}

static inline void red_go_level ( unsigned char intensity ) {

  // for now, just on/off :)
  if ( intensity ) {
    gpio_set ( GPIOC, GPIO0 );
  } else {
    gpio_clear ( GPIOC, GPIO0 );
  }

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

volatile unsigned int some_toggle = 0;

#define FBWIDTH 320 /* 256 */
#define FBHEIGHT 200 /* 256 */
volatile unsigned char framebuffer [ FBWIDTH * FBHEIGHT ] /*__attribute((aligned (1024)))*/;
volatile unsigned int i = 0;
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
  if ( front_porch_togo ) {
    front_porch_togo --;

    if ( ! front_porch_togo ) { // on exit front porch, start vsync pulse
      vsync_go_low();
      vsync_togo = 4;
    }

    return; // do nothing..
  }

  if ( vsync_togo ) {
    vsync_togo --;

    if ( ! vsync_togo ) { // on exit vsync pulse, start back porch
      vsync_go_high();
      back_porch_togo = 23;
    }

    return;
  }

  if ( back_porch_togo ) {
    back_porch_togo --;

    if ( ! back_porch_togo ) {
      line_count = 1;
    }

    return; // do nothing..
  }


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

#if 1
  i = (line_count/3);
  unsigned char *p = framebuffer + ( i * FBWIDTH ); // 240
  dma_memcpy ( p, 320 );
#endif

  /* 2.2uS Back Porch */
  hsync_go_high();
  i = 45;
  while ( i-- ) {
    __asm__("nop");
  }




  // actual line data
  //
  // line data on/off/on/off..
  // off for hsync/porch business!

#if 0 // pull from array
  i = line_count % FBHEIGHT;
  unsigned char *p = framebuffer + ( i * FBWIDTH ); // 240
  //p = framebuffer + ( (line_count%240) * 240 );
  unsigned int px;

#if 0
  dma_memcpy ( p, 352 );
#endif
#if 0
  i = 60; // 120
  while ( i-- ) {
    //red_go_level ( *p++ );
    rgb_go_level ( (*p++) );

    //gpio_set ( GPIOC, *p++ );
    //GPIO_BSRR(GPIOC) = *p++;
    //GPIO_BSRR(GPIOC) = 1<<6;
  }
#endif
#if 0
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
#endif

  gpio_clear ( GPIOC, GPIO0 | GPIO1 | GPIO2 );
  //GPIO_BSRR(GPIOC) = 0x00;
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
  rcc_clock_setup_hse_3v3 ( &hse_8mhz_3v3 [ CLOCK_3V3_120MHZ ] );
#endif

#if 1 // fill framebuffer
  //unsigned char i;
  unsigned int x, y;
  unsigned char v;
  for ( y = 0; y < FBHEIGHT; y++ ) {

    i = 0;

    i = ( y / 10 ) % 2;

    for ( x = 0; x < FBWIDTH; x++ ) {

      if ( x % 10 == 0 ) {
        i++;
        if ( i == 3 ) {
          i = 0;
        }
      }

      if ( i == 0 ) {
        v = (unsigned char) GPIO0;
      } else if ( i == 1 ) {
        v = (unsigned char) GPIO1;
      } else if ( i == 2 ) {
        v = (unsigned char) GPIO2;
      }

      *( framebuffer + ( y * FBWIDTH ) + x ) = v;
      //*( framebuffer + ( y * FBWIDTH ) + x ) = (unsigned char)GPIO2;

    } // x

  } // y
#endif

  gpio_setup();

  /* Enable TIM2 clock. */
  rcc_peripheral_enable_clock(&RCC_APB1ENR, RCC_APB1ENR_TIM2EN);

  //__enable_irq();
  //cm_enable_interrupts();

  nvic_setup();

  timer2_setup();

  dma_setup();

  TIM_SR(TIM2) &= ~TIM_SR_UIF; /* Clear interrrupt flag. */

  while ( 1 ) {
    __asm__("nop");
  } // while forever

  return 0;
}