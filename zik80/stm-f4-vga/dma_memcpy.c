
#include "stm32f4xx.h"
#include "stm32f4xx_dma.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "misc.h"

#include "dma_memcpy.h"
#include "pixelclock.h"

#define NULL 0

// DMA2 can do MemoryToMemory
// DMA Stream 0 worked, but DMA2-Stream1-Channel7 jas TIM8_UP as a optional trigger
// (see reference channel selection table.) This suggests if we set up TIM8 as the
// pixel-clock speed, we can control how fast the DMA doles out the bytes!

// 2-1-7 -- TIM_UP
// 2-0-7 -- blank
#define DMA_STREAM DMA2_Stream1
#define DMA_CHANNEL DMA_Channel_7
#define DMA_STREAM_CLOCK         RCC_AHB1Periph_DMA2 
#define DMA_STREAM_IRQ           DMA2_Stream1_IRQn
#define DMA_IT_TCIF              DMA_IT_TCIF0
#define DMA_ISR_FUNC             dma2_stream1_isr

DMA_InitTypeDef  DMA_InitStructure __attribute((aligned (4)));

void dma_setup ( void ) {

  /* Enable the DMA clock */
  RCC_AHB1PeriphClockCmd ( DMA_STREAM_CLOCK, ENABLE );

  /* Set the parameters to be configured */
  DMA_InitStructure.DMA_Channel = DMA_CHANNEL;

  // Periph is apparently the source (!!)
  // GPIO data register IDR input, ODR output
  //DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)src;

  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal; //DMA_Mode_Circular;
 
  DMA_InitStructure.DMA_Priority = DMA_Priority_Low;
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Enable;
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_3QuartersFull; //Full 1QuarterFull HalfFull 3QuartersFull
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single; // Single DMA_MemoryBurst_INC16
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

}

void dma_memcpy ( unsigned char *src, unsigned char *dst, unsigned int len ) {

  // NOTE: Memory to memory seems like it uses Periph->Memory ... opposite of what you'd expect

  // set up memory to gpio
  //

  /* Configure the DMA Stream */
  DMA_Cmd ( DMA_STREAM, DISABLE );
  DMA_DeInit ( DMA_STREAM );
  //while(DMA_GetCmdStatus(DMA_STREAM) != DISABLE);

  // DMA_DIR_MemoryToMemory
  // DMA_DIR_MemoryToPeripheral;
  // DMA_DIR_PeripheralToMemory;
#define MODE 3
#if MODE == 1 // M2M
  DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToMemory;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Enable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable;
  DMA_InitStructure.DMA_Memory0BaseAddr    = (uint32_t)dst;
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)src;
#elif MODE == 2 // P2M (mem as gpio)
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Enable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable;
  DMA_InitStructure.DMA_Memory0BaseAddr    = (uint32_t)dst;
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)src;
#else // M2P (periph as gpio)
  DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;
  DMA_InitStructure.DMA_Memory0BaseAddr    = (uint32_t)src;
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)dst;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
#endif

  DMA_InitStructure.DMA_BufferSize = (uint32_t) (len);


  DMA_Init(DMA_STREAM, &DMA_InitStructure);
 
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
#if 0
  //  if (DMA_GetITStatus(DMA_STREAM, DMA_IT_TCIF)) {

    /* Clear DMA Stream Transfer Complete interrupt pending bit */
    DMA_ClearITPendingBit(DMA_STREAM, DMA_IT_TCIF);
#endif

    //gpio_toggle_blinkenlight ( 1 ); // LED

    // ensure colour lines are down
#if 1
    GPIO_ResetBits ( GPIOC, 1<<0 | 1<<1 | 1<<2 | 1<<3 | 1<<4 | 1<<5 );
#endif
 
    // Add code here to process things
    //}

  /* Clear DMA Stream Transfer Complete interrupt pending bit */
  //DMA_ClearITPendingBit(DMA_STREAM, DMA_IT_TCIF );
  //DMA_ClearITPendingBit(DMA_STREAM, DMA_IT_HTIF0 | DMA_IT_TEIF0 | DMA_IT_DMEIF0 | DMA_IT_FEIF0 );

  /* Test on DMA Stream Transfer Complete interrupt */
  if (DMA_GetITStatus(DMA2_Stream1, DMA_IT_TCIF1))
    {
      /* Clear DMA Stream Transfer Complete interrupt pending bit */
      DMA_ClearITPendingBit(DMA2_Stream1, DMA_IT_TCIF1);
 
      /* Toggle LED3 : End of Transfer */
      //STM_EVAL_LEDToggle(LED3);
 
      // Add code here to process things
    }

}

void tim8_up_tim13_isr ( void ) {
  //gpio_toggle_blinkenlight ( 1 ); // LED
}
