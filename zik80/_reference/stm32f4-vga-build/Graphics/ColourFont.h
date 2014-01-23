#ifndef __COLOUR_FONT_H__
#define __COLOUR_FONT_H__

#include "Font.h"
#include "RLEBitmap.h"

typedef struct ColourFont
{
	Font font;
	int firstglyph,lastglyph;
	const RLEBitmap *glyphs[0];
} ColourFont;

int WidthOfColourFontCharacter(const Font *font,int c);
void DrawColourFontCharacter(Bitmap *bitmap,const Font *font,int x,int y,Pixel col,int c);
void CompositeColourFontCharacter(Bitmap *bitmap,const Font *font,int x,int y,Pixel col,CompositionMode comp,int c);

#endif
