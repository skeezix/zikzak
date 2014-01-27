#ifndef h_int_swipe_h
#define h_int_swipe_H

#include <stdbool.h>

typedef void InterruptHandler();

void InitializeSystem();

void InstallInterruptHandler(IRQn_Type interrupt,InterruptHandler handler);
void RemoveInterruptHandler(IRQn_Type interrupt,InterruptHandler handler);

static inline void EnableInterrupt(IRQn_Type interrupt)
{
	int regindex=interrupt>>5;
	int shift=interrupt&0x1f;
	NVIC->ISER[regindex]|=1<<shift;
}

static inline void DisableInterrupt(IRQn_Type interrupt)
{
	int regindex=interrupt>>5;
	int shift=interrupt&0x1f;
	NVIC->ISER[regindex]&=~(1<<shift);
}

static inline bool IsInterruptEnabled(IRQn_Type interrupt)
{
	int regindex=interrupt>>5;
	int shift=interrupt&0x1f;
	if(NVIC->ISER[regindex]&(1<<shift)) return true;
	else return false;
}

static inline void SetInterruptPriority(IRQn_Type interrupt,int priority)
{
	NVIC->IP[interrupt]=priority<<4;
}

#endif
