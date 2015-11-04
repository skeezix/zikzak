#ifndef h_config_h
#define h_config_h

#define STARTUP_WASTE /* if present, will kill a few seconds before really start moving */

//#define RUNMODE_COMMAND_SERIAL          /* listen to GPU Serial console for commands */
//#define RUNMODE_FRAMEBUFFER_FOREVER     /* 60fps, grab framebuffer from ext ram and copy to local, blitting to VGA */
//#define RUNMODE_FRAMEBUFFER_TEST        /* some single assorted test */
#define RUNMODE_FRAMEBUFFER_TEST_CYCLE    /* some series of assorted tests */

#define STARTUP_WAIT_FRAMEREADY_LOW     /* if active, will spin do nothing until frameready from CPU goes low;
                                         * this lets cpu get ready before we hammer the heck out of it
                                         */

#define INVERT_VBLANK /* inverted means vbl active is LOW on VBL line to eZ80; otherwise vbl active is HIGH */
//#define VGA_DMA /* if defined, use DMA for VGA emit; otherwise, brute force */

#define BUS_FRAMEBUFFER /* if defined, will try to use the BUS to find RAM; if not defined, bus is not touched */

#endif
