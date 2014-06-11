#ifndef h_command_h
#define h_command_h

#define _s(a,b) (a+(b<<8)) /* squinch together */

#define LD _s ( 'L', 'D' ) // line draw; LD <colour byte> <x0 byte> <y0> <x1> <y1>
#define CB _s ( 'C', 'B' ) // clear to black
#define DP _s ( 'D', 'P' ) // demo pattern

void command_queue_run ( void );

#endif
