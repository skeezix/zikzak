// STM32F4 DMA GPIO OUT - sourcer32@gmail.com
 
// Per RM0090 DMA2, Channel7, Stream1 correlates to TIM8_UP source
// 2 KHz clock generated on TIM8_UP (Update - Timebase Period)
// PC0 conflicts, adapt as required
// Should be able to measure 1 KHz on PC5 with 0x00/0xFF data pattern
 
//#include "stm32f4_discovery.h"
#include "stm32f4xx.h"
 
#define BufferSize 1000 // Samples at 2 KHz, loops at 2 Hz
 
uint8_t GPIO_DATA[BufferSize];
 
/*******************************************************************************
 * Function Name  : GPIOC Configuration
 * Description    : GPIO PORT configurations in such a way that so it status can be read
 * Input          : None
 * Output         : None
 * Return         : None
 *******************************************************************************/
void GPIOC_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
 
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE); /* GPIOC clock enable */
 
  GPIO_InitStructure.GPIO_Pin = 0xFF; // Pin0 .. Pin7
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
}
 
/*******************************************************************************
 * Function Name  : Timer8 configurations
 * Description    : Configure Timer8 in such a way that it can initiate data transfer using DMA
 * Input          : None
 * Output         : None
 * Return         : None
 *******************************************************************************/
void TIM8_Configuration(void)
{
#define TIM8_PERIOD      (500-1)             /* Timer 8 PWM period 2 KHz */
  //#define TIM8_PRESCALER   (168-1)             /* Timer 8 prescaler  1 MHz */
#define TIM8_PRESCALER   (16-1)             /* Timer 8 prescaler  1 MHz */
 
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
 
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);  /* TIM8 clock enable */
 
  TIM_TimeBaseStructure.TIM_Period        = TIM8_PERIOD;
  TIM_TimeBaseStructure.TIM_Prescaler     = TIM8_PRESCALER;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode   = TIM_CounterMode_Up;
 
  TIM_TimeBaseInit(TIM8, &TIM_TimeBaseStructure);  /* Time base configuration */
  TIM_ARRPreloadConfig(TIM8, ENABLE);              /* Enable the time autoreload register */
  TIM_Cmd(TIM8, ENABLE);                           /* TIM enable counter */
  TIM_DMACmd(TIM8, TIM_DMA_Update, ENABLE);        /* Enable TIM8_UP DMA Requests */
}
 
/*******************************************************************************
 * Function Name  : DMA2 configuration
 * Description    : Transfer Data to peripheral port (GPIOC) from RAM buffer
 * Input          : None
 * Output         : None
 * Return         : None
 *******************************************************************************/
void DMA2_Configuration(void)
{
  DMA_InitTypeDef  DMA_InitStructure;
 
  /* Enable the DMA clock */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);
 
  /* Configure the DMA Stream */
  DMA_Cmd(DMA2_Stream1, DISABLE);
  DMA_DeInit(DMA2_Stream1);
 
  /* Set the parameters to be configured */
  DMA_InitStructure.DMA_Channel = DMA_Channel_7;
  DMA_InitStructure.DMA_Memory0BaseAddr    = (uint32_t)&GPIO_DATA[0]; /* Read the data from the RAM buffer */
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&GPIOC->ODR;   /* Send GPIO output data register */
  DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;
  DMA_InitStructure.DMA_BufferSize = BufferSize;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
 
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Enable;
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
  DMA_Init(DMA2_Stream1, &DMA_InitStructure);
 
  /* Enable DMA Transfer Complete interrupt */
  DMA_ITConfig(DMA2_Stream1, DMA_IT_TC, ENABLE);
 
  DMA_Cmd(DMA2_Stream1, ENABLE);
}
 
/**************************************************************************************/
 
void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
 
  /* Enable the DMA Stream IRQ Channel */
  NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}
 
/**************************************************************************************/
 
void dma2_stream1_isr (void) // 2 Hz
//void DMA2_Stream1_IRQHandler(void) // 2 Hz
{
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
 
/**************************************************************************************/
 
/**
 * @brief  Main program
 * @param  None
 * @retval None
 */
int main(void)
{
  int i;
  for(i=0;i<BufferSize; i++)
    {
      if((i % 4) == 0)
        GPIO_DATA[i] = 0;
      else
        GPIO_DATA[i] = 0xFF;
    }
  GPIOC_Configuration();
  NVIC_Configuration();
  DMA2_Configuration();
  TIM8_Configuration();
 
  //STM_EVAL_LEDInit(LED3); /* Configure LEDs to monitor program status */
 
  //STM_EVAL_LEDOn(LED3); /* Turn LED3 on, 1 Hz means it working */
 
  while(1); // Don't want to exit
}
