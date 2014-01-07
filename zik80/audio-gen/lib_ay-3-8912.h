
#ifndef h_ay38912_h
#define h_ay38912_h

void ay_set_bc1 ( unsigned char on );
void ay_set_bdir ( unsigned char on );

void ay_mode_latch();
void ay_mode_write();
void ay_mode_inactive();

void ay_set_chA ( int i );
void ay_set_chB ( int i );

#endif
