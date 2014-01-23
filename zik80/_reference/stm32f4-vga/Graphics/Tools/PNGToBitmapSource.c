#include "BitmapLoader.h"

#include <stdio.h>

static void PrintBitmapInitializer(UnpackedBitmap *bitmap,const char *name,const char *indent);
static void PrintBitmapPixelArray(UnpackedBitmap *bitmap,const char *name);

int main(int argc,const char **argv)
{
	if(argc!=2)
	{
		fprintf(stderr,"Usage: %s name <input.png >output.c\n",argv[0]);
		exit(1);
	}

	const char *name=argv[1];

	UnpackedBitmap *bitmap=AllocAndLoadBitmapOrDie("-");

	PrintBitmapPixelArray(bitmap,name);

	printf("const Bitmap %s=",name);
	PrintBitmapInitializer(bitmap,name,"");
	printf(";\n");
}

static void PrintBitmapInitializer(UnpackedBitmap *bitmap,const char *name,const char *indent)
{
	int w=bitmap->width;
	int h=bitmap->height;
	printf("{ %d,%d,BytesPerRowForWidth(%d),(Pixel *)%s_pixels }",w,h,h,name);

	// Thanks to gcc bug #37303, we can't use the below code which rolls
	// the pixel array into the initializer.
/*	bool transparent=IsPartialBitmapTransparent(bitmap,x,y,w,h);

	printf("{\n");

	printf("%s\t%d,%d,BytesPerRowForWidth(%d),\n",indent,w,h,h);
	printf("%s\t(const Pixel[NumberOfPixelsForWidthAndHeight(%d,%d)]) {",indent,w,h);

	for(int y=0;y<h;y++)
	{
		printf("\n");
		printf("%s\t\tStartIntializingBitmapRow(%d,%d)",indent,y,w);

		for(int x=0;x<w;x++)
		{
			if(dx%4==0) printf("\n%s\t\t",indent);

			UnpackedPixel p=UnpackedPixelAt(bitmap,x,y);
			if(transparent) printf("RGBA(0x%02x,0x%02x,0x%02x,0x%02x),",p.r,p.g,p.b,p.a);
			else printf("RGB(0x%02x,0x%02x,0x%02x),",p.r,p.g,p.b);
		}
	}

	printf("\n");
	printf("%s\t}\n",indent);
	printf("%s}",indent);*/
}

static void PrintBitmapPixelArray(UnpackedBitmap *bitmap,const char *name)
{
	int w=bitmap->width;
	int h=bitmap->height;
	bool transparent=IsBitmapTransparent(bitmap);

	printf("static const Pixel %s_pixels[NumberOfPixelsForWidthAndHeight(%d,%d)]=\n",name,w,h);
	printf("{");

	for(int y=0;y<h;y++)
	{
		printf("\n");
		printf("\tStartIntializingBitmapRow(%d,%d)",y,w);

		for(int x=0;x<w;x++)
		{
			if(x%4==0) printf("\n\t");

			UnpackedPixel p=UnpackedPixelAt(bitmap,x,y);
			if(transparent) printf("RGBA(0x%02x,0x%02x,0x%02x,0x%02x),",p.r,p.g,p.b,p.a);
			else printf("RGB(0x%02x,0x%02x,0x%02x),",p.r,p.g,p.b);
		}
	}

	printf("\n");
	printf("};\n");
}

