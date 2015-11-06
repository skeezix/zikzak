#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "ym5-header.h"
#include "endian.h"

int main ( int argc, char *argv[] ) {

  if ( argc < 2 ) {
    fprintf ( stderr, "Specify file to open\n" );
    return ( 0 );
  }

  // infile
  char *fullpath = argv [ 1 ];

  FILE *f = fopen ( fullpath, "r" );

  if ( ! f ) {
    fprintf ( stderr, "Couldn't open file %s\n", fullpath );
    return ( 0 );
  }

  fseek ( f, 0, SEEK_END );
  unsigned int length = ftell ( f );
  rewind ( f );

  unsigned char *buf = (unsigned char*) malloc ( length );
  unsigned char *repack = (unsigned char*) malloc ( length );

  if ( fread ( buf, length, 1, f ) < 1 ) {
    fclose ( f );
    fprintf ( stderr, "Couldn't read file %s\n", fullpath );
    return ( 0 );
  }

  fclose ( f );

  // header check
  //
  ym5_header_t *hdr = (ym5_header_t*) buf;

  // header hack
  hdr -> vbl_count = SWAP32 ( hdr -> vbl_count );
  hdr -> attrib = SWAP32 ( hdr -> attrib );
  hdr -> drum_count = SWAP32 ( hdr -> drum_count );

  // dump
  printf ( "Header\n" );
  printf ( "ID:\t%s\n", hdr -> id );
  printf ( "Check:\t%s\n", hdr -> check );
  printf ( "VBL Count:\t%d\n", hdr -> vbl_count );
  printf ( "Attrib:\t%x\t", hdr -> attrib );
  if ( hdr -> attrib & 1 ) {
    printf ( "Interleaved\n" );
  } else {
    printf ( "Non-interleaved\n" );
  }
  printf ( "Drum Count:\t%d\n", hdr -> drum_count );

  // out bin file
  //

  // bin outfile for de-interleaved
  char fullpathbin [ FILENAME_MAX ];
  sprintf ( fullpathbin, "%s.deinterleave.bin", fullpath );

  f = fopen ( fullpathbin, "w" );

  // got buf, do something with it..
  //
  unsigned char *p = buf;

  // skip header
  p = buf + 34;

  // find null terminated strings
  //printf ( "p offset is %d %x\n", (int)(p - buf), (unsigned int) (p - buf) );
  p = (unsigned char*) strchr ( (char*) p, '\0' ) + 1;
  //printf ( "p offset is %d %x\n", (int)(p - buf), (unsigned int) (p - buf) );
  p = (unsigned char*) strchr ( (char*) p, '\0' ) + 1;
  //printf ( "p offset is %d %x\n", (int)(p - buf), (unsigned int) (p - buf) );
  p = (unsigned char*) strchr ( (char*) p, '\0' ) + 1;
  //printf ( "p offset is %d %x\n", (int)(p - buf), (unsigned int) (p - buf) );

  unsigned paylength = length - ( p - buf ) - 4 /* End! */;

  printf ( "// Data payload offset is %d %x\n", (int)(p - buf), (unsigned int) (p - buf) );
  printf ( "// Data payload in file is %d bytes\n", paylength );
  printf ( "// Module 16 -> %d\n", paylength % 16 );
  
  //
  // extract data
  //

#if 1
  // so-called "interleave"
  unsigned int frames = paylength / 16;
  unsigned int iter;
  unsigned int reg;

  for ( reg = 0; reg < 16; reg++ ) {
    for ( iter = 0; iter < frames; iter++ ) {
      repack [ ( iter * 16 ) + reg ] = p [ ( reg * frames ) + iter ];
    } // for iter
  } // for reg
  printf ( "/* interleaved */\n" );
#else
  // normal (non-"interleaved")
  memcpy ( repack, p, paylength );
  printf ( "/* non-interleaved */\n" );
#endif

  // dump bin file
  //
  fwrite ( repack, frames * 16, 1, f );
  fclose ( f );

  //
  // dump however mant we want
  //

  unsigned int framelimit = 2000;
  printf ( "#include <avr/pgmspace.h>\n" );
  printf ( "unsigned int framelimit = %d;\n", framelimit );
  printf ( "const unsigned char song_hacked_ym[] PROGMEM = {\n" );
  unsigned int framecount = framelimit;
  unsigned int a;
  unsigned char *data = repack;
  while ( framecount ) {
    printf ( "  " );
    for ( a = 0; a < 16; a++ ) {
      printf ( "0x%02x, ", *data );
      data++;
    }
    printf ( "\n" );
    framecount--;
  }
  printf ( "};\n" );
  printf ( "unsigned long int song_hacked_fullym_len = %d;\n", paylength );
  printf ( "unsigned int song_hacked_ym_len = %d;\n", framelimit * 16 );

  return ( 0 );
}
