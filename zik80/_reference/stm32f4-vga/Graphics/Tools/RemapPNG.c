#include "BitmapLoader.h"
#include "BitmapSaver.h"

#include <stdio.h>
#include <string.h>
#include <ctype.h>

int main(int argc,const char **argv)
{
	if(argc!=3 && argc!=6)
	{
		fprintf(stderr,"Usage: %s palette.png row <input.png >output.png\n",argv[0]);
		fprintf(stderr,"       %s x y width height row <input.png >output.png\n",argv[0]);
		fprintf(stderr,"Where \"row\" is either \"+N\" for relative remapping or \"N\" for absolute.\n");
		exit(1);
	}

	UnpackedBitmap *bitmap=AllocAndLoadBitmapOrDie("-");

	UnpackedBitmap *palette;
	const char *rowstr;

	if(argc==3)
	{
		palette=AllocAndLoadBitmapOrDie(argv[1]);
		rowstr=argv[2];
	}
	else
	{
		int x=atoi(argv[1]);
		int y=atoi(argv[2]);
		int width=atoi(argv[3]);
		int height=atoi(argv[4]);
		rowstr=argv[5];

		palette=AllocSubBitmapOrDie(bitmap,x,y,width,height);
	}

	bool relative=false;
	if(rowstr[0]=='+')
	{
		relative=true;
		rowstr++;
	}

	int remaprow=atoi(rowstr);

	for(int i=0;i<bitmap->width*bitmap->height;i++)
	{
		UnpackedPixel p=bitmap->pixels[i];
		int j;
		for(j=0;j<palette->width*palette->height;j++)
		{
			if(UnpackedPixelsAreEqual(p,palette->pixels[j]))
			{
				int col=j%palette->width;
				int row=j/palette->width;

				if(relative) row=(row+remaprow)%palette->height;
				else row=remaprow;

				bitmap->pixels[i]=UnpackedPixelAt(palette,col,row);

				break;
			}
		}
	}

	SaveBitmapOrDie(bitmap,"-");

	return 0;
}
