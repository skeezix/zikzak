#ifndef __UNPACKED_BITMAP_H__
#define __UNPACKED_BITMAP_H__

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>



typedef struct UnpackedPixel
{
	uint8_t r,g,b,a;
} UnpackedPixel;

static inline UnpackedPixel MakeUnpackedPixel(int r,int g,int b,int a)
{
	if(r<0) r=0; if(r>255) r=255;
	if(g<0) g=0; if(g>255) g=255;
	if(b<0) b=0; if(b>255) b=255;
	if(a<0) a=0; if(a>255) a=255;
	return (UnpackedPixel){ .r=r, .g=g, .b=b, .a=a };
}

static inline int UnpackedPixelRed(const UnpackedPixel p) { return p.r; }
static inline int UnpackedPixelGreen(const UnpackedPixel p) { return p.g; }
static inline int UnpackedPixelBlue(const UnpackedPixel p) { return p.b; }
static inline int UnpackedPixelAlpha(const UnpackedPixel p) { return p.a; }

static inline bool IsUnpackedPixelTransparent(const UnpackedPixel p) { return p.a<128; }

static inline bool UnpackedPixelsAreEqual(const UnpackedPixel a,const UnpackedPixel b) { return a.r==b.r && a.g==b.g && a.b==b.b && a.a==b.a; }



typedef struct UnpackedBitmap
{
	int width,height;
	UnpackedPixel pixels[0];
} UnpackedBitmap;

static UnpackedBitmap *AllocBitmapOrDie(int width,int height);
static UnpackedBitmap *AllocSubBitmapOrDie(const UnpackedBitmap *bitmap,int x0,int y0,int width,int height);
static inline UnpackedPixel UnpackedPixelAt(const UnpackedBitmap *bitmap,int x,int y);
static inline void SetUnpackedPixelAt(UnpackedBitmap *bitmap,int x,int y,UnpackedPixel p);
static inline bool IsUnpackedPixelTransparentAt(const UnpackedBitmap *bitmap,int x,int y);
static bool IsPartialBitmapTransparent(const UnpackedBitmap *self,int x,int y,int w,int h);
static bool IsPartialBitmapInvisible(const UnpackedBitmap *self,int x,int y,int w,int h);
static inline bool IsBitmapTransparent(const UnpackedBitmap *self);
static inline bool IsBitmapInvisible(const UnpackedBitmap *self);

static UnpackedBitmap *AllocBitmapOrDie(int width,int height)
{
	UnpackedBitmap *self=malloc(sizeof(UnpackedBitmap)+width*height*sizeof(UnpackedPixel));
	if(!self)
	{
		fprintf(stderr,"Failed to allocate a bitmap of size %dx%d.\n",width,height);
		exit(1);
	}

	self->width=width;
	self->height=height;

	return self;
}

static UnpackedBitmap *AllocSubBitmapOrDie(const UnpackedBitmap *bitmap,int x0,int y0,int width,int height)
{
	UnpackedBitmap *self=AllocBitmapOrDie(width,height);

	for(int y=0;y<height;y++)
	for(int x=0;x<width;x++)
	{
		SetUnpackedPixelAt(self,x,y,UnpackedPixelAt(bitmap,x+x0,y+y0));
	}

	return self;
}

static inline UnpackedPixel UnpackedPixelAt(const UnpackedBitmap *bitmap,int x,int y)
{
	if(x<0||y<0||x>=bitmap->width||y>=bitmap->height) return MakeUnpackedPixel(0,0,0,0);
	return bitmap->pixels[x+y*bitmap->width];
}

static inline void SetUnpackedPixelAt(UnpackedBitmap *bitmap,int x,int y,UnpackedPixel p)
{
	if(x<0||y<0||x>=bitmap->width||y>=bitmap->height) return;
	bitmap->pixels[x+y*bitmap->width]=p;
}

static inline bool IsUnpackedPixelTransparentAt(const UnpackedBitmap *bitmap,int x,int y)
{
	return IsUnpackedPixelTransparent(UnpackedPixelAt(bitmap,x,y));
}

static bool IsPartialBitmapTransparent(const UnpackedBitmap *self,int x,int y,int w,int h)
{
	for(int dy=0;dy<h;dy++)
	for(int dx=0;dx<w;dx++)
	if(IsUnpackedPixelTransparentAt(self,x+dx,y+dy)) return true;

	return false;
}

static bool IsPartialBitmapInvisible(const UnpackedBitmap *self,int x,int y,int w,int h)
{
	for(int dy=0;dy<h;dy++)
	for(int dx=0;dx<w;dx++)
	if(!IsUnpackedPixelTransparentAt(self,x+dx,y+dy)) return false;

	return true;
}

static inline bool IsBitmapTransparent(const UnpackedBitmap *self)
{
	return IsPartialBitmapTransparent(self,0,0,self->width,self->height);
}

static inline bool IsBitmapInvisible(const UnpackedBitmap *self)
{
	return IsPartialBitmapInvisible(self,0,0,self->width,self->height);
}

#endif
