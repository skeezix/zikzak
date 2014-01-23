#ifndef __PIXELS_COMPOSITION_FAST_H__
#define __PIXELS_COMPOSITION_FAST_H__

#include "Functions.h"

#if PixelAlphaBits

#define PixelLowBits ( \
	(1<<PixelRedShift)| \
	(1<<PixelGreenShift)| \
	(1<<PixelBlueShift)| \
	(1<<PixelAlphaShift) )
#define PixelAllButHighBits ( ~(\
	(1<<(PixelRedShift+PixelRedBits-1))| \
	(1<<(PixelGreenShift+PixelGreenBits-1))| \
	(1<<(PixelBlueShift+PixelBlueBits-1))| \
	(1<<(PixelAlphaShift+PixelAlphaBits-1)) ))

#else

#define PixelLowBits ( \
	(1<<PixelRedShift)| \
	(1<<PixelGreenShift)| \
	(1<<PixelBlueShift) )
#define PixelAllButHighBits ( ~(\
	(1<<(PixelRedShift+PixelRedBits-1))| \
	(1<<(PixelGreenShift+PixelGreenBits-1))| \
	(1<<(PixelBlueShift+PixelBlueBits-1)) ))

#endif

static Pixel FastTransparencyCompositionMode(Pixel below,Pixel above)
{
	if(IsPixelTransparent(above)) return below;
	else return above;
}

static Pixel FastHalfTransparentCompositionMode(Pixel below,Pixel above)
{
	if(!IsPixelTransparent(above))
	{
		Pixel halfabove=(above>>1)&PixelAllButHighBits;
		Pixel halfbelow=(below>>1)&PixelAllButHighBits;
		Pixel carry=(above&below&PixelLowBits);

		return halfabove+halfbelow+carry;
	}
	else return below;
}

static Pixel DummyCompositionMode(Pixel below,Pixel above)
{
	return above;
}

#endif
