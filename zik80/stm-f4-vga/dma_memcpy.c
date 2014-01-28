
#include "stm32f4xx.h"
#include "stm32f4xx_dma.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "misc.h"

#include "dma_memcpy.h"
#include "dma_memcpy_timer.h"

#define NULL 0

// DMA2 can do MemoryToMemory
// DMA Stream 0 worked, but DMA2-Stream1-Channel7 jas TIM8_UP as a optional trigger
// (see reference channel selection table.) This suggests if we set up TIM8 as the
// pixel-clock speed, we can control how fast the DMA doles out the bytes!

#define DMA_STREAM DMA2_Stream1
#define DMA_CHANNEL DMA_Channel_7
#define DMA_STREAM_CLOCK         RCC_AHB1Periph_DMA2 
#define DMA_STREAM_IRQ           DMA2_Stream1_IRQn
#define DMA_IT_TCIF              DMA_IT_TCIF0
#define DMA_ISR_FUNC             dma2_stream1_isr

DMA_InitTypeDef  DMA_InitStructure __attribute((aligned (4)));

void dma_setup ( void ) {

  /* Set the parameters to be configured */
  DMA_InitStructure.DMA_Channel = DMA_CHANNEL;

  // Periph is apparently the source (!!)
  // GPIO data register IDR input, ODR output
  //DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)src;
  DMA_InitStructure.DMA_Memory0BaseAddr    = (uint32_t)&GPIOC->ODR;

  DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToMemory; //DMA_DIR_MemoryToPeripheral;
  //DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;
  //DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;
  //DMA_InitStructure.DMA_BufferSize = (uint32_t) (len);

  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Enable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable;

  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal; //DMA_Mode_Circular;
 
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full; //Full 1QuarterFull
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single; // Single
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single; // Single INC4 INC8 INC16

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

  // pixelclock
  RCC->APB2ENR |= RCC_APB2ENR_TIM8EN;

}

void dma_memcpy ( unsigned char *src, unsigned int len ) {

  // NOTE: Memory to memory seems like it uses Periph->Memory ... opposite of what you'd expect

  /* Enable the DMA clock */
  RCC_AHB1PeriphClockCmd ( DMA_STREAM_CLOCK, ENABLE );

  // set up memory to gpio
  //

  /* Configure the DMA Stream */
  //DMA_Cmd ( DMA_STREAM, DISABLE );
  DMA_DeInit ( DMA_STREAM );
  //while(DMA_GetCmdStatus(DMA_STREAM) != DISABLE);

  // populate the struct.. did most of it above!
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)src;
  DMA_InitStructure.DMA_BufferSize = (uint32_t) (len);



  DMA_Init(DMA_STREAM, &DMA_InitStructure);
 


  DMA2_Stream1->CR &= ~DMA_SxCR_EN;

  pixelclock_start();

  DMA2_Stream1->CR|=DMA_SxCR_EN;


  /* Enable DMA Transfer Complete interrupt */
  DMA_ITConfig ( DMA_STREAM, DMA_IT_TC, ENABLE ); // interupts needed?
 
  DMA_Cmd ( DMA_STREAM, ENABLE );

  //while(DMA_GetCmdStatus(DMA_STREAM) != ENABLE);

}

void DMA_ISR_FUNC (void) {

  /* Clear DMA Stream Transfer Complete interrupt pending bit */
  //DMA_ClearITPendingBit(DMA_STREAM, DMA_IT_TCIF);

  //GPIO_ToggleBits ( GPIOB, 1<<12 );

#if 0 // full transfer
  if (DMA_GetITStatus(DMA_STREAM, DMA_IT_TCIF)) {
  }
#endif
#if 0 // half transfer
  if (DMA_GetITStatus(DMA_STREAM, DMA_IT_HTIF6)) {
  }
#endif
#if 0 // transfer error
  if (DMA_GetITStatus(DMA_STREAM, DMA_IT_TEIF6)) {
  }
#endif

  /* Test on DMA Stream Transfer Complete interrupt */
  //  if (DMA_GetITStatus(DMA_STREAM, DMA_IT_TCIF)) {

    /* Clear DMA Stream Transfer Complete interrupt pending bit */
    DMA_ClearITPendingBit(DMA_STREAM, DMA_IT_TCIF);

    //gpio_toggle_blinkenlight ( 1 ); // LED

    // ensure colour lines are down
    //GPIO_ResetBits ( GPIOC, 1<<0 | 1<<1 | 1<<2 );
    GPIO_ResetBits ( GPIOC, 1<<0 | 1<<1 | 1<<2 | 1<<3 | 1<<4 | 1<<5 );
 
    // Add code here to process things
    //}

  /* Clear DMA Stream Transfer Complete interrupt pending bit */
  //DMA_ClearITPendingBit(DMA_STREAM, DMA_IT_TCIF );
  //DMA_ClearITPendingBit(DMA_STREAM, DMA_IT_HTIF0 | DMA_IT_TEIF0 | DMA_IT_DMEIF0 | DMA_IT_FEIF0 );

}

void tim8_up_tim13_isr ( void ) {
  //gpio_toggle_blinkenlight ( 1 ); // LED
}
