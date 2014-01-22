
typedef unsigned int guint;
typedef unsigned char guint8;

typedef struct {
  guint  	 width;
  guint  	 height;
  guint  	 bytes_per_pixel; /* 2:RGB16, 3:RGB, 4:RGBA */ 
  guint8 	 pixel_data[320 * 200 * 3 + 1];
} gimp_image_t;
