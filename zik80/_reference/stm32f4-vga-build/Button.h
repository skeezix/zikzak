#ifndef __BUTTON_H__
#define __BUTTON_H__

#include "stm32f4xx.h"

#include <stdbool.h>

void InitializeUserButton();
void EnableUserButtonInterrupt();

static inline bool UserButtonState()
{
	return (GPIOA->IDR>>0)&0x01;
}

#endif

