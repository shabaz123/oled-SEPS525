// color.c
// Color mapping functions
// rev 0.5 Shabaz June 2013

#include <math.h>
#include "color.h"

unsigned int
col565(rgb_t* rgb)
{
	unsigned int r,g,b;
	r=(unsigned int) rgb->r;
	g=(unsigned int) rgb->g;
	b=(unsigned int) rgb->b;
  return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
}

