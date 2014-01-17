#include "stm32f2xx.h"
#include "stm32f2xx_dma.h"
#include "stm32f2xx_rcc.h"
#include "stm32f2xx_gpio.h"
#include "misc.h"

#include "dma_memcpy.h"

#define NULL 0

void dma_setup ( void ) {

  /* Enable the DMA clock */
  RCC_AHB1PeriphClockCmd ( RCC_AHB1Periph_DMA2, ENABLE );

  // enable nvic clock for dma
  //
#if 1
  NVIC_InitTypeDef NVIC_InitStructure;
 
  /* Enable the DMA Stream IRQ Channel */
  NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream6_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
#endif

}

#define DMA_STREAM DMA2_Stream0
#define DMA_CHANNEL DMA2_Channel_0

void dma_memcpy ( unsigned char *p, unsigned int len ) {

  // NOTE: Memory to memory seems like it uses Periph->Memory ... opposite of what you'd expect

  // set up memory to gpio
  //
  DMA_InitTypeDef  DMA_InitStructure;

  /* Configure the DMA Stream */
  DMA_Cmd ( DMA2_Stream6, DISABLE );
  DMA_DeInit ( DMA2_Stream6 );
 
  /* Set the parameters to be configured */
  DMA_InitStructure.DMA_Channel = DMA_Channel_0;

  //DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&GPIOC->ODR);  /* GPIO data register IDR input, ODR output */
  //DMA_InitStructure.DMA_Memory0BaseAddr    = (uint32_t)(p);          /* RAM buffer */
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(p);  /* GPIO data register IDR input, ODR output */
  DMA_InitStructure.DMA_Memory0BaseAddr    = (uint32_t)(&GPIOC->ODR);          /* RAM buffer */

  DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToMemory; //DMA_DIR_MemoryToPeripheral;
  DMA_InitStructure.DMA_BufferSize = len;

  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Enable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable;
  //DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Enable;
  //DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable;

  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal; //DMA_Mode_Circular;
 
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Enable;
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;

  DMA_Init(DMA2_Stream6, &DMA_InitStructure);
 
  /* Enable DMA Transfer Complete interrupt */
  //DMA_ITConfig ( DMA2_Stream6, DMA_IT_TC, ENABLE );
  DMA_ITConfig ( DMA2_Stream6, DMA_IT_TC, ENABLE ); // no interupts needed
 
  DMA_Cmd ( DMA2_Stream6, ENABLE );

}

void dma2_stream6_isr /*DMA2_Stream6_IRQHandler*/ (void) // 2 Hz
{
  GPIO_ToggleBits ( GPIOC, 1<<3 );

  /* Test on DMA Stream Transfer Complete interrupt */
  if (DMA_GetITStatus(DMA2_Stream6, DMA_IT_TCIF6)) {

    /* Clear DMA Stream Transfer Complete interrupt pending bit */
    DMA_ClearITPendingBit(DMA2_Stream6, DMA_IT_TCIF6);
 
    /* Toggle LED3 : End of Transfer */
    //GPIO_ToggleBits ( GPIOC, 1<<3 );
 
    // Add code here to process things
  }

}
