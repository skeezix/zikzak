#ifndef __BINARY_LOADER_H__
#define __BINARY_LOADER_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#include <io.h>
#include <sys/fcntl.h>
int _fileno(FILE *fh); // Why isn't this defined? Who knows.
#endif

static void *AllocAndLoadFileOrDie(const char *filename,size_t *sizeptr)
{
	FILE *fh;

	if(strcmp(filename,"-")==0)
	{
		fh=stdin;
		#ifdef _WIN32
		setmode(_fileno(stdin),O_BINARY);
		#endif
	}
	else
	{
		fh=fopen(filename,"rb");
		if(!fh)
		{
			fprintf(stderr,"Failed to open file \"%s\".\n",filename);
			exit(1);
		}
	}

	char *bytes=NULL;
	size_t size=0;

	while(!feof(fh))
	{
		char buf[65536];
		size_t actual=fread(buf,1,sizeof(buf),fh);

		if(ferror(fh))
		{
			fprintf(stderr,"Failed to read from file \"%s\".\n",filename);
			exit(1);
		}

		bytes=realloc(bytes,size+actual);
		if(!bytes)
		{
			fprintf(stderr,"Out of memory while reading from file \"%s\".\n",filename);
			exit(1);
		}

		memcpy(&bytes[size],buf,actual);
		size+=actual;
	}

	fclose(fh);

	if(sizeptr) *sizeptr=size;

	return bytes;
}

#endif
