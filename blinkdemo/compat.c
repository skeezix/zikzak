
// for digitalRead, digitalWrite, pinMode, delayMicroseconds

// install path: /usr/lib/avr/include/avr

#include <stdint.h>
#include <avr/io.h>
#include <avr/portpins.h>
#include <avr/sfr_defs.h>

#include "compat.h"

unsigned char digitalRead ( int pin ) {
}

void digitalWrite ( int pin, unsigned char lowhigh ) {
  return;
}

// ------[ pinMode ]---------------------

#define PROGMEM
#define SREG _SFR_IO8(0x3F)
#define NULL 0
#define NOT_A_PORT NULL
#define NOT_A_PIN NULL

// -- digitalPinToBitMask
#define __LPM_enhanced__(addr)  \
  (__extension__({                \
  uint16_t __addr16 = (uint16_t)(addr); \
  uint8_t __result;           \
    __asm__                     \
    (                           \
    "lpm %0, Z" "\n\t"      \
    : "=r" (__result)       \
    : "z" (__addr16)        \
                                );                          \
    __result;                   \
    }))

#define __LPM(addr)         __LPM_enhanced__(addr)

#define pgm_read_byte_near(address_short) __LPM((uint16_t)(address_short))

#define pgm_read_byte(address_short)    pgm_read_byte_near(address_short)

#define digitalPinToBitMask(P) ( pgm_read_byte( digital_pin_to_bit_mask_PGM + (P) ) )

const uint8_t PROGMEM digital_pin_to_bit_mask_PGM[] = {
  _BV(0), /* 0, port D */
  _BV(1),
  _BV(2),
  _BV(3),
  _BV(4),
  _BV(5),
  _BV(6),
  _BV(7),
  _BV(0), /* 8, port B */
  _BV(1),
  _BV(2),
  _BV(3),
  _BV(4),
  _BV(5),
  _BV(0), /* 14, port C */
  _BV(1),
  _BV(2),
  _BV(3),
  _BV(4),
  _BV(5),
};

// -- digitalPinToPort
#define digitalPinToPort(P) ( pgm_read_byte( digital_pin_to_port_PGM + (P) ) )

#define PA 1
#define PB 2
#define PC 3
#define PD 4
#define PE 5
#define PF 6
#define PG 7
#define PH 8
#define PJ 10
#define PK 11
#define PL 12

const uint8_t PROGMEM digital_pin_to_port_PGM[] = {
  PD, /* 0 */
  PD,
  PD,
  PD,
  PD,
  PD,
  PD,
  PD,
  PB, /* 8 */
  PB,
  PB,
  PB,
  PB,
  PB,
  PC, /* 14 */
  PC,
  PC,
  PC,
  PC,
  PC,
};

// -- more

#define __LPM_word_enhanced__(addr)         \
  (__extension__({                            \
  uint16_t __addr16 = (uint16_t)(addr);   \
  uint16_t __result;                      \
    __asm__                                 \
    (                                       \
    "lpm %A0, Z+"   "\n\t"              \
    "lpm %B0, Z"    "\n\t"              \
    : "=r" (__result), "=z" (__addr16)  \
      : "1" (__addr16)                    \
                                            );                                      \
    __result;                               \
    }))
#define __LPM_word(addr)    __LPM_word_enhanced__(addr)
#define pgm_read_word_near(address_short) __LPM_word((uint16_t)(address_short))

#define pgm_read_word(address_short)    pgm_read_word_near(address_short)

#define portModeRegister(P) ( (volatile uint8_t *)( pgm_read_word( port_to_mode_PGM + (P))) )

const uint16_t PROGMEM port_to_mode_PGM[] = {
  NOT_A_PORT,
  NOT_A_PORT,
  (uint16_t) &DDRB,
  (uint16_t) &DDRC,
  (uint16_t) &DDRD,
};

// --

#define DDRB _SFR_IO8(0x04)
#define DDRC _SFR_IO8(0x07)
#define DDRD _SFR_IO8(0x0A)

#define PORTB _SFR_IO8(0x05)
#define PORTC _SFR_IO8(0x08)
#define PORTD _SFR_IO8(0x0B)

// --

#define portOutputRegister(P) ( (volatile uint8_t *)( pgm_read_word( port_to_output_PGM + (P))) )

const uint16_t PROGMEM port_to_output_PGM[] = {
  NOT_A_PORT,
  NOT_A_PORT,
  (uint16_t) &PORTB,
  (uint16_t) &PORTC,
  (uint16_t) &PORTD,
};

// --

void pinMode ( int pin, pinmode_e mode ) {

  uint8_t bit = digitalPinToBitMask(pin);
  uint8_t port = digitalPinToPort(pin);
  volatile uint8_t *reg, *out;

  if (port == NOT_A_PIN) return;

  // JWS: can I let the optimizer do this?
  reg = portModeRegister(port);
  out = portOutputRegister(port);

  if (mode == INPUT) {
    uint8_t oldSREG = SREG;
    cli();
    *reg &= ~bit;
    *out &= ~bit;
    SREG = oldSREG;
  } else if (mode == INPUT_PULLUP) {
    uint8_t oldSREG = SREG;
    cli();
    *reg &= ~bit;
    *out |= bit;
    SREG = oldSREG;
  } else {
    uint8_t oldSREG = SREG;
    cli();
    *reg |= bit;
    SREG = oldSREG;
  }

  return;
}
// -----------------------

void delayMicroseconds ( int usec ) {
  _delay_us ( usec );
  return;
}
