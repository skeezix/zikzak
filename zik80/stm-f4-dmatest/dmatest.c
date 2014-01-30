#include "stm32f4xx.h"
#include "stm32f4xx_dma.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_tim.h"
#include "misc.h"

#define NULL 0

#define DMA_STREAM DMA2_Stream1
#define DMA_CHANNEL DMA_Channel_7
#define DMA_STREAM_CLOCK         RCC_AHB1Periph_DMA2 
#define DMA_STREAM_IRQ           DMA2_Stream1_IRQn
#define DMA_IT_TCIF              DMA_IT_TCIF0
#define DMA_ISR_FUNC             dma2_stream1_isr

#define BUFSIZE 10
volatile uint32_t src [ BUFSIZE ] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
volatile uint32_t dst [ BUFSIZE ] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

int main ( void ) {

  // enable nvic clock for dma
  //
#if 1
  NVIC_InitTypeDef NVIC_InitStructure;
 
  /* Enable the DMA Stream IRQ Channel */
  NVIC_InitStructure.NVIC_IRQChannel = DMA_STREAM_IRQ;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
#endif

  // set us up the bomb
  //

  DMA_InitTypeDef  DMA_InitStructure __attribute((aligned (4)));

  /* Enable the DMA clock */
  RCC_AHB1PeriphClockCmd ( DMA_STREAM_CLOCK, ENABLE );

  /* Configure the DMA Stream */
  //DMA_Cmd ( DMA_STREAM, DISABLE );
  DMA_DeInit ( DMA_STREAM );
  while(DMA_GetCmdStatus(DMA_STREAM) != DISABLE);

  /* Set the parameters to be configured */
  DMA_InitStructure.DMA_Channel = DMA_CHANNEL;

  // GPIO data register IDR input, ODR output
  DMA_InitStructure.DMA_BufferSize = (uint32_t) (BUFSIZE);

#if 1 // M2M

  DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToMemory;
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)src;
  DMA_InitStructure.DMA_Memory0BaseAddr    = (uint32_t)dst;

  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Enable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;

#else
#endif


  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal; //DMA_Mode_Circular;
 
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full; //Full 1QuarterFull
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single; // Single
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single; // Single INC4 INC8 INC16

  DMA_Init(DMA_STREAM, &DMA_InitStructure);
 
  //DMA2_Stream1->CR &= ~DMA_SxCR_EN;
  //pixelclock_start();
  //DMA2_Stream1->CR|=DMA_SxCR_EN;

  /* Enable DMA Transfer Complete interrupt */
  DMA_ITConfig ( DMA_STREAM, DMA_IT_TC, ENABLE ); // interupts needed?
 
  DMA_Cmd ( DMA_STREAM, ENABLE );

  //while(DMA_GetCmdStatus(DMA_STREAM) != ENABLE);

  RCC-> APB2ENR |= RCC_APB2ENR_TIM1EN;

  TIM1->PSC = 0;  // clk is 16MHz, no prescaler
  TIM1->ARR = 100;   // -> the whole 10-beat DMA transfer takes cca 1000 clk
  TIM1->DIER = TIM_DIER_UDE | TIM_DIER_UIE;   /* Update DMA enable */
  TIM1->CR1 = TIM_CR1_CEN;   /* Counter enable */

  while (!(DMA2->LISR & DMA_LISR_TCIF1));    // wait until DMA transfer finishes



  // wait forever
  while ( 1 ) {
    __asm__("nop"); // main spin
  } // while forever

  return 0;
}

void DMA_ISR_FUNC (void) {

  // Clear DMA Stream Transfer Complete interrupt pending bit
  DMA_ClearITPendingBit(DMA_STREAM, DMA_IT_TCIF);

  // block so we don't tail-call the ISR
  unsigned int i;
  for ( i = 0; i < 32; i++ ) {
    __asm__("nop"); // isr waste
  }

}
