#ifndef h_ym5header_h
#define h_ym5header_h

/*
 0	4	DWORD		ID of YM4 format. ('YM4!')
 4	8	String[8]	Check String ('LeOnArD!')
 12	4	DWORD		Nb of valid VBL of the file
 16	4	DWORD		Song attributes (see below)
 20	4	DWORD		Nb of digi-drum sample (can be 0)
 22	4	DWORD		YM2149 External frequency in Hz (2000000 = ATARI-ST, 1000000 = AMSTRAD CPC)
 26	2	WORD		Player frequency in Hz (Ex: 50Hz for almost every player)
 28	4	DWORD		VBL number to loop the song (0 is default)
 32	2	WORD		Size (in bytes) of future additinal data (must be 0 for now)
*/

typedef struct {
  char id [ 4 ];                // YM5!
  char check [ 8 ];             // LeOnArD!
  uint32_t vbl_count;
  uint32_t attrib;
  uint32_t drum_count;
  uint32_t ym_freq;
  uint16_t player_freq;         // likely 50 (as in Hz)
  uint32_t vbl_loop;            // 0 default
  uint16_t size_extras;         // should be 0 for YM5
} ym5_header_t;

#endif
