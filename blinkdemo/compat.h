
#ifndef h_pinmode_h
#define h_pinmode_h

typedef enum {
  INPUT = 0,
  OUTPUT = 1,
  INPUT_PULLUP = 2,
} pinmode_e;

unsigned char digitalRead ( int pin );
void digitalWrite ( int pin, unsigned char lowhigh );
void pinMode ( int pin, pinmode_e mode );
void delayMicroseconds ( int usec );

#endif
