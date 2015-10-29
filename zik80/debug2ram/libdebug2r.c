
#include <string.h>
#include "libdebug2r.h"

#ifdef D2R_ON
char *_d2r_literals [ D2R_MAX ];   /* literal store */
unsigned char _d2r_head = D2R_MAX + 1;   /* default is off the end */
unsigned char _d2r_tail = D2R_MAX + 1;   /* default is off the end */

unsigned char *_d2r_stringbase = (unsigned char*) 0x0D0000;
#define D2R_STRINGSTRIDE 20

void _d2r_logliteral ( char *s ) {

  // first insert? reset..
  if ( _d2r_tail == D2R_MAX + 1 ) {
    _d2r_head = 0;
    _d2r_tail = 1;
    _d2r_literals [ 0 ] = s;
    return;
  }

  // store
  _d2r_literals [ _d2r_tail ] = s;

  // move up the head?
  if ( _d2r_tail == _d2r_head ) {
    _d2r_head = ( _d2r_head + 1 ) % D2R_MAX;
  }

  // move up the tail
  _d2r_tail = ( _d2r_tail + 1 ) % D2R_MAX;

  return;
}

void _d2r_logstring ( char *s ) {

  // first insert? reset..
  if ( _d2r_tail == D2R_MAX + 1 ) {
    memset ( _d2r_stringbase, '\0', 20*20 );
    _d2r_head = 0;
    _d2r_tail = 1;
    _d2r_literals [ 0 ] = s;
    return;
  }

  // store
  strcpy ( _d2r_stringbase + (D2R_STRINGSTRIDE * _d2r_tail), s );
  _d2r_literals [ _d2r_tail ] = 0x00; // indicator

  // move up the head?
  if ( _d2r_tail == _d2r_head ) {
    _d2r_head = ( _d2r_head + 1 ) % D2R_MAX;
  }

  // move up the tail
  _d2r_tail = ( _d2r_tail + 1 ) % D2R_MAX;

  return;
}

char *_d2r_fetchliteral ( char **h ) {
  return *h;
}

D2R_HANDLE_T _d2r_first ( void ) {

  if ( _d2r_head == D2R_MAX + 1 ) {
    return ( NULL );
  }

  return ( & _d2r_literals [ _d2r_head ] );
}

D2R_HANDLE_T _d2r_next ( char **h ) {
  unsigned char i;

  if ( ! h ) {
    return ( NULL );
  }

  i = ( h - _d2r_literals );

  i = ( i + 1  ) % D2R_MAX;

  if ( i == _d2r_tail ) {
    return ( NULL );
  }

  return ( & _d2r_literals [ i ] );
}

#endif

#ifdef TESTMAIN
#include <stdio.h>

int main ( int argc, char *argv[] ) {
  printf ( "-- TEST START\n" );

  // do some logging
  //

  D2R_LOGLITERAL ( "one" );
  D2R_LOGLITERAL ( "two" );
#if 1
  D2R_LOGLITERAL ( "three" );
  D2R_LOGLITERAL ( "four" );
  D2R_LOGLITERAL ( "five" );
  D2R_LOGLITERAL ( "six" );
  D2R_LOGLITERAL ( "seven" );
  D2R_LOGLITERAL ( "eight" );
  D2R_LOGLITERAL ( "nine" );
  D2R_LOGLITERAL ( "ten" );
  D2R_LOGLITERAL ( "eleven" );
  D2R_LOGLITERAL ( "twelve" );
  D2R_LOGLITERAL ( "thirteen" );
#endif

  // dump the log
  //

  D2R_HANDLE_T dh;
  dh = D2R_FIRST();

  while ( dh ) {

    printf ( "%s\n", D2R_FETCHLITERAL ( dh ) );
    fflush ( stdout );

    dh = D2R_NEXT ( dh );
  } // while

  // done
  //

  printf ( "-- TEST DONE\n" );
  return ( 0 );
}
#endif
