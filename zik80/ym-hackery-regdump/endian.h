#ifndef h_endian_h
#define h_endian_h

#define SWAP16(num) (num>>8) | (num<<8)

// move byte 3 to byte 0
// move byte 1 to byte 2
// move byte 2 to byte 1
// byte 0 to byte 3
#define SWAP32(num) ((num>>24)&0xff) | ((num<<8)&0xff0000) | ((num>>8)&0xff00) | ((num<<24)&0xff000000)

#endif
