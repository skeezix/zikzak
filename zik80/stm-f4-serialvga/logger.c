
#include <string.h>
#include "logger.h"

#define QUEUESIZE 5
#define LINESIZE 80
static signed char   read_at = -1;
static unsigned char insert_at = 0;

static char _log [ QUEUESIZE ][ LINESIZE + 1 ];

void log_setup ( void ) {
  unsigned char i, j;

  for ( i = 0; i < QUEUESIZE; i++ ) {
    for ( j = 0; j < LINESIZE; j++ ) {
      _log [ i ][ j ] = '\0';
    }
  }

  return;
}

void logit ( char *foo ) {

  //strncpy ( _log [ insert_at ], foo, LINESIZE );
  // strncpy must use DMA or something, causes problems.. brute force it instead
  unsigned char *p = foo;
  unsigned char *t = _log [ insert_at ];
  while ( *p ) {
    *t++ = *p++;
  }
  *t = '\0';

  if ( read_at == -1 ) {
    read_at = insert_at;
  }

  insert_at++;

  if ( insert_at == QUEUESIZE ) {
    insert_at = 0;
  }

  return;
}

char *logpull ( void ) {
  if ( read_at >= 0 ) {
    char *poop = _log [ read_at ];
    read_at ++;
    if ( read_at == QUEUESIZE ) {
      read_at = 0;
    }
    if ( read_at == insert_at ) {
      read_at = -1;
    }
    return ( poop );
  }
  return ( NULL );
}

unsigned char logready ( void ) {
  if ( read_at >= 0 ) {
    return ( 1 );
  }
  return ( 0 );
}
