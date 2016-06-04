
#include <eZ80.h>
#include <uart.h>
#include <gpio.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "skeelib.h"
#include "lib_serial.h"
#include "flashtool_menu.h"
#include "lib_crc32.h"

// ASSUMPTION:
// - serial is already set up


/* convenience functions
 */
 
unsigned char log_format = 1;

void logit ( char *foo ) {
  write_UART0 ( foo, lame_strlen ( foo ) );
  return;
}

void logaddress ( unsigned int address, unsigned char value ) {
  char buffer [ 30 ];

  sprintf ( buffer, "%5u: ", address );
  logit ( buffer );

  sprintf ( buffer, "%X ", value );
  logit ( buffer );

#if 0 // binary dump as well?
  sprintf ( buffer, BYTETOBINARYPATTERN, BYTETOBINARY(b) );
  logit ( buffer );
#endif

  if ( log_format ) {

    if ( address != 0 && address % 6 == 0 ) {
      logit ( "\n" );
    } else {
      logit ( "\t" );
    }

  } else {
    logit ( "\n" );
  }

  return;
}


/* get to it
 */

void flashtool_serial_forever ( void ) {
  serial_state_e state;
  char c;

  // state machine vars .. don't screw with them
  unsigned char char_echo_mode = 1; // default on
  char buffer [ 50 ];               // input from terminal accrued here
  char *args;                       // set on entering non-cmd-parse states; used randomly otherwise
  unsigned char data_buffer [ MAXDATASIZE ];   // as received from terminal
  unsigned int data_len = 0;                   // length of 'data_buffer'

  // test: echo time..
  //serial_echo_loop_forever();

  state = ss_ready;

  while ( 1 ) {

    switch ( state ) {

    case ss_ready:
      logit ( "+READY\n" );
      // reset for next go..
      buffer [ 0 ] = '\0';
      state = ss_cmd_build;
      break;

    case ss_cmd_build:

      c = uart_getchar_block();

      if ( c == '\n'  || c == '\r') {

        if ( c == '\r' ) {
          if ( char_echo_mode ) {
            uart_putchar ( '\n' );
          }
        }

        // parse the buffer; modify to have \0 at first space, so we
        // can find command and assume the rest is args
        args = strchr ( buffer, ' ' );

        if ( args ) {
          *args = '\0';
          args ++;
        } else {
          args = NULL;
        }

        // dispatch command
        if ( strcmp ( buffer, "ohai" ) == 0 ) {
          state = ss_ohai;
        } else if ( strcmp ( buffer, "echo" ) == 0 ) {
          state = ss_echo;
        } else if ( strcmp ( buffer, "receive" ) == 0 ) {
          state = ss_receive;
        } else if ( strcmp ( buffer, "burn" ) == 0 ) {
          state = ss_burn;
        } else if ( strcmp ( buffer, "dump" ) == 0 ) {
          state = ss_dump;
        } else if ( strcmp ( buffer, "help" ) == 0 ) {
          state = ss_help;
        } else if ( strcmp ( buffer, "charecho" ) == 0 ) {
          state = ss_charecho;
        } else if ( strcmp ( buffer, "format" ) == 0 ) {
          state = ss_format;
        } else if ( strcmp ( buffer, "buffer" ) == 0 ) {
          state = ss_buffer;
        } else if ( strcmp ( buffer, "testbuf" ) == 0 ) {
          state = ss_testbuf;
        } else {
          logit ( "+BADCOMMAND\n" );
          buffer [ 0 ] = '\0';
        }

      } else {

        if ( strlen ( buffer ) < 49 ) {
          sprintf ( strchr ( buffer, '\0' ), "%c", c );
        } else {
          // uuuh, overrun, .. reset?!
          logit ( "+GARBAGE\n" );
          buffer [ 0 ] = '\0';
        }

      }

      if ( char_echo_mode ) {
        uart_putchar ( c );
      }

      break;

    case ss_ohai:
      logit ( "+OHAI\n" );

      log_format = 1;
      char_echo_mode = 1;

      state = ss_ready;
      break;

    case ss_help:

      logit ( "Enter commands into terminal.\n" );
      logit ( "\n" );
      logit ( "ohai -> return OHAI; reset to defaults. (use as first command, always)\n" );
      logit ( "echo -> enter loop, returning received characters.. forever\n" );
      logit ( "receive N -> store next N chars (after command return) to buffer\n" );
      logit ( "burn A -> given a received buffer, burn to address A\n" );
      logit ( "dump A L -> hexdump from address A of length L\n" );
      logit ( "charecho -> toggle character echo\n" );
      logit ( "buffer -> dump the currently received buffer\n" );
      logit ( "testbuf V -> generate a buffer of 0xV\n" );
      logit ( "format -> toggle dump formatting; default on. When off, only 1 address/value per line\n" );
      logit ( "help -> duh\n" );
      logit ( "\n" );

      state = ss_ready;
      break;

    case ss_charecho:

      if ( char_echo_mode ) {
        char_echo_mode = 0;
      } else {
        char_echo_mode = 1;
      }

      state = ss_ready;
      break;

    case ss_format:

      if ( log_format ) {
        log_format = 0;
      } else {
        log_format = 1;
      }

      state = ss_ready;
      break;

    case ss_echo:
      logit ( "+ECHO\n" );

      uart_echo_loop_forever();

      state = ss_ready; // should never get here...
      break;

    case ss_testbuf:
      {
		unsigned int b;
        char log [ 40 ];

        // verify arguments
        if ( ! args ) {
          logit ( "+BADARGS\n" );
          state = ss_ready;
          break;
        }

        b = atoi ( args );

        if ( b > 255 ) {
          logit ( "+BADARGS\n" );
          state = ss_ready;
          break;
        }

        data_len = 255;

		{
			unsigned int counter;
			for ( counter = 0; counter < data_len; counter++ ) {
			  data_buffer [ counter ] = b;
			}
		}

        sprintf ( log, "# Generated %d bytes of value %X\n", data_len, b );
        logit ( log );

      }

      state = ss_ready;
      break;

    case ss_buffer:
      {
        unsigned int counter;

        if ( ( data_len == 0 ) ||
             ( data_len > MAXDATASIZE ) )
        {
          logit ( "+BADARGS\n" );
          state = ss_ready;
          break;
        }

        for ( counter = ((unsigned int)0); counter < ((unsigned int) data_len); counter++ ) {
          logaddress ( counter, data_buffer [ counter ] );
        } // for
        logit ( "\n" );

        state = ss_ready;
        break;
      }

    case ss_receive:
      // receive LEN
      // TODO: someday, possibly..
      // TODO: receive LEN [NAME] --> allow naming the incoming buffer for re-use?

      // verify arguments
      if ( ! args ) {
        logit ( "+BADARGS\n" );
        state = ss_ready;
        break;
      }

      data_len = atoi ( args );

      if ( ( data_len == 0 ) ||
           ( data_len > MAXDATASIZE ) )
      {
        logit ( "+BADARGS\n" );
        state = ss_ready;
        break;
      }

      //sprintf ( log, "# Receive %d bytes\n", data_len );
      //logit ( log );

      {
        char log [ 40 ];
        unsigned int counter;
		unsigned int crc;

  	    for ( counter = 0; counter < data_len; counter++ ) {
          data_buffer [ counter ] = uart_getchar_block();
        }

        crc = crc32 ( 0, data_buffer, data_len );

        sprintf ( log, "+RECEIVE %X\n", crc );
        logit ( log );
      }

      state = ss_ready;
      break;

    case ss_burn:
      {
        // burn address
        // TODO: Someday maybe..
        // TODO: burn address [name] [len] (see above)
		  unsigned int address;
		  char log [ 40 ];

        // verify arguments
        if ( ! args ) {
          logit ( "+BADARGS\n" );
          state = ss_ready;
          break;
        }

        address = atoi ( args );

        if ( /*( address > 65000 ) ||*/
             ( data_len == 0 ) )
        {
          logit ( "+BADARGS\n" );
          state = ss_ready;
          break;
        }

        sprintf ( log, "# Address %X\n", address );
        logit ( log );

		
        // burn it out
		{
			UINT8 *burnaddr = (UINT8*) address;
			UINT16 burnlen = data_len;
			UINT8 *srcaddr = data_buffer;
			
			while ( burnlen ) {
				*burnaddr++ = *srcaddr++;
				delay_ms_spin(2); // WARN: in main.c, none was needed; but here, some is needed or it fails... optims?!
				burnlen--;
			}
			
		}
		
        // verify
        if ( memcmp ( (void*)address, (void*)data_buffer, (size_t)data_len ) == 0 ) {
          logit ( "+BURNOK\n" );
        } else {
          logit ( "+BURNFAIL\n" );
        }

      }
      state = ss_ready;
      break;

    case ss_dump:
      // dump address len
      {
        char *split;
        unsigned int address;
        unsigned int len;

        // verify arguments
        if ( ! args ) {
          logit ( "+BADARGS\n" );
          state = ss_ready;
          break;
        }

        split = strchr ( args, ' ' );
        address = atoi ( args );
        len = atoi ( split + 1 );

        if ( /*( address > 65000 ) ||*/
             ( len == 0 ) )
        {
          logit ( "+BADARGS\n" );
          state = ss_ready;
          break;
        }

        // dump it out
		while ( len ) {
			logaddress ( address, *((UINT8*)address) );
			address++;
			len--;
		}

        logit ( "+OK\n" );

      }

      state = ss_ready;
      break;

    } // switch



  } // while

  return;
}
