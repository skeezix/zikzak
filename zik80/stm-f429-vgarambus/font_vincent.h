#ifndef h_font_vincent_h
#define h_font_vincent_h

extern unsigned char vincent_data[128][8];

void render_font_8x8 ( unsigned char *fb, unsigned int tx, unsigned int ty, char *text, unsigned char colour );

#endif
