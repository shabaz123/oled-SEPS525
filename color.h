// color.h
// Color mapping functions
// rev 0.5 Shabaz June 2013

#ifndef __COLOR__H__
#define __COLOR__H__

typedef struct rgb_s{
	unsigned char r;
	unsigned char g;
	unsigned char b;
} rgb_t;	

// convert from 24-bit RGB 16-bit
unsigned int col565(rgb_t* rgb);

#endif // __COLOR__H__
