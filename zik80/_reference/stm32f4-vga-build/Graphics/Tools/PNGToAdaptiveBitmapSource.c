#include "BitmapLoader.h"
#include "RLELoader.h"

#include <stdio.h>

static void PrintPartialBitmapInitializer(UnpackedBitmap *bitmap,int x,int y,int w,int h,const char *name,int n,const char *indent);
static void PrintPartialBitmapPixelArray(UnpackedBitmap *bitmap,int x,int y,int w,int h,const char *name,int n);
static void PrintRLEStructDefinition(UnpackedRLEBitmap *rle);
static void PrintRLEBitmapInitializer(UnpackedRLEBitmap *rle,const char *indent);
static void PrintAdaptiveBitmapPointerFromPartialBitmap(UnpackedBitmap *bitmap,int x,int y,int w,int h,const char *name,int n,const char *indent);

int main(int argc,const char **argv)
{
	if(argc!=2 && argc!=4)
	{
		fprintf(stderr,"Usage: %s name [framewidth frameheight]<input.png >output.c\n",argv[0]);
		exit(1);
	}

	const char *name=argv[1];

	UnpackedBitmap *bitmap=AllocAndLoadBitmapOrDie("-");

	if(argc==2)
	{
		if(!IsBitmapTransparent(bitmap)) PrintPartialBitmapPixelArray(bitmap,0,0,bitmap->width,bitmap->height,name,0);

		printf("const AdaptiveBitmap * const %s=",name);
		PrintAdaptiveBitmapPointerFromPartialBitmap(bitmap,0,0,bitmap->width,bitmap->height,name,0,"");
		printf(";\n");
	}
	else
	{
		int fw=atoi(argv[2]);
		int fh=atoi(argv[3]);

		if(fw<=0)
		{
			fprintf(stderr,"Invalid frame width.\n");
			exit(1);
		}

		if(fh<=0)
		{
			fprintf(stderr,"Invalid frame height.\n");
			exit(1);
		}

		int w=bitmap->width;
		int h=bitmap->height;

		int numcols=w/fw;
		int numrows=h/fh;

		for(int row=0;row<numrows;row++)
		for(int col=0;col<numcols;col++)
		{
			int n=col+row*numcols;
			int x=col*fw;
			int y=row*fh;
			if(!IsPartialBitmapTransparent(bitmap,x,y,fw,fh)) PrintPartialBitmapPixelArray(bitmap,x,y,fw,fh,name,n);
		}

		printf("const AdaptiveBitmap * const %s[%d]=\n",name,numcols*numrows);
		printf("{\n");

		for(int row=0;row<numrows;row++)
		for(int col=0;col<numcols;col++)
		{
			int n=col+row*numcols;
			int x=col*fw;
			int y=row*fh;
			PrintAdaptiveBitmapPointerFromPartialBitmap(bitmap,x,y,fw,fh,name,n,"\t");
			printf(",\n");
		}

		printf("};\n");
	}
}

static void PrintPartialBitmapInitializer(UnpackedBitmap *bitmap,int x,int y,int w,int h,const char *name,int n,const char *indent)
{
	printf("{ %d,%d,BytesPerRowForWidth(%d),(Pixel *)%s_%d }",w,h,h,name,n);

	// Thanks to gcc bug #37303, we can't use the below code which rolls
	// the pixel array into the initializer.
/*	bool transparent=IsPartialBitmapTransparent(bitmap,x,y,w,h);

	printf("{\n");

	printf("%s\t%d,%d,BytesPerRowForWidth(%d),\n",indent,w,h,h);
	printf("%s\t(const Pixel[NumberOfPixelsForWidthAndHeight(%d,%d)]) {",indent,w,h);

	for(int dy=0;dy<h;dy++)
	{
		printf("\n");
		printf("%s\t\tStartIntializingBitmapRow(%d,%d)",indent,dy,w);

		for(int dx=0;dx<w;dx++)
		{
			if(dx%4==0) printf("\n%s\t\t",indent);

			UnpackedPixel p=UnpackedPixelAt(bitmap,x+dx,y+dy);
			if(transparent) printf("RGBA(0x%02x,0x%02x,0x%02x,0x%02x),",p.r,p.g,p.b,p.a);
			else printf("RGB(0x%02x,0x%02x,0x%02x),",p.r,p.g,p.b);
		}
	}

	printf("\n");
	printf("%s\t}\n",indent);
	printf("%s}",indent);*/
}

static void PrintPartialBitmapPixelArray(UnpackedBitmap *bitmap,int x,int y,int w,int h,const char *name,int n)
{
	bool transparent=IsPartialBitmapTransparent(bitmap,x,y,w,h);

	printf("static const Pixel %s_%d[NumberOfPixelsForWidthAndHeight(%d,%d)]=\n",name,n,w,h);
	printf("{");

	for(int dy=0;dy<h;dy++)
	{
		printf("\n");
		printf("\tStartIntializingBitmapRow(%d,%d)",dy,w);

		for(int dx=0;dx<w;dx++)
		{
			if(dx%4==0) printf("\n\t");

			UnpackedPixel p=UnpackedPixelAt(bitmap,x+dx,y+dy);
			if(transparent) printf("RGBA(0x%02x,0x%02x,0x%02x,0x%02x),",p.r,p.g,p.b,p.a);
			else printf("RGB(0x%02x,0x%02x,0x%02x),",p.r,p.g,p.b);
		}
	}

	printf("\n");
	printf("};\n");
}

static void PrintRLEStructDefinition(UnpackedRLEBitmap *rle)
{
	int numpixels=0,numlengths=0,numends=0;

	for(int i=0;i<rle->numcodes;i++)
	{
		if(rle->codes[i].islength)
		{
			if(rle->codes[i].isend) numends++;
			else numlengths++;
		}
		else
		{
			numpixels++;
		}
	}

	printf("const struct { RLEBitmap rle; Pixel codes[RLECodeCount(%d,%d,%d)]; }",numpixels,numlengths,numends);
}

static void PrintRLEBitmapInitializer(UnpackedRLEBitmap *rle,const char *indent)
{
	printf("{\n");

	printf("%s\t{ %d,%d },\n",indent,rle->width,rle->height);
	printf("%s\t{",indent);

	for(int i=0;i<rle->numcodes;i++)
	{
		if(i%4==0) printf("\n%s\t\t",indent);

		if(rle->codes[i].islength)
		{
			if(rle->codes[i].isend) printf("RLEEndCode(0x%02x),   ",rle->codes[i].value.lengths.empty);
			else printf("RLECode(0x%02x,0x%02x), ",rle->codes[i].value.lengths.empty,rle->codes[i].value.lengths.filled);
		}
		else
		{
			printf("RGB(0x%02x,0x%02x,0x%02x),",
			rle->codes[i].value.pixel.r,rle->codes[i].value.pixel.g,rle->codes[i].value.pixel.b);
		}
	}

	printf("\n");
	printf("%s\t}\n",indent);
	printf("%s}",indent);
}

static void PrintAdaptiveBitmapPointerFromPartialBitmap(UnpackedBitmap *bitmap,int x,int y,int w,int h,const char *name,int n,const char *indent)
{
	if(IsPartialBitmapInvisible(bitmap,x,y,w,h))
	{
		printf("\tNULL");
	}
	else if(IsPartialBitmapTransparent(bitmap,x,y,w,h))
	{
		UnpackedRLEBitmap *rle=AllocRLEBitmapFromPartialBitmapOrDie(bitmap,x,y,w,h);

		printf("\tStartRLEBitmapToAdaptiveBitmapCast (const RLEBitmap *)&(");
		PrintRLEStructDefinition(rle);
		printf(") ");
		PrintRLEBitmapInitializer(rle,indent);
		printf(" EndRLEBitmapToAdaptiveBitmapCast");

		free(rle);
	}
	else
	{
		printf("\tStartBitmapToAdaptiveBitmapCast &(const Bitmap)");
		PrintPartialBitmapInitializer(bitmap,x,y,w,h,name,n,indent);
		printf(" EndBitmapToAdaptiveBitmapCast");
	}
}
