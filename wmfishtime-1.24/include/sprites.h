/*  GIMP header image file format (INDEXED): /tmp/fishmon-work/sprites.h  */

static unsigned int width = 35;
static unsigned int height = 208;

/*  Call this macro repeatedly.  After each use, the pixel data can be extracted  */

#define HEADER_PIXEL(data,pixel) {\
  pixel[0] = header_data_cmap[(unsigned char)data[0]][0]; \
  pixel[1] = header_data_cmap[(unsigned char)data[0]][1]; \
  pixel[2] = header_data_cmap[(unsigned char)data[0]][2]; \
  data ++; }

static unsigned char header_data_cmap[256][3] = {
	{ 55, 95,135},
	{  0,  0,  0},
	{255,255,255},
	{199,119, 43},
	{199,159, 43},
	{  0,170,  0},
	{  7,115,  0},
	{171,186,198},
	{255,  0,  0},
	{191,  0,  0},
	{119,199, 43},
	{159,199, 43},
	{ 47,107,135},
	{ 75,131,159},
	{ 23, 83,111},
	{111,159,183},
	{199,199, 43},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255},
	{255,255,255}
	};
static char header_data[] = {
	0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,
	0,2,0,0,0,0,0,0,0,0,1,1,0,0,0,0,
	0,0,0,
	0,0,0,0,0,0,1,3,3,1,0,0,0,0,0,0,
	0,2,0,0,0,0,0,0,0,1,3,3,1,0,0,0,
	0,0,0,
	0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,
	0,2,0,0,0,0,0,0,0,0,1,1,1,1,1,0,
	0,0,0,
	0,0,1,1,4,4,4,4,4,1,1,0,0,0,0,0,
	0,2,0,0,0,0,0,0,1,1,4,4,4,4,4,1,
	1,0,0,
	0,1,4,4,4,4,4,4,4,4,4,1,0,0,1,1,
	0,2,0,1,1,0,0,1,4,4,4,4,4,4,4,4,
	4,1,0,
	0,1,4,4,1,1,4,4,4,4,4,1,0,1,3,3,
	1,2,1,3,3,1,0,1,4,4,4,4,4,1,1,4,
	4,1,0,
	1,4,4,4,1,1,4,4,4,4,4,4,1,3,3,3,
	1,2,1,3,3,3,1,4,4,4,4,4,4,1,1,4,
	4,4,1,
	1,4,4,4,4,4,4,4,4,4,4,4,1,3,3,1,
	0,2,0,1,3,3,1,4,4,4,4,4,4,4,4,4,
	4,4,1,
	1,4,4,4,4,4,4,4,4,4,4,4,1,3,3,3,
	1,2,1,3,3,3,1,4,4,4,4,4,4,4,4,4,
	4,4,1,
	0,1,4,4,4,4,1,4,4,4,4,1,0,1,3,3,
	1,2,1,3,3,1,0,1,4,4,4,4,1,4,4,4,
	4,1,0,
	0,1,4,4,1,1,4,4,4,1,1,1,0,0,1,1,
	0,2,0,1,1,0,0,1,1,1,4,4,4,1,1,4,
	4,1,0,
	0,0,1,1,4,4,4,4,1,3,3,1,0,0,0,0,
	0,2,0,0,0,0,0,1,3,3,1,4,4,4,4,1,
	1,0,0,
	0,0,0,0,1,1,1,1,0,1,1,0,0,0,0,0,
	0,2,0,0,0,0,0,0,1,1,0,1,1,1,1,0,
	0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,
	2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
	2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
	2,2,2,
	0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,
	0,2,0,0,0,0,0,0,0,0,1,1,0,0,0,0,
	0,0,0,
	0,0,0,0,0,0,1,3,3,1,0,0,0,0,0,0,
	0,2,0,0,0,0,0,0,0,1,3,3,1,0,0,0,
	0,0,0,
	0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,
	0,2,0,0,0,0,0,0,0,0,1,1,1,1,1,0,
	0,0,0,
	0,0,1,1,4,4,4,4,4,1,1,0,0,0,0,0,
	0,2,0,0,0,0,0,0,1,1,4,4,4,4,4,1,
	1,0,0,
	0,1,4,4,4,4,4,4,4,4,4,1,0,0,1,0,
	0,2,0,0,1,0,0,1,4,4,4,4,4,4,4,4,
	4,1,0,
	0,1,4,4,1,1,4,4,4,4,4,1,0,1,3,1,
	0,2,0,1,3,1,0,1,4,4,4,4,4,1,1,4,
	4,1,0,
	1,4,4,4,1,1,4,4,4,4,4,4,1,3,3,1,
	0,2,0,1,3,3,1,4,4,4,4,4,4,1,1,4,
	4,4,1,
	1,4,4,4,4,4,4,4,4,4,4,4,1,3,1,0,
	0,2,0,0,1,3,1,4,4,4,4,4,4,4,4,4,
	4,4,1,
	1,4,4,4,4,4,4,4,4,4,4,4,1,3,3,1,
	0,2,0,1,3,3,1,4,4,4,4,4,4,4,4,4,
	4,4,1,
	0,1,4,4,4,4,1,4,4,4,4,1,0,1,3,1,
	0,2,0,1,3,1,0,1,4,4,4,4,1,4,4,4,
	4,1,0,
	0,1,4,4,1,1,4,4,4,1,1,1,0,0,1,0,
	0,2,0,0,1,0,0,1,1,1,4,4,4,1,1,4,
	4,1,0,
	0,0,1,1,4,4,4,4,1,3,3,1,0,0,0,0,
	0,2,0,0,0,0,0,1,3,3,1,4,4,4,4,1,
	1,0,0,
	0,0,0,0,1,1,1,1,0,1,1,0,0,0,0,0,
	0,2,0,0,0,0,0,0,1,1,0,1,1,1,1,0,
	0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,
	2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
	2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
	2,2,2,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,
	0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,
	0,2,0,0,0,0,0,0,0,0,0,1,1,0,0,0,
	0,0,0,
	0,0,0,0,1,1,3,3,1,0,0,0,0,0,0,0,
	0,2,0,0,0,0,0,0,0,0,1,3,3,1,1,0,
	0,0,0,
	0,0,1,1,4,4,1,1,4,1,1,0,0,0,0,0,
	0,2,0,0,0,0,0,0,1,1,4,1,1,4,4,1,
	1,0,0,
	0,1,4,4,4,4,4,4,4,4,4,1,0,0,1,0,
	0,2,0,0,1,0,0,1,4,4,4,4,4,4,4,4,
	4,1,0,
	0,1,4,4,1,1,4,4,4,4,4,1,0,1,3,1,
	0,2,0,1,3,1,0,1,4,4,4,4,4,1,1,4,
	4,1,0,
	1,4,4,4,1,1,4,4,4,4,4,4,1,3,3,1,
	0,2,0,1,3,3,1,4,4,4,4,4,4,1,1,4,
	4,4,1,
	1,4,4,4,4,4,4,4,4,4,4,4,1,3,1,0,
	0,2,0,0,1,3,1,4,4,4,4,4,4,4,4,4,
	4,4,1,
	1,4,4,4,4,4,4,4,4,4,4,4,1,3,3,1,
	0,2,0,1,3,3,1,4,4,4,4,4,4,4,4,4,
	4,4,1,
	0,1,4,4,4,4,1,4,4,4,4,1,0,1,3,1,
	0,2,0,1,3,1,0,1,4,4,4,4,1,4,4,4,
	4,1,0,
	0,1,4,4,1,1,4,4,4,4,4,1,0,0,1,0,
	0,2,0,0,1,0,0,1,4,4,4,4,4,1,1,4,
	4,1,0,
	0,0,1,1,4,4,4,4,4,1,1,0,0,0,0,0,
	0,2,0,0,0,0,0,0,1,1,4,4,4,4,4,1,
	1,0,0,
	0,0,0,0,1,1,1,1,1,3,3,1,0,0,0,0,
	0,2,0,0,0,0,0,1,3,3,1,1,1,1,1,0,
	0,0,0,
	0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,
	0,2,0,0,0,0,0,0,1,1,0,0,0,0,0,0,
	0,0,0,
	2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
	2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
	2,2,2,
	0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,
	0,2,0,0,0,0,0,0,0,0,1,1,0,0,0,0,
	0,0,0,
	0,0,0,0,0,0,1,3,3,1,0,0,0,0,0,0,
	0,2,0,0,0,0,0,0,0,1,3,3,1,0,0,0,
	0,0,0,
	0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,
	0,2,0,0,0,0,0,0,0,1,1,1,1,1,0,0,
	0,0,0,
	0,0,0,1,1,4,4,4,4,4,1,1,0,0,0,0,
	0,2,0,0,0,0,0,1,1,4,4,4,4,4,1,1,
	0,0,0,
	0,0,1,4,4,4,4,4,4,4,4,4,1,0,0,0,
	0,2,0,0,0,0,1,4,4,4,4,4,4,4,4,4,
	1,0,0,
	0,0,1,1,4,1,1,4,4,4,4,4,1,0,1,0,
	0,2,0,0,1,0,1,4,4,4,4,4,1,1,4,1,
	1,0,0,
	0,1,1,1,4,1,1,4,4,4,4,4,4,1,3,1,
	0,2,0,1,3,1,4,4,4,4,4,4,1,1,4,1,
	1,1,0,
	0,1,4,4,4,4,4,4,4,4,4,4,4,1,1,0,
	0,2,0,0,1,1,4,4,4,4,4,4,4,4,4,4,
	4,1,0,
	0,1,4,4,4,4,4,4,4,4,4,4,4,1,3,1,
	0,2,0,1,3,1,4,4,4,4,4,4,4,4,4,4,
	4,1,0,
	0,0,1,4,4,4,1,4,4,4,4,4,1,0,1,0,
	0,2,0,0,1,0,1,4,4,4,4,4,1,4,4,4,
	1,0,0,
	0,0,1,1,1,1,4,4,4,4,4,4,1,0,0,0,
	0,2,0,0,0,0,1,4,4,4,4,4,4,1,1,1,
	1,0,0,
	0,0,0,0,1,4,4,4,4,4,1,1,0,0,0,0,
	0,2,0,0,0,0,0,1,1,4,4,4,4,4,1,0,
	0,0,0,
	0,0,0,0,0,1,1,1,1,1,3,3,1,0,0,0,
	0,2,0,0,0,0,1,3,3,1,1,1,1,1,0,0,
	0,0,0,
	0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,
	0,2,0,0,0,0,0,1,1,0,0,0,0,0,0,0,
	0,0,0,
	2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
	2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
	2,2,2,
	0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,
	0,2,0,0,0,0,0,0,0,0,1,1,0,0,0,0,
	0,0,0,
	0,0,0,0,0,0,0,1,3,3,1,0,0,0,0,0,
	0,2,0,0,0,0,0,0,0,1,3,3,1,0,0,0,
	0,0,0,
	0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,
	0,2,0,0,0,0,0,0,1,1,1,1,1,0,0,0,
	0,0,0,
	0,0,0,0,1,1,4,4,4,4,4,1,1,0,0,0,
	0,2,0,0,0,0,1,1,4,4,4,4,4,1,1,0,
	0,0,0,
	0,0,0,1,4,4,4,4,4,4,4,4,4,1,0,0,
	0,2,0,0,0,1,4,4,4,4,4,4,4,4,4,1,
	0,0,0,
	0,0,0,1,4,4,1,1,4,1,1,4,4,1,0,0,
	0,2,0,0,0,1,4,4,1,1,4,1,1,4,4,1,
	0,0,0,
	0,0,1,4,4,4,1,1,4,1,1,4,4,4,1,0,
	0,2,0,0,1,4,4,4,1,1,4,1,1,4,4,4,
	1,0,0,
	0,0,1,4,4,4,4,4,4,4,4,4,4,4,1,0,
	0,2,0,0,1,4,4,4,4,4,4,4,4,4,4,4,
	1,0,0,
	0,0,1,4,4,4,4,4,4,4,4,4,4,4,1,0,
	0,2,0,0,1,4,4,4,4,4,4,4,4,4,4,4,
	1,0,0,
	0,0,0,1,4,4,1,4,4,4,1,4,4,1,0,0,
	0,2,0,0,0,1,4,4,1,4,4,4,1,4,4,1,
	0,0,0,
	0,0,0,1,4,4,4,1,1,1,4,4,4,1,0,0,
	0,2,0,0,0,1,4,4,4,1,1,1,4,4,4,1,
	0,0,0,
	0,0,0,0,1,1,4,4,4,4,4,1,1,0,0,0,
	0,2,0,0,0,0,1,1,4,4,4,4,4,1,1,0,
	0,0,0,
	0,0,0,1,3,3,1,1,1,1,1,3,3,1,0,0,
	0,2,0,0,0,1,3,3,1,1,1,1,1,3,3,1,
	0,0,0,
	0,0,0,0,1,1,0,0,0,0,0,1,1,0,0,0,
	0,2,0,0,0,0,1,1,0,0,0,0,0,1,1,0,
	0,0,0,
	2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
	2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
	2,2,2,
	0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,
	0,2,0,0,0,0,0,0,0,0,1,1,0,0,0,0,
	0,0,0,
	0,0,0,0,0,0,1,3,3,1,0,0,0,0,0,0,
	0,2,0,0,0,0,0,0,0,1,3,3,1,0,0,0,
	0,0,0,
	0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,
	0,2,0,0,0,0,0,0,0,0,1,1,1,1,1,0,
	0,0,0,
	0,0,1,1,4,4,4,4,4,1,1,0,0,0,0,0,
	0,2,0,0,0,0,0,0,1,1,4,4,4,4,4,1,
	1,0,0,
	0,1,4,4,4,4,4,4,4,4,4,1,0,0,1,1,
	0,2,0,1,1,0,0,1,4,4,4,4,4,4,4,4,
	4,1,0,
	0,1,4,4,1,1,4,4,4,4,4,1,0,1,3,3,
	1,2,1,3,3,1,0,1,4,4,4,4,4,1,1,4,
	4,1,0,
	1,4,4,4,1,1,4,4,4,4,4,4,1,3,3,3,
	1,2,1,3,3,3,1,4,4,4,4,4,4,1,1,4,
	4,4,1,
	1,4,4,4,4,4,4,4,4,4,4,4,1,3,3,1,
	0,2,0,1,3,3,1,4,4,4,4,4,4,4,4,4,
	4,4,1,
	1,4,4,4,4,4,1,4,4,4,4,4,1,3,3,3,
	1,2,1,3,3,3,1,4,4,4,4,4,1,4,4,4,
	4,4,1,
	0,1,4,4,1,1,0,1,4,4,4,1,0,1,3,3,
	1,2,1,3,3,1,0,1,4,4,4,1,0,1,1,4,
	4,1,0,
	0,0,1,1,0,0,1,4,4,1,1,1,0,0,1,1,
	0,2,0,1,1,0,0,1,1,1,4,4,1,0,0,1,
	1,0,0,
	0,0,0,0,0,1,4,4,1,3,3,1,0,0,0,0,
	0,2,0,0,0,0,0,1,3,3,1,4,4,1,0,0,
	0,0,0,
	0,0,0,0,0,1,1,1,0,1,1,0,0,0,0,0,
	0,2,0,0,0,0,0,0,1,1,0,1,1,1,0,0,
	0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,
	2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
	2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
	2,2,2,
	0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,
	0,2,0,0,0,0,0,0,0,0,1,1,0,0,0,0,
	0,0,0,
	0,0,0,0,0,0,1,3,3,1,0,0,0,0,0,0,
	0,2,0,0,0,0,0,0,0,1,3,3,1,0,0,0,
	0,0,0,
	0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,
	0,2,0,0,0,0,0,0,0,0,1,1,1,1,1,0,
	0,0,0,
	0,0,1,1,4,4,4,4,4,1,1,0,0,0,0,0,
	0,2,0,0,0,0,0,0,1,1,4,4,4,4,4,1,
	1,0,0,
	0,1,4,4,4,4,4,4,4,4,4,1,0,0,1,1,
	0,2,0,1,1,0,0,1,4,4,4,4,4,4,4,4,
	4,1,0,
	0,1,4,4,1,1,4,4,4,4,4,1,0,1,3,3,
	1,2,1,3,3,1,0,1,4,4,4,4,4,1,1,4,
	4,1,0,
	1,4,4,4,1,1,4,4,4,4,4,4,1,3,3,3,
	1,2,1,3,3,3,1,4,4,4,4,4,4,1,1,4,
	4,4,1,
	1,4,4,4,4,4,4,4,4,4,4,4,1,3,3,1,
	0,2,0,1,3,3,1,4,4,4,4,4,4,4,4,4,
	4,4,1,
	1,4,4,4,4,1,1,4,4,4,4,4,1,3,3,3,
	1,2,1,3,3,3,1,4,4,4,4,4,1,1,4,4,
	4,4,1,
	0,1,4,1,1,0,0,1,4,4,4,1,0,1,3,3,
	1,2,1,3,3,1,0,1,4,4,4,1,0,0,1,1,
	4,1,0,
	0,0,1,0,0,0,1,4,4,1,1,1,0,0,1,1,
	0,2,0,1,1,0,0,1,1,1,4,4,1,0,0,0,
	1,0,0,
	0,0,0,0,0,1,4,4,1,3,3,1,0,0,0,0,
	0,2,0,0,0,0,0,1,3,3,1,4,4,1,0,0,
	0,0,0,
	0,0,0,0,0,1,1,1,0,1,1,0,0,0,0,0,
	0,2,0,0,0,0,0,0,1,1,0,1,1,1,0,0,
	0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,
	2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
	2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
	2,2,2,
	0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,
	0,2,0,0,0,0,0,0,0,0,1,1,0,0,0,0,
	0,0,0,
	0,0,0,0,0,0,1,3,3,1,0,0,0,0,0,0,
	0,2,0,0,0,0,0,0,0,1,3,3,1,0,0,0,
	0,0,0,
	0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,
	0,2,0,0,0,0,0,0,0,0,1,1,1,1,1,0,
	0,0,0,
	0,0,1,1,4,4,4,4,4,1,1,0,0,0,0,0,
	0,2,0,0,0,0,0,0,1,1,4,4,4,4,4,1,
	1,0,0,
	0,1,4,4,4,4,4,4,4,4,4,1,0,0,1,1,
	0,2,0,1,1,0,0,1,4,4,4,4,4,4,4,4,
	4,1,0,
	0,1,4,4,1,1,4,4,4,4,4,1,0,1,3,3,
	1,2,1,3,3,1,0,1,4,4,4,4,4,1,1,4,
	4,1,0,
	1,4,4,4,1,1,4,4,4,4,4,4,1,3,3,3,
	1,2,1,3,3,3,1,4,4,4,4,4,4,1,1,4,
	4,4,1,
	1,4,4,4,4,4,4,4,4,4,4,4,1,3,3,1,
	0,2,0,1,3,3,1,4,4,4,4,4,4,4,4,4,
	4,4,1,
	1,4,4,1,1,1,4,4,4,4,4,4,1,3,3,3,
	1,2,1,3,3,3,1,4,4,4,4,4,4,1,1,1,
	4,4,1,
	0,1,1,0,0,0,1,4,4,4,4,1,0,1,3,3,
	1,2,1,3,3,1,0,1,4,4,4,4,1,0,0,0,
	1,1,0,
	0,0,0,0,0,0,1,4,4,1,1,1,0,0,1,1,
	0,2,0,1,1,0,0,1,1,1,4,4,1,0,0,0,
	0,0,0,
	0,0,0,0,0,1,4,4,1,3,3,1,0,0,0,0,
	0,2,0,0,0,0,0,1,3,3,1,4,4,1,0,0,
	0,0,0,
	0,0,0,0,0,1,1,1,0,1,1,0,0,0,0,0,
	0,2,0,0,0,0,0,0,1,1,0,1,1,1,0,0,
	0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,
	2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
	2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
	2,2,2,
	0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,
	0,2,0,0,0,0,0,0,0,0,1,0,0,0,0,0,
	0,0,0,
	0,0,0,0,0,0,0,0,1,5,1,0,0,0,0,0,
	0,2,0,0,0,0,0,0,0,1,5,1,0,0,0,0,
	0,0,0,
	0,0,0,0,0,0,0,0,1,5,1,0,0,0,0,0,
	0,2,0,0,0,0,0,0,0,0,1,5,1,0,0,0,
	0,0,0,
	0,0,0,0,1,0,0,0,1,5,1,0,0,0,0,0,
	0,2,0,0,0,0,1,0,0,0,1,5,1,0,0,0,
	0,0,0,
	0,0,0,1,5,1,0,0,1,5,5,1,0,0,0,0,
	0,2,0,0,0,1,5,1,0,0,1,5,5,1,0,0,
	0,0,0,
	0,0,1,5,5,1,0,0,1,5,1,0,1,0,0,0,
	0,2,0,0,1,5,5,1,0,0,1,5,1,0,1,0,
	0,0,0,
	0,0,0,1,6,5,1,1,5,5,1,1,5,1,0,0,
	0,2,0,0,0,1,5,5,1,1,5,5,1,1,5,1,
	0,0,0,
	0,0,1,6,5,1,0,1,5,1,0,1,5,5,1,0,
	0,2,0,0,1,6,5,1,0,1,5,1,0,1,5,5,
	1,0,0,
	0,0,1,6,5,1,1,6,5,5,1,6,5,5,1,0,
	0,2,0,0,1,6,5,1,1,6,5,5,1,6,5,5,
	1,0,0,
	0,0,1,6,5,5,1,0,1,6,5,1,6,5,5,1,
	0,2,0,1,6,5,5,1,0,1,6,5,6,1,5,5,
	1,0,0,
	0,0,1,6,5,5,5,1,6,5,5,5,5,5,1,0,
	0,2,0,1,6,5,5,5,1,1,6,5,5,5,5,1,
	0,0,0,
	0,0,0,0,1,6,5,5,5,5,5,5,5,5,5,1,
	0,2,0,0,1,6,5,5,5,5,5,5,5,5,5,1,
	0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,
	0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,
	0,2,0,0,0,0,0,0,0,1,0,1,0,0,0,0,
	0,0,0,
	2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
	2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
	2,2,2,
	0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,
	0,2,0,0,0,0,0,0,0,0,1,0,0,0,0,0,
	0,0,0,
	0,0,0,0,0,0,0,1,5,1,0,0,0,0,0,0,
	0,2,0,0,0,0,0,0,0,1,5,6,1,0,0,0,
	0,0,0,
	0,0,0,0,0,0,0,0,1,5,1,0,0,0,0,0,
	0,2,0,0,0,0,0,0,0,0,6,5,5,1,0,0,
	0,0,0,
	0,0,0,0,1,0,0,0,1,5,1,0,0,0,0,0,
	0,2,0,0,0,0,0,1,0,0,1,6,5,1,0,0,
	0,0,0,
	0,0,0,1,5,1,0,0,1,5,5,1,0,0,0,0,
	0,2,0,0,0,0,1,5,1,0,0,1,5,5,1,0,
	0,0,0,
	0,0,1,5,5,1,0,0,1,5,1,0,1,0,0,0,
	0,2,0,0,0,1,5,5,1,0,0,1,5,1,0,1,
	0,0,0,
	0,0,0,1,5,5,1,1,6,5,1,1,5,1,0,0,
	0,2,0,0,0,0,1,5,5,1,1,5,5,6,1,5,
	1,0,0,
	0,0,1,6,5,1,0,1,6,1,0,1,5,5,1,0,
	0,2,0,0,0,1,6,5,1,0,1,5,1,0,1,5,
	5,1,0,
	0,0,1,6,5,1,1,6,5,5,1,1,5,5,1,0,
	0,2,0,0,6,1,5,5,1,1,5,5,5,1,1,5,
	5,1,0,
	0,1,6,5,5,1,0,1,6,5,1,1,5,5,1,0,
	0,2,0,1,6,6,5,1,0,1,6,5,1,1,5,5,
	1,0,0,
	0,1,6,5,5,5,1,1,6,5,5,5,5,1,1,0,
	0,2,0,1,6,5,5,5,1,6,5,5,5,6,5,1,
	0,0,0,
	0,0,1,6,5,5,5,5,5,5,5,5,5,1,0,0,
	0,2,0,1,6,5,5,5,5,5,5,5,5,5,1,0,
	0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,
	0,0,0,0,0,0,1,0,1,0,1,0,0,0,0,0,
	0,2,0,0,0,1,0,1,0,1,0,1,0,0,0,0,
	0,0,0,
	2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
	2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
	2,2,2,
	0,0,0,0,0,0,0,0,0,6,0,0,0,0,0,0,
	0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,
	0,0,0,0,0,0,0,0,1,5,1,0,0,0,0,0,
	0,2,0,0,0,0,0,0,0,0,0,0,7,7,0,0,
	0,0,0,
	0,0,0,0,0,0,0,0,0,1,5,1,0,0,0,0,
	0,2,0,0,0,0,0,0,0,0,0,7,8,9,7,0,
	0,0,0,
	0,0,0,0,0,0,1,0,0,0,1,5,1,0,0,0,
	0,2,0,0,0,0,0,0,0,0,0,7,8,9,7,7,
	0,0,0,
	0,0,0,0,0,1,5,1,0,0,1,5,5,1,0,0,
	0,2,0,0,0,0,0,0,0,0,0,7,8,9,7,0,
	0,0,0,
	0,0,0,0,1,6,5,1,0,0,1,5,1,0,1,0,
	0,2,0,0,0,0,0,0,0,0,0,7,8,9,7,0,
	0,0,0,
	0,0,0,0,0,1,5,5,1,1,6,5,1,1,5,1,
	0,2,0,0,0,0,0,0,0,0,0,7,8,9,7,0,
	0,0,0,
	0,0,0,0,1,6,5,1,0,1,5,1,0,1,5,5,
	1,2,0,0,0,0,0,0,0,0,0,7,8,9,7,0,
	0,0,0,
	0,0,0,1,6,5,1,1,6,5,5,6,1,5,5,1,
	0,2,0,0,0,0,0,0,0,0,0,7,8,9,7,0,
	0,0,0,
	0,0,1,6,5,5,1,0,1,5,5,1,6,5,5,1,
	0,2,0,0,0,0,0,0,0,0,0,7,8,9,7,0,
	0,0,0,
	0,1,6,5,5,5,1,1,6,5,5,5,5,1,0,0,
	0,2,0,0,0,0,0,0,0,0,0,7,8,9,7,7,
	0,0,0,
	0,1,6,5,5,5,5,5,5,5,5,5,1,0,0,0,
	0,2,0,0,0,0,0,0,0,0,0,7,8,9,7,0,
	0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,2,0,0,0,0,0,0,0,0,0,7,8,9,7,0,
	0,0,0,
	0,0,0,1,0,1,0,1,0,1,0,1,0,0,0,0,
	0,2,0,0,0,0,0,0,0,0,0,7,8,9,7,0,
	0,0,0,
	2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
	2,2,0,0,0,0,0,0,0,0,0,7,8,9,7,0,
	0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,2,0,0,0,0,0,0,0,0,0,7,8,9,7,0,
	0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,2,0,0,0,0,0,0,0,0,0,7,8,9,7,0,
	0,0,0,
	0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,
	0,2,0,0,0,0,0,0,0,0,0,7,8,9,7,7,
	0,0,0,
	0,0,0,0,0,0,0,1,9,1,0,0,0,0,0,0,
	0,2,0,0,0,0,0,0,0,0,0,7,8,9,7,0,
	0,0,0,
	0,0,0,1,1,0,0,1,8,1,0,0,1,1,0,0,
	0,2,0,0,0,0,0,0,0,0,0,7,8,9,7,0,
	0,0,0,
	0,0,1,8,8,1,1,9,8,1,1,1,8,9,1,0,
	0,2,0,0,0,0,0,0,0,0,0,7,8,9,7,0,
	0,0,0,
	0,0,0,1,9,8,8,8,8,8,8,9,9,1,0,0,
	0,2,0,0,0,0,0,0,0,0,0,7,8,9,7,0,
	0,0,0,
	0,0,0,0,1,1,1,8,8,8,1,1,1,0,0,0,
	0,2,0,0,0,0,0,0,0,0,0,7,8,9,7,0,
	0,0,0,
	0,0,0,0,0,1,8,9,1,8,9,1,0,0,0,0,
	0,2,0,0,0,0,0,0,0,0,0,7,8,9,7,0,
	0,0,0,
	0,0,0,0,1,8,9,1,0,1,8,9,1,0,0,0,
	0,2,0,0,0,0,0,0,0,0,0,7,8,9,7,0,
	0,0,0,
	0,0,0,0,1,9,9,1,0,1,9,9,1,0,0,0,
	0,2,0,0,0,0,0,0,0,0,0,7,8,9,7,7,
	0,0,0,
	0,0,0,0,0,1,1,0,0,0,1,1,0,0,0,0,
	0,2,0,0,0,0,0,0,0,0,0,7,8,9,7,0,
	0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,2,0,0,0,0,0,0,0,0,0,7,8,9,7,0,
	0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,2,0,0,0,0,0,0,0,0,0,7,8,9,7,0,
	0,0,0,
	2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
	2,2,0,0,0,0,0,0,0,0,0,7,8,9,7,7,
	0,0,0,
	0,0,0,1,1,1,0,0,0,0,0,2,0,0,0,0,
	0,2,0,0,0,0,0,0,0,0,0,7,8,9,7,0,
	0,0,0,
	0,0,1,10,10,10,1,0,1,1,0,2,0,0,0,0,
	0,2,0,0,0,0,0,0,0,0,0,7,8,9,7,0,
	0,0,0,
	0,1,10,10,10,10,10,1,10,10,1,2,0,0,0,0,
	0,2,0,0,0,0,0,0,0,0,7,9,8,9,9,7,
	0,0,0,
	1,10,10,1,10,10,10,10,10,1,0,2,0,0,0,0,
	0,2,0,0,0,0,0,0,0,0,7,8,8,8,9,7,
	0,0,0,
	1,10,10,10,10,10,10,10,10,1,0,2,0,0,0,0,
	0,2,0,0,0,0,0,0,0,0,0,7,8,8,7,0,
	0,0,0,
	0,1,10,10,10,10,10,1,10,10,1,2,0,0,0,0,
	0,2,0,0,0,0,0,0,0,0,0,0,7,7,0,0,
	0,0,0,
	0,0,1,10,10,10,1,0,1,1,0,2,0,0,0,0,
	0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,
	0,0,0,1,1,1,0,0,0,0,0,2,0,0,0,0,
	0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,
	2,2,2,2,2,2,2,2,2,2,2,2,0,0,0,0,
	0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,
	0,0,0,1,1,1,0,0,0,0,0,2,0,0,0,0,
	0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,
	0,0,1,11,11,11,1,0,1,1,0,2,0,0,0,0,
	0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,
	0,1,11,11,11,11,11,1,11,11,1,2,0,0,0,0,
	0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,
	1,11,11,1,11,11,11,11,11,1,0,2,0,0,0,0,
	0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,
	1,11,11,11,11,11,11,1,11,1,0,2,0,0,0,0,
	0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,
	0,1,1,11,11,11,1,0,1,0,0,2,0,0,0,0,
	0,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
	2,2,2,
	0,0,0,1,1,1,0,0,0,0,0,2,0,0,0,0,
	0,2,0,0,0,0,0,0,0,2,0,0,0,0,0,2,
	0,0,0,
	2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
	2,2,0,0,12,13,13,0,0,2,0,14,12,13,0,2,
	0,14,0,
	0,0,1,1,1,0,0,0,0,2,0,1,1,1,0,1,
	0,2,0,12,13,13,15,13,0,2,0,14,12,12,0,2,
	0,0,0,
	0,1,16,16,16,1,0,1,0,2,1,4,1,4,1,4,
	1,2,0,12,12,12,13,13,0,2,0,14,14,14,0,2,
	2,2,2,
	1,16,1,16,16,16,1,16,1,2,1,4,4,4,1,4,
	1,2,0,14,12,12,12,14,0,2,0,0,0,0,0,2,
	0,0,0,
	1,16,16,16,16,16,1,16,1,2,0,1,1,1,0,1,
	0,2,0,0,14,14,14,0,0,2,2,2,2,2,2,2,
	0,0,0,
	0,1,16,16,16,1,0,1,0,2,2,2,2,2,2,2,
	2,2,0,0,0,0,0,0,0,2,0,0,0,0,2,0,
	0,0,0,
	0,0,1,1,1,0,0,0,0,2,0,1,1,1,0,1,
	0,2,2,2,2,2,2,2,2,2,0,14,12,0,2,0,
	0,0,0,
	2,2,2,2,2,2,2,2,2,2,1,3,3,3,1,3,
	1,2,0,0,0,0,0,0,0,2,0,14,14,0,2,0,
	0,0,0,
	0,0,0,0,0,0,0,0,0,2,0,1,1,1,0,1,
	0,2,0,0,12,13,15,13,0,2,0,0,0,0,2,0,
	0,0,0,
	0,0,0,0,0,0,0,0,0,2,2,2,2,2,2,2,
	2,2,0,14,12,12,12,12,0,2,2,2,2,2,2,0,
	0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,2,0,14,14,14,12,0,0,2,0,0,0,0,0,0,
	0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,2,0,0,0,0,0,0,0,2,0,0,0,0,0,0,
	0,0,0
	};