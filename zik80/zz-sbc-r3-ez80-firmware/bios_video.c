
#include <eZ80.h>
#include <gpio.h>
//#include <intvect.h>

#include "bios_video.h"
#include "bios_cart.h"

volatile unsigned char vb_saw_vblank = 0;
volatile unsigned char vb_is_vblank = 0;

static interrupt void ISR_PC6 ( void ) {

	// default config for gpu is vbl active-low
	
	if ( PC_DR & (1<<6) ) {
		vb_is_vblank = 0;
		return;
	}

	vb_is_vblank = 1;
	vb_saw_vblank = 1;
	
	PC_DR |= (1<<6);

	return;
}

void vb_enable_vblank_interupt ( void ) {

	PORT pc;
	UCHAR err;

	// set handler
	_set_vector ( PORTC6_IVECT, ISR_PC6 );

	// enable interupt on port	
	pc.dr = 1;
	pc.ddr = 0;
	//pc.alt0 = 0;  // does not exist for F93
	pc.alt1 = 0;
	pc.alt2 = 1;
		
	open_PortC ( &pc );
	//control_PortC ( &pc);

	err = setmode_PortC ( PORTPIN_SIX, GPIOMODE_INTERRUPTDUALEDGE );
	
	PC_DR |= (1<<6);

	return;
}

void vb_wait_for_vblank ( unsigned char clearit ) {

	while ( vb_saw_vblank == 0 ) {
		// nada
	}
	
	if ( clearit ) {
		vb_saw_vblank = 0;
	}

	return;
}
