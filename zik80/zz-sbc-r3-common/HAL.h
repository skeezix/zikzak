
/* zikzak sbc rev3
 * Jeff Mitchell 2014
 */
 
 // HAL.h is the basic hardware definitions

// zz sbc rev3:
// ROM cart spec limits cartridge to 4MB address space
 
// Chip Selects:
// CS0 e8 0 0c0000 0dffff -> ram (128KB)
//                 13FFFF -> ram (512KB)
// CS1 e8 0 1c0000 5bffff -> rom - space is 4MB regardless of cart size
// CS2 e8 0 5c0000 5cffff -> audio
//
// 2.457MHz oscilator in place

// vblank
// STM PB12 -> eZ80 PC6      vbl active low (though is configurable..)

#ifndef h_hal_h
#define h_hal_h

/* System/Board defines
 */

#define EXTRAM_BASE  0x0C0000   /* address where physical external RAM starts */
#define EXTRAM_END   0x13FFFF   /* address where physical external RAM ends */

#define VRAM_BASE (EXTRAM_BASE) /* where to place VRAM in RAM */

/* Video modes
 */
// textmode
// textsprite
// spriteonly
// rawframebuffer
// --> perhaps rle option, or delta (changed regions only) or something?

/* Text Mode defines
 */
#define TM_PAGE_STRIDE 768                          /* size in cells (bytes) of a page */
#define TM_VRAM_FB (VRAM_BASE+0)                    /* where to place text cell in VRAM */
#define TM_VRAM_ATTR (TM_VRAM_FB+TM_PAGE_STRIDE)    /* where to place attribute cell in VRAM */
#define TM_TEXTLINE_STRIDE 32                       /* 32 chars/bytes across */
#define TM_TEXTCOL_STRIDE 24                        /* 24 chars/bytyes tall */
// sprite list is (say) 32spr*3b == 96b long
#define TM_SPRITE_BASE (TM_VRAM_ATTR+TM_PAGE_STRIDE)       /* where to place sprite block */
#define TM_SPRITE_ACTIVE (TM_SPRITE_BASE)                  /* where to place sprite current counter */
#define TM_SPRITE_LIST_BASE (TM_SPRITE_ACTIVE+1)           /* where to place sprite list in VRAM */
#define TM_SPRITE_MAX      32                       /* number of sprites to allow */
#define TM_SPRITE_CELL_STRIDE 4                     /* size in bytes of sprite cell */

typedef struct {
  unsigned char x;
  unsigned char y;
  unsigned char f;                                  /* flags */
#define TM_SPRITE_SHOW (1<<0)                       /* flag bit 1: default sprite hide, enable to show sprite */
  unsigned char _u;                                 /* user value */
} tm_sprite_cell_t;


#endif
