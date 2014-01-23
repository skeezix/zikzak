#ifndef __PIXELS_COMPOSITION_SIMPLE_H__
#define __PIXELS_COMPOSITION_SIMPLE_H__

#include "CompositionFast.h"

static Pixel SourceOverCompositionMode(Pixel below,Pixel above)
{
	return FastTransparencyCompositionMode(below,above);
}

#endif
