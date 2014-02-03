#include "stm32f4xx.h"
#include "stm32f4xx_dma.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "misc.h"

#include "pixelclock.h"

void pixelclock_setup ( void ) {

  /* turn up the pixelclock timer
   * --> really should slave this off the hsync timer, or perhaps be a subchannel?
   */

//#define TIM8_PERIOD      (500-1)             /* Timer 8 PWM period 2 KHz */
#define TIM8_PERIOD      (5)             /* Timer 8 PWM period 2 KHz */

//#define TIM8_PRESCALER   (168-1)             /* Timer 8 prescaler  1 MHz */
#define TIM8_PRESCALER   (1-1)             /* Timer 8 prescaler  1 MHz */
 
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
 
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);  /* TIM8 clock enable */
 
  TIM_TimeBaseStructure.TIM_Period        = TIM8_PERIOD; /* ARR end counter to trigger interupt */
  TIM_TimeBaseStructure.TIM_Prescaler     = TIM8_PRESCALER;
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; // TIM_CKD_DIV1  TIM_CKD_DIV2 TIM_CKD_DIV14
  TIM_TimeBaseStructure.TIM_CounterMode   = TIM_CounterMode_Up;
 
  TIM_TimeBaseInit(TIM8, &TIM_TimeBaseStructure);  /* Time base configuration */
  TIM_ARRPreloadConfig(TIM8, ENABLE);              /* Enable the time autoreload register */
  TIM_Cmd(TIM8, ENABLE);                           /* TIM enable counter */
  TIM_DMACmd(TIM8, TIM_DMA_Update, ENABLE);        /* Enable TIM8_UP DMA Requests */

  // enable timer for pixel clock
  //nvic_enable_irq(NVIC_TIM8_UP_TIM13_IRQ);

}


void pixelclock_start ( void ) {

}

void pixelclock_stop ( void ) {

}
