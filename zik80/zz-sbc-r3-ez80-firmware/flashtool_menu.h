
#ifndef h_flashtool_menu_h
#define h_flashtool_menu_h

#define MAXDATASIZE 256

void flashtool_serial_forever ( void );

// internal..
typedef enum {
  ss_ready = 0,
  ss_cmd_build,
  ss_ohai,
  ss_echo,
  ss_receive,
  ss_burn,
  ss_dump,
  ss_charecho,
  ss_buffer,
  ss_help,
  ss_format,
  ss_testbuf
} serial_state_e;

#endif
