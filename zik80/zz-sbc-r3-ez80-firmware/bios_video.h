
#ifndef h_bios_video_h
#define h_bios_video_h

extern unsigned char vb_saw_vblank;   /* set to 1 when vbl comes in; user may clear it */
extern unsigned char vb_is_vblank;    /* is 1 when vblank is active, 0 when not */

// interupt driven
void vb_enable_vblank_interupt ( void );

#define VB_CLEAR_SAW_VBLANK 1
void vb_wait_for_vblank ( unsigned char clearit );     // waits for vb_saw_vblank to go live, then optionally clears it

#endif
