
#ifndef h_zzcartmap_h
#define h_zzcartmap_h

typedef struct {
  uint32_t cart_code;
  uint16_t cart_type;
  uint16_t cart_mode; // z80 or eZ80? etc
  uint16_t cart_size; // 16KB * size-value to get actual max size
} zzcartheader_t;

#endif
