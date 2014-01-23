#include "LED.h"
#include "GPIO.h"
#include "RCC.h"

void InitializeLEDs()
{
	EnableAHB1PeripheralClock(RCC_AHB1ENR_GPIODEN);

	SetGPIOOutputMode(GPIOD,(1<<12)|(1<<13)|(1<<14)|(1<<15));
	SetGPIOPushPullOutput(GPIOD,(1<<12)|(1<<13)|(1<<14)|(1<<15));
	SetGPIOSpeed50MHz(GPIOD,(1<<12)|(1<<13)|(1<<14)|(1<<15));
	SetGPIOPullUpResistor(GPIOD,(1<<12)|(1<<13)|(1<<14)|(1<<15));
}

