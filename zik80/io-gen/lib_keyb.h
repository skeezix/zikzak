
#ifndef h_lib_keyb_h
#define h_lib_keyb_h

#define KEYCODE_ENTER 0x5A              /* middle, above shift.. also the calc-pad bottom right */
#define KEYCODE_BACKSPACE 0x66

void init_keyboard();                   // set us up the bomb

char map_scan_code ( uint8_t data );    // map key code to character
uint8_t read_packet();                  // fetch data from keyboard

// if key waiting, return >0.. otherwise return 0
// when captured, a key is populated to both mapped code (where possible), and keycode (see table above)
unsigned char keyb_fetch_nonblocking ( char *c, unsigned char *keycode );

#endif
