#include "BitmapLoader.h"
#include "RLELoader.h"

#include <stdio.h>

static void PrintRLEStructDefinition(UnpackedRLEBitmap *rle);
static void PrintRLEBitmapInitializer(UnpackedRLEBitmap *rle,const char *indent);

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
		UnpackedRLEBitmap *rle=AllocRLEBitmapFromBitmapOrDie(bitmap);

		PrintRLEStructDefinition(rle);
		printf(" %s=\n",name);
		PrintRLEBitmapInitializer(rle,"");
		printf(";\n");

		free(rle);
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

		printf("const RLEBitmap * const %s[%d]=\n",name,numcols*numrows);
		printf("{\n");

		for(int row=0;row<numrows;row++)
		for(int col=0;col<numcols;col++)
		{
			int x=col*fw;
			int y=row*fh;

			UnpackedRLEBitmap *rle=AllocRLEBitmapFromPartialBitmapOrDie(bitmap,x,y,fw,fh);

			printf("\t(const RLEBitmap *)&(");
			PrintRLEStructDefinition(rle);
			printf(") ");
			PrintRLEBitmapInitializer(rle,"\t");
			printf(",\n");

			free(rle);
		}

		printf("};\n");
	}
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

