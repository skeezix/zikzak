#ifndef h_command_h
#define h_command_h

#define _s(a,b) (a+(b<<8)) /* squinch together */

#define DL _s ( 'D', 'L' ) // draw line; DL <colour byte> <x0 byte> <y0> <x1> <y1>
#define DF _s ( 'D', 'F' ) // draw filled-box; DF <colour byte> <x0 byte> <y0> <x1> <y1>
#define SR _s ( 'S', 'R' ) // sprite receive; SR <id#> <w> <h> <bytestream>
#define CB _s ( 'C', 'B' ) // clear to black
#define DP _s ( 'D', 'P' ) // demo pattern
#define LO _s ( 'L', 'O' ) // debugLog on (usart2 serial)
#define LF _s ( 'L', 'F' ) // debugLog off (usart2 serial)
#define BD _s ( 'B', 'D' ) // bus debug
#define ID _s ( 'i', 'd' ) // i2c debug

void command_queue_run ( void );

#endif
