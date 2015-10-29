#ifndef h_lib_ps2_keyboard_h
#define h_lib_ps2_keyboard_h

#define KEYCODE_ENTER 0x5A              /* middle, above shift.. also the calc-pad bottom right */
#define KEYCODE_BACKSPACE 0x66

// setup
void keyb_setup ( UCHAR open_it );    // >0 to open as gpio port; if 0, leave alone (ie: already opened as UART say)

// internals
void interrupt keyb_clock_isr ( void );
char map_scan_code ( UCHAR data );    // map key code to character
UCHAR read_packet();                  // fetch data from keyboard

extern uint8_t _g_kbd_data;
extern uint8_t _g_char_waiting;

// if key waiting, return >0.. otherwise return 0
// when captured, a key is populated to both mapped code (where possible), and keycode (see table)
unsigned char keyb_fetch_nonblocking ( char *c, unsigned char *keycode );

#endif
