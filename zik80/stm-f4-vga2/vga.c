
#include "stm32f4xx.h"
#include "core_cm4.h"

#include "timers.h"
#include "vga.h"

#define NULL 0

volatile uint32_t VGALine;
volatile uint32_t VGAFrame;

static HBlankInterruptFunction *HBlankInterruptHandler;

static inline int HandleVGAHSync480()
{
	// TIM2->SR=0;
	__asm__ volatile(
	"	mov.w	r1,#0x40000000\n"
	"	movs	r0,#0\n"
	"	strh	r0,[r1,#0x10]\n"
	:::"r0","r1");

	VGALine++;
	if(VGALine<480)
	{
		return VGALine;
	}
	else if(VGALine==480)
	{
		VGAFrame++;
	}
	else if(VGALine==490)
	{
		LowerVGAVSyncLine();
	}
	else if(VGALine==492)
	{
		RaiseVGAVSyncLine();
	}
	else if(VGALine==524)
	{
		VGALine=-1;
	}
	return -1;
}

static inline void WaitForLastLineIfVGAEnabled()
{
	if(!IsInterruptEnabled(TIM2_IRQn)) return;
	while(VGALine!=0xffffffff);
}

void vga_setup ( void ) {

  VGALine = -1;
  VGAFrame = 0;

  HBlankInterruptHandler = NULL;

  // Turn on peripherals.
  EnableAHB1PeripheralClock(RCC_AHB1ENR_GPIOBEN|RCC_AHB1ENR_GPIOEEN|RCC_AHB1ENR_DMA2EN);
  EnableAPB2PeripheralClock(RCC_APB2ENR_TIM8EN|RCC_APB2ENR_SYSCFGEN);
  EnableAPB1PeripheralClock(RCC_APB1ENR_TIM2EN);

  // set DAC to black
  GPIOE->BSRRH=0xff00;

  // Configure sync pins and drive them high.
  // Also link HSync-pin (PB11) to TIM2 CC-channel 4.
  SetGPIOAlternateFunctionMode(GPIOB,1<<11);
  SelectAlternateFunctionForGPIOPin(GPIOB,11,1); // TIM2_CH4
  SetGPIOOutputMode(GPIOB,1<<12);
  SetGPIOPushPullOutput(GPIOB,(1<<11)|(1<<12));
  SetGPIOSpeed50MHz(GPIOB,(1<<11)|(1<<12));
  SetGPIOPullUpResistor(GPIOB,(1<<11)|(1<<12));
  GPIOB->BSRRL=(1<<11)|(1<<12);

  // Configure timer 2 as the HSync timer. Timer 2 runs at half frequency, thus 84 MHz.
  // CC4 is used to generate the HSync pulse, using PWM mode and driving the pin directly.
  // CC3 is used to generate a trigger signal for TIM8, which drives the pixel DMA.
  TIM2->CR1=TIM_CR1_ARPE;
  TIM2->CR2=(6*TIM_CR2_MMS_0); // Trigger-out on CCR3.
  TIM2->DIER=TIM_DIER_UIE; // Enable update interrupt.
  TIM2->CCER=0; // Disable CC, so we can program it.
  TIM2->CCMR1=0;
  // PWM-mode: Channel 4 set to active level on reload, passive level after CC4-match.
  // Channel 3 set to passive level on reload, active level after CC3-match.
  TIM2->CCMR2=(6*TIM_CCMR2_OC4M_0)|(7*TIM_CCMR2_OC3M_0);
  TIM2->CCER=TIM_CCER_CC4E|TIM_CCER_CC4P; // Channel 4 enabled, reversed polarity (active low).
  TIM2->PSC=0; // Prescaler = 1

  // non overclocking
  TIM2->ARR=2669-1; // 84 MHz / 31.46875 kHz = 2669.31479643
  // On CNT==0: sync pulse start
  TIM2->CCR4=320; // 84 MHz * 3.813 microseconds = 320.292 - sync pulse end
  TIM2->CCR3=480; // 84 MHz * (3.813 + 1.907) microseconds = 480.48 - back porch end, start pixel clock
  TIM2->CCR2=480; // 84 MHz * (3.813 + 1.907) microseconds = 480.48 - back porch end, start pixel clock

  // Enable HSync timer.
  TIM2->CNT=-10; // Make sure it hits ARR. 
  TIM2->CR1|=TIM_CR1_CEN;

}

static void InitializePixelDMA(int pixelclock,int pixelsperrow)
{
	// Configure timer 8 as the pixel clock.
	TIM8->CR1=TIM_CR1_ARPE;
	TIM8->DIER=TIM_DIER_UDE; // Enable update DMA request.
	TIM8->PSC=0; // Prescaler = 1.
	TIM8->ARR=pixelclock-1;
	TIM8->SMCR=(5*TIM_SMCR_SMS_0)|(1*TIM_SMCR_TS_0); // Only run TIM8 when TIM2 trigger-out is high.

	// DMA2 stream 1 channel 7 is triggered by timer 8.
	// Stop it and configure interrupts.
	DMA2_Stream1->CR&=~DMA_SxCR_EN;
	InstallInterruptHandler(DMA2_Stream1_IRQn,DMACompleteHandler);
	EnableInterrupt(DMA2_Stream1_IRQn);
	SetInterruptPriority(DMA2_Stream1_IRQn,0);

	VGAPixelsPerRow=pixelsperrow;
}

static void DMACompleteHandler()
{
	SetVGASignalToBlack();
	DMA2->LIFCR|=DMA_LIFCR_CTCIF1; // Clear interrupt flag.
	StopPixelDMA();

	if(HBlankInterruptHandler) HBlankInterruptHandler();
	// TODO: VBlank interrupt? At lower priority?
}

static inline void StartPixelDMA()
{
	// Configure and enable pixel DMA.
	DMA2_Stream1->CR=(7*DMA_SxCR_CHSEL_0)| // Channel 7
	(3*DMA_SxCR_PL_0)| // Priority 3
	(0*DMA_SxCR_PSIZE_0)| // PSIZE = 8 bit
	(0*DMA_SxCR_MSIZE_0)| // MSIZE = 8 bit
	DMA_SxCR_MINC| // Increase memory address
	(1*DMA_SxCR_DIR_0)| // Memory to peripheral
	DMA_SxCR_TCIE| // Transfer complete interrupt
	(1*DMA_SxCR_MBURST_0); // burst on the memory-side
//	(2*DMA_SxCR_MSIZE_0) // perhaps 32-bit bursts on memory-side. Currently does not work.

	DMA2_Stream1->FCR=DMA_SxFCR_DMDIS| // Enable FIFO.
	(1*DMA_SxFCR_FTH_0); // Set threshold to 1/2. (TODO: look at this)

	DMA2_Stream1->NDTR=VGAPixelsPerRow;
	DMA2_Stream1->PAR=((uint32_t)&GPIOE->ODR)+1;
	DMA2_Stream1->M0AR=VGACurrentLineAddress;

	// Enable pixel clock. Clock will only start once TIM2 allows it.
	TIM8->DIER=0; // Update DMA request has to be disabled while zeroing the counter.
	TIM8->EGR=TIM_EGR_UG; // Force an update event to reset counter. Setting CNT is not reliable.
	TIM8->DIER=TIM_DIER_UDE; // Re-enable update DMA request.
	TIM8->CR1|=TIM_CR1_CEN;

	DMA2_Stream1->CR|=DMA_SxCR_EN;
}

static inline void StopPixelDMA()
{
	TIM8->CR1&=~TIM_CR1_CEN; // Stop pixel clock.
	DMA2_Stream1->CR=0; // Disable pixel DMA.
}
