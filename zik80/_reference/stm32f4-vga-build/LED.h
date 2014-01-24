#ifndef __LED_H__
#define __LED_H__

#include "stm32f4xx.h"

void InitializeLEDs();

static inline void SetLEDs(int leds)
{
	uint16_t val=GPIOG->ODR;
	val&=~(0x0f<<12);
	val|=leds<<12;
	GPIOG->ODR=val;
}

static inline void ToggleLEDs(int leds)
{
	GPIOG->ODR^=leds<<12;
}
static inline void TurnOnLEDs(int leds)
{
	GPIOG->BSRRL=leds<<12;
}

static inline void TurnOffLEDs(int leds)
{
	GPIOG->BSRRH=leds<<12;
}

#endif

