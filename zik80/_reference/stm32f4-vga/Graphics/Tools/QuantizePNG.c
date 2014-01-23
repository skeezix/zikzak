#include "BitmapLoader.h"
#include "BitmapSaver.h"

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

int main(int argc,const char **argv)
{
	if(argc!=2 && argc!=5)
	{
		fprintf(stderr,"Usage: %s palette.png <input.png >output.png\n",argv[0]);
		fprintf(stderr,"       %s x y width height <input.png >output.png\n",argv[0]);
		fprintf(stderr,"Where \"row\" is either \"+N\" for relative remapping or \"N\" for absolute.\n");
		exit(1);
	}

	UnpackedBitmap *bitmap=AllocAndLoadBitmapOrDie("-");

	UnpackedBitmap *palette;

	if(argc==2)
	{
		palette=AllocAndLoadBitmapOrDie(argv[1]);
	}
	else
	{
		int x=atoi(argv[1]);
		int y=atoi(argv[2]);
		int width=atoi(argv[3]);
		int height=atoi(argv[4]);

		palette=AllocSubBitmapOrDie(bitmap,x,y,width,height);
	}

	for(int i=0;i<bitmap->width*bitmap->height;i++)
	{
		UnpackedPixel p=bitmap->pixels[i];
		int closest=0;
		int mindist=10000;

		for(int j=0;j<palette->width*palette->height;j++)
		{
			UnpackedPixel p2=palette->pixels[j];
			int dist=sqrt((p.r-p2.r)*(p.r-p2.r)+(p.g-p2.g)*(p.g-p2.g)+
			(p.b-p2.b)*(p.b-p2.b)+(p.a-p2.a)*(p.a-p2.a));

			if(dist<mindist)
			{
				mindist=dist;
				closest=j;
			}
		}

		bitmap->pixels[i]=palette->pixels[closest];
	}

	SaveBitmapOrDie(bitmap,"-");

	return 0;
}
