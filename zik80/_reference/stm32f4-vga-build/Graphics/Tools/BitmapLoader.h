#ifndef __BITMAP_LOADER_H__
#define __BITMAP_LOADER_H__

#include "UnpackedBitmap.h"
#include "BinaryLoader.h"
#include "PNG.h"

#include <stdio.h>
#include <stdlib.h>

static void *BitmapLoaderPixelFunc(void *destination,uint8_t r,uint8_t g,uint8_t b,uint8_t a,int x,int y);

static UnpackedBitmap *AllocAndLoadBitmapOrDie(const char *filename)
{
	size_t length;
	void *bytes=AllocAndLoadFileOrDie(filename,&length);

	PNGLoader loader;
	InitializePNGLoader(&loader,bytes,length);

	if(!LoadPNGHeader(&loader))
	{
		fprintf(stderr,"\"%s\" is not a valid PNG file.\n",filename);
		exit(1);
	}

	UnpackedBitmap *self=AllocBitmapOrDie(loader.width,loader.height);

	if(!LoadPNGImageData(&loader,self->pixels,sizeof(UnpackedPixel)*loader.width,BitmapLoaderPixelFunc))
	{
		fprintf(stderr,"Failed while loading PNG data from file \"%s\".\n",filename);
		exit(1);
	}

	free(bytes);

	return self;
}

static void *BitmapLoaderPixelFunc(void *destination,uint8_t r,uint8_t g,uint8_t b,uint8_t a,int x,int y)
{
	UnpackedPixel *pixel=destination;

	pixel->r=r;
	pixel->g=g;
	pixel->b=b;
	pixel->a=a;
	return pixel+1;
}

#endif
