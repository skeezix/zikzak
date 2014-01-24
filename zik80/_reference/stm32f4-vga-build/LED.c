#include "LED.h"
#include "GPIO.h"
#include "RCC.h"

void InitializeLEDs()
{
	EnableAHB1PeripheralClock(RCC_AHB1ENR_GPIODEN);

	SetGPIOOutputMode(GPIOG,(1<<12)|(1<<13)|(1<<14)|(1<<15));
	SetGPIOPushPullOutput(GPIOG,(1<<12)|(1<<13)|(1<<14)|(1<<15));
	SetGPIOSpeed50MHz(GPIOG,(1<<12)|(1<<13)|(1<<14)|(1<<15));
	SetGPIOPullUpResistor(GPIOG,(1<<12)|(1<<13)|(1<<14)|(1<<15));
}

