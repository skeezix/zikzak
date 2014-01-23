#include "BitmapLoader.h"
#include "BitmapSaver.h"

#include <stdio.h>

int main(int argc,const char **argv)
{
	if(argc!=5)
	{
		fprintf(stderr,"Usage: %s x y width height <input.png >output.png\n",argv[0]);
		exit(1);
	}

	UnpackedBitmap *bitmap=AllocAndLoadBitmapOrDie("-");

	int	x0=atoi(argv[1]);
	int y0=atoi(argv[2]);
	int width=atoi(argv[3]);
	int height=atoi(argv[4]);

	if(width<=0)
	{
		fprintf(stderr,"Invalid width.\n");
		exit(1);
	}

	if(height<=0)
	{
		fprintf(stderr,"Invalid height.\n");
		exit(1);
	}

	UnpackedBitmap *crop=AllocSubBitmapOrDie(bitmap,x0,y0,width,height);

	SaveBitmapOrDie(crop,"-");

	return 0;
}
