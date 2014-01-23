#include "ColourFont.h"
#include "Drawing.h"

int WidthOfColourFontCharacter(const Font *font,int c)
{
	const ColourFont *self=(const ColourFont *)font;

	if(c<self->firstglyph || c>self->lastglyph) return 0;
	if(!self->glyphs[c-self->firstglyph]) return 0;

	return self->glyphs[c-self->firstglyph]->width;
}

void DrawColourFontCharacter(Bitmap *bitmap,const Font *font,int x,int y,Pixel col,int c)
{
	const ColourFont *self=(const ColourFont *)font;

	if(c<self->firstglyph || c>self->lastglyph) return;
	if(!self->glyphs[c-self->firstglyph]) return;

	const RLEBitmap *glyph=self->glyphs[c-self->firstglyph];
	DrawRLEBitmap(bitmap,glyph,x,y);
}

void CompositeColourFontCharacter(Bitmap *bitmap,const Font *font,int x,int y,Pixel col,CompositionMode comp,int c)
{
	const ColourFont *self=(const ColourFont *)font;

	if(c<self->firstglyph || c>self->lastglyph) return;
	if(!self->glyphs[c-self->firstglyph]) return;

	const RLEBitmap *glyph=self->glyphs[c-self->firstglyph];
	CompositeRLEBitmap(bitmap,glyph,x,y,comp);
}

