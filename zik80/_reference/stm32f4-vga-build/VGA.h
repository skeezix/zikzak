#ifndef __VGA_H__
#define __VGA_H__

#include "System.h"
#include "stm32f4xx.h"

#include <stdint.h>

typedef void HBlankInterruptFunction(void);

extern volatile uint32_t VGALine;
extern volatile uint32_t VGAFrame;

void InitializeVGA();

void SetBlankVGAScreenMode480();
void SetBlankVGAScreenMode400();
void SetBlankVGAScreenMode350();

void SetVGAScreenMode240(uint8_t *framebuffer,int pixelsperrow,int pixelclock);
void SetVGAScreenMode200_60Hz(uint8_t *framebuffer,int pixelsperrow,int pixelclock);
void SetVGAScreenMode200(uint8_t *framebuffer,int pixelsperrow,int pixelclock);
void SetVGAScreenMode175(uint8_t *framebuffer,int pixelsperrow,int pixelclock);

void SetVGAScreenMode160(uint8_t *framebuffer,int pixelsperrow,int pixelclock);
void SetVGAScreenMode133_60Hz(uint8_t *framebuffer,int pixelsperrow,int pixelclock);
void SetVGAScreenMode133(uint8_t *framebuffer,int pixelsperrow,int pixelclock);
void SetVGAScreenMode117(uint8_t *framebuffer,int pixelsperrow,int pixelclock);

void SetFrameBuffer(uint8_t *framebuffer);

#ifdef EnableOverclocking
static inline void SetVGAScreenMode320x240(uint8_t *framebuffer) { SetVGAScreenMode240(framebuffer,320,14); }
static inline void SetVGAScreenMode320x200_60Hz(uint8_t *framebuffer) { SetVGAScreenMode200_60Hz(framebuffer,320,14); }
static inline void SetVGAScreenMode320x200(uint8_t *framebuffer) { SetVGAScreenMode200(framebuffer,320,14); }
static inline void SetVGAScreenMode320x175(uint8_t *framebuffer) { SetVGAScreenMode175(framebuffer,320,14); }

static inline void SetVGAScreenMode212x160(uint8_t *framebuffer) { SetVGAScreenMode160(framebuffer,212,21); }
static inline void SetVGAScreenMode212x133_60Hz(uint8_t *framebuffer) { SetVGAScreenMode133_60Hz(framebuffer,212,21); }
static inline void SetVGAScreenMode212x133(uint8_t *framebuffer) { SetVGAScreenMode133(framebuffer,212,21); }
static inline void SetVGAScreenMode212x175(uint8_t *framebuffer) { SetVGAScreenMode117(framebuffer,212,21); }
#else
static inline void SetVGAScreenMode320x240(uint8_t *framebuffer) { SetVGAScreenMode240(framebuffer,320,13); }
static inline void SetVGAScreenMode320x200_60Hz(uint8_t *framebuffer) { SetVGAScreenMode200_60Hz(framebuffer,320,13); }
static inline void SetVGAScreenMode320x200(uint8_t *framebuffer) { SetVGAScreenMode200(framebuffer,320,13); }
static inline void SetVGAScreenMode320x175(uint8_t *framebuffer) { SetVGAScreenMode175(framebuffer,320,13); }

static inline void SetVGAScreenMode212x160(uint8_t *framebuffer) { SetVGAScreenMode160(framebuffer,212,19); }
static inline void SetVGAScreenMode212x133_60Hz(uint8_t *framebuffer) { SetVGAScreenMode133_60Hz(framebuffer,212,19); }
static inline void SetVGAScreenMode212x133(uint8_t *framebuffer) { SetVGAScreenMode133(framebuffer,212,19); }
static inline void SetVGAScreenMode212x175(uint8_t *framebuffer) { SetVGAScreenMode117(framebuffer,212,19); }
#endif

static inline void WaitVBL()
{
	uint32_t currframe=VGAFrame;
	while(VGAFrame==currframe);
}

static inline uint32_t VGAFrameCounter() { return VGAFrame; }



// Functions for implementing your own video driving code.

void SetVGAHorizontalSync31kHz(InterruptHandler *handler);
void SetVGAHorizontalSync31kHzActiveHigh(InterruptHandler *handler);
void SetVGAHorizontalSync31kHzWithEarlyStart(InterruptHandler *handler,int offset);
void SetVGAHorizontalSync31kHzActiveHighWithEarlyStart(InterruptHandler *handler,int offset);

static inline void RaiseVGAVSyncLine() { GPIOB->BSRRL=(1<<12); }
static inline void LowerVGAVSyncLine() { GPIOB->BSRRH=(1<<12); }

static inline void SetVGASignalToBlack() { GPIOE->BSRRH=0xff00; }
static inline void SetVGASignal(uint8_t pixel) { ((uint8_t *)&GPIOE->ODR)[1]=pixel; }

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

static inline int HandleVGAHSync400_60Hz()
{
	// TIM2->SR=0;
	__asm__ volatile(
	"	mov.w	r1,#0x40000000\n"
	"	movs	r0,#0\n"
	"	strh	r0,[r1,#0x10]\n"
	:::"r0","r1");

	VGALine++;
	if(VGALine<40)
	{
		return -1;
	}
	else if(VGALine<440)
	{
		return VGALine-40;
	}
	else if(VGALine==440)
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

static inline int HandleVGAHSync400()
{
	// TIM2->SR=0;
	__asm__ volatile(
	"	mov.w	r1,#0x40000000\n"
	"	movs	r0,#0\n"
	"	strh	r0,[r1,#0x10]\n"
	:::"r0","r1");

	VGALine++;
	if(VGALine<400)
	{
		return VGALine;
	}
	else if(VGALine==400)
	{
		VGAFrame++;
	}
	else if(VGALine==412)
	{
		RaiseVGAVSyncLine();
	}
	else if(VGALine==414)
	{
		LowerVGAVSyncLine();
	}
	else if(VGALine==448)
	{
		VGALine=-1;
	}
	return -1;
}

static inline int HandleVGAHSync350()
{
	// TIM2->SR=0;
	__asm__ volatile(
	"	mov.w	r1,#0x40000000\n"
	"	movs	r0,#0\n"
	"	strh	r0,[r1,#0x10]\n"
	:::"r0","r1");

	VGALine++;
	if(VGALine<350)
	{
		return VGALine;
	}
	else if(VGALine==350)
	{
		VGAFrame++;
	}
	else if(VGALine==387)
	{
		LowerVGAVSyncLine();
	}
	else if(VGALine==389)
	{
		RaiseVGAVSyncLine();
	}
	else if(VGALine==448)
	{
		VGALine=-1;
	}
	return -1;
}

#endif
