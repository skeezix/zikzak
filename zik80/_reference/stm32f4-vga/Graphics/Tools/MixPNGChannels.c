#include "BitmapLoader.h"
#include "BitmapSaver.h"

#include <stdio.h>
#include <string.h>
#include <ctype.h>

static int PixelChannel(UnpackedPixel p,char channel);

int main(int argc,const char **argv)
{
	if(argc!=2)
	{
		fprintf(stderr,"Usage: %s xxxx <input.png >output.png\n",argv[0]);
		fprintf(stderr,"Where each \"x\" is one of r, g, b, a, 0 or 1.\n",argv[0]);
		exit(1);
	}

	const char *filter=argv[1];
	if(strlen(filter)!=4)
	{
		fprintf(stderr,"Usage: %s xxxx <input.png >output.png\n",argv[0]);
		fprintf(stderr,"Where each \"x\" is one of r, g, b, a, 0 or 1.\n",argv[0]);
		exit(1);
	}

	UnpackedBitmap *bitmap=AllocAndLoadBitmapOrDie("-");

	char rchannel=tolower(filter[0]);
	char gchannel=tolower(filter[1]);
	char bchannel=tolower(filter[2]);
	char achannel=tolower(filter[3]);

	for(int i=0;i<bitmap->width*bitmap->height;i++)
	{
		UnpackedPixel p=bitmap->pixels[i];
		bitmap->pixels[i].r=PixelChannel(p,rchannel);
		bitmap->pixels[i].g=PixelChannel(p,gchannel);
		bitmap->pixels[i].b=PixelChannel(p,bchannel);
		bitmap->pixels[i].a=PixelChannel(p,achannel);
	}

	SaveBitmapOrDie(bitmap,"-");

	return 0;
}

static int PixelChannel(UnpackedPixel p,char channel)
{
	if(channel=='r') return p.r;
	else if(channel=='g') return p.g;
	else if(channel=='b') return p.b;
	else if(channel=='a') return p.a;
	else if(channel=='0') return 0;
	else if(channel=='1') return 0xff;
	else return 0;
}
