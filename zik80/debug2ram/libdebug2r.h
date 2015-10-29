
#ifndef h_libdebug2r_h
#define h_libdebug2r_h

/* simple debug logging to a RAM array
 */
#ifndef NULL
#define NULL (void*)0
#endif

#define D2R_ON 1 /* if defined, debug code will be present; if not, no footprint at all */

#ifdef D2R_ON

#define D2R_MAX 40                 /* max number of debug items retained */
#define D2R_HANDLE_T char**
extern char *_d2r_literals [ D2R_MAX ];
extern unsigned char _d2r_head;
extern unsigned char _d2r_tail;

// append to log
//   logl -> append a "" literal string
//   logs -> append a possibly manufactured string (or literal); it will be copied
#define D2R_LOGLITERAL(s) _d2r_logliteral ( s )
#define D2R_LOGL(s) D2R_LOGLITERAL(s)
#define D2R_LOGS(s) _d2r_logstring ( s )

// accessors
//   fetch() -> given a handle, get the string
//              -> if fetchliteral should be valid, yet returns 0x00 string, then check D2R_FETCHSTRING(h) instead
//   first() -> get handle to first
//   next() -> given a handle, find the next one or return null if no more (at end)
#define D2R_FETCHLITERAL(h)  _d2r_fetchliteral ( h )
#define D2R_FIRST()          _d2r_first()
#define D2R_NEXT(h)          _d2r_next ( h )
#define D2R_FETCHSTRING(h)   _d2r_fetchstring ( h )

void _d2r_logliteral ( char *s );
void _d2r_logstring ( char *s );
char *_d2r_fetchliteral ( D2R_HANDLE_T h );
D2R_HANDLE_T _d2r_first ( void );
D2R_HANDLE_T _d2r_next ( D2R_HANDLE_T h );

#else

// NOPs, so code can compile easily without debug
#define D2R_HANDLE_T char**
#define D2R_LOGLITERAL(x)
#define D2R_LOGL(x)
#define D2R_LOGS(s)
#define D2R_FETCH(x)
#define D2R_FIRST()
#define D2R_NEXT(x)

#endif




#endif
