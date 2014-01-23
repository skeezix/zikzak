#ifndef __LED_H__
#define __LED_H__

#include "stm32f4xx.h"

void InitializeLEDs();

static inline void SetLEDs(int leds)
{
	uint16_t val=GPIOD->ODR;
	val&=~(0x0f<<12);
	val|=leds<<12;
	GPIOD->ODR=val;
}

static inline void ToggleLEDs(int leds)
{
	GPIOD->ODR^=leds<<12;
}
static inline void TurnOnLEDs(int leds)
{
	GPIOD->BSRRL=leds<<12;
}

static inline void TurnOffLEDs(int leds)
{
	GPIOD->BSRRH=leds<<12;
}

#endif

