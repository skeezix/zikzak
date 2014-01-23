#ifndef __PIXELS_COMPOSITION_32_H__
#define __PIXELS_COMPOSITION_32_H__

#include "Functions.h"
#include "CompositionFast.h"

static Pixel SourceOverCompositionMode(Pixel below,Pixel above)
{
	int r1=ExtractRawRed(below);
	int g1=ExtractRawGreen(below);
	int b1=ExtractRawBlue(below);
	int a1=ExtractRawAlpha(below);
	int r2=ExtractRawRed(above);
	int g2=ExtractRawGreen(above);
	int b2=ExtractRawBlue(above);
	int a2=ExtractRawAlpha(above);

	int r=PixelMul(a2,r2)+PixelMul(255-a2,r1);
	int g=PixelMul(a2,g2)+PixelMul(255-a2,g1);
	int b=PixelMul(a2,b2)+PixelMul(255-a2,b1);
	int a=PixelMul(a2,a2)+PixelMul(255-a2,a1);

	return RawRGBA(r,g,b,a);
}

#endif
