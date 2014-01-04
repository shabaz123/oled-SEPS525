// oled.c
// OLED test program
// rev 1 - January 2014 - Shabaz

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "BBBiolib.h"
#include "oled.h"
#include "vfont.h"
#include "color.h"

// function prototypes
void oled_powerup(void);
void oled_powerdown(void);
void write_idx(unsigned char v);
void write_ctrl(unsigned char v);
void write_param(unsigned char addr, unsigned char val);
void write_16(unsigned short int v);
void plot(unsigned char x, unsigned char y, unsigned int lcol);
void line(unsigned char x, unsigned char y, unsigned char x2, unsigned char y2,
						unsigned int lcol);
void rect_fill(unsigned char x, unsigned char y, unsigned char dx, unsigned char dy,
             unsigned int lcol, unsigned int fcol);
void rect(unsigned char x, unsigned char y, unsigned char dx, unsigned char dy,
             unsigned int lcol);
unsigned char write_text(char* text, unsigned char x, unsigned char y,
						unsigned int lcol, double xscale, double yscale, unsigned char* dy);
void read_img(unsigned int* buf, unsigned int size, unsigned char* fname);
void write_img(unsigned char x, unsigned char y, unsigned int* imgbuf,
						unsigned int dx, unsigned int dy);


// variables
extern const int simplex[95][112];
const av_t oled_init_arr[]=
{
	{DISP_ON_OFF,0},
	{OSC_CTL,1},
	{CLOCK_DIV, 0x30},
	{REDUCE_CURRENT, 0},
	{IREF, 0},
	{PRECHARGE_TIME_R, 1},
	{PRECHARGE_TIME_G, 1},
	{PRECHARGE_TIME_B, 1},
	{PRECHARGE_CURRENT_R, 0x0c},
	{PRECHARGE_CURRENT_G, 0x19},
	{PRECHARGE_CURRENT_B, 0x15},
	{DRIVING_CURRENT_R, 0x32},
	{DRIVING_CURRENT_G, 0x27},
	{DRIVING_CURRENT_B, 0x2b},
	{DISPLAY_MODE_SET, 0},
	{RGB_IF, 0x11},
	{RGB_POL,0},
	{MEMORY_WRITE_MODE, 0x26}, /*76*/
	{MX1_ADDR, 0},
	{MX2_ADDR, 0x9f},
	{MY1_ADDR, 0},
	{MY2_ADDR, 0x7f},
	{MEM_PTR_X, 0},
	{MEM_PTR_Y, 0},
	{DUTY, 0x7f},
	{DSL, 0},
	{D1_DDRAM_FAC, 0},
	{D1_DDRAM_FAR, 0},
	{D2_DDRAM_SAC, 0},
	{D2_DDRAM_SAR, 0},
	{SCR1_FX1, 0},
	{SCR1_FX2, 0x9f},
	{SCR1_FY1, 0},
	{SCR1_FY2, 0x7f},
	{FIN, FIN}
};

// set to zero to disable debug, 1 to enable debug
int display_dbg=1;

void
demo1(void)
{
	unsigned char dx, dy;
	int i;
	
	rect_fill(0, 0, 160, 128, 0x0000, 0x0000); // clear to blank screen
	
	dx=write_text("IP Addr", 90, 110, 0xe000, 0.5, 0.5, &dy);
	dx=write_text("Netmask", 90, 90, 0xe000, 0.5, 0.5, &dy);
	dx=write_text("Default GW", 65, 70, 0xe000, 0.5, 0.5, &dy);
	dx=write_text("Timezone", 85, 50, 0xe000, 0.5, 0.5, &dy);
	dx=write_text("Time", 120, 30, 0xe000, 0.5, 0.5, &dy);
	dx=write_text("MENU", 112, 10, 0xffff, 0.5, 0.5, &dy);
	
	for(i=0; i<8; i++)
	{
		dx=write_text("IP Addr", 90, 110, 0xe000, 0.5, 0.5, &dy);
		iolib_delay_ms(500);
		rect_fill(90, 110, dx, dy, 0x0000, 0x0000);
		iolib_delay_ms(500);
	}
}

void
demo2(void)
{
	int i;
	unsigned int img[160*120];
	
	rect_fill(0, 0, 160, 128, 0x0000, 0x0000); // clear to blank screen
	read_img(img, 160*120, "snoopy.raw");
	write_img(0, 4, img, 160, 120);
	for (i=0; i<10; i++)
	{
		iolib_delay_ms(999);
	}
}

void
demo3(void)
{
	int i;
	unsigned char dx, dy;
	char ts[32];
	
	rect_fill(0, 0, 160, 128, 0x0000, 0x0000); // clear to blank screen
	rect_fill(0, 0, 160, 35, 0xd0d0, 0x0a0a);
	dx=write_text("FM", 80, 64, 0xe000, 1.5, 1.5, &dy);

	for (i=1; i<110; i++)
	{
		sprintf(ts, "%d.0000MHz", i);
		
		dx=write_text(ts, 10, 10, 0xe000, 0.5, 0.5, &dy);
		rect_fill(10,10 , dx, dy, 0x0a0a, 0x0a0a);
	}	
	dx=write_text(ts, 10, 10, 0xe000, 0.5, 0.5, &dy);
	for (i=0; i<3; i++)
	{
		iolib_delay_ms(999);
	}
}

void
demo4(void)
{
	int i;
	rgb_t col, col2, col3;
	unsigned char dx, dy;
	
	col.r=255;
	col.g=0;
	col.b=0;
	col2.r=0;
	col2.g=100;
	col2.b=0;
	col3.r=255;
	col3.g=255;
	col3.b=0;
	
	rect_fill(0, 0, 160, 128, 0x0000, 0x0000); // clear to blank screen
	
	rect_fill(0, 30, 160, 60, col565(&col), col565(&col2)); // Bordered rectangle
	dx=write_text("Alarm", 10, 45, col565(&col3), 1.5, 1.5, &dy);
	for (i=0; i<5; i++)
	{
		iolib_delay_ms(999);
	}
}

void
demo5(void)
{
	int i,fr;
	unsigned int img[3][43*82]; // create RAM buffer for three frames, with a max size of 43*82 per image
	
	// read in three sprites into RAM
	read_img(&img[0][0], 43*82, "f1.raw");
	read_img(&img[1][0], 43*82, "f2.raw");
	read_img(&img[2][0], 43*82, "f3.raw");
	
	rect_fill(0, 0, 160, 128, 0x0000, 0x0000); // clear to blank screen
	
	// play animation for a while
	fr=0;
	for (i=0; i<100; i++)
	{
		write_img(0, 0, &img[fr][0], 43, 82);
		fr++;
		if (fr==3) fr=0;
	}
	
	for (i=0; i<5; i++)
	{
		iolib_delay_ms(999);
	}
}

int
main(int argc, char *argv[])
{
	// Do iolib initialisation
	iolib_init();
  BBBIO_sys_Enable_GPIO(BBBIO_GPIO2);
  iolib_setdir(8, 11, BBBIO_DIR_OUT);
	iolib_setdir(8, 12, BBBIO_DIR_OUT);
	iolib_setdir(8, 15, BBBIO_DIR_OUT);
	iolib_setdir(8, 44, BBBIO_DIR_OUT);
	iolib_setdir(8, 45, BBBIO_DIR_OUT);
	iolib_setdir(8, 46, BBBIO_DIR_OUT);
	
	// initial setup
	// Get pins into a known state, ensure the HV supply
	// is off, and issue a reset pulse for the OLED display
	HV_DIS;
	CS_HIGH;
	RS_HIGH;
	SCL_LOW;
	SDA_LOW;
	RESET_LOW;
	iolib_delay_ms(1);
	RESET_HIGH;
	iolib_delay_ms(1);
	
	oled_powerup(); // configures the OLED, and powers it up
	
	// run the demonstrations
	demo1();
	demo2();
	demo3();
	demo4();
	demo5();
	
	oled_powerdown();
	return(0);
}

// This switches off display and removes the high voltage
void
oled_powerdown(void)
{
	write_param(DISP_ON_OFF, 0);
	HV_DIS;
	iolib_delay_ms(100);
}

// This configures the display, applies the high voltage and enables the display
void
oled_powerup(void)
{
	int i=0;
	int not_finished=1;
	unsigned char addr, val;
	while(not_finished)
	{
		addr=oled_init_arr[i].addr;
		val=oled_init_arr[i].val;
		if ((addr==FIN) && (val==FIN))
		{
			not_finished=0;
			break;
		}
		write_param(addr, val);
		i++;
	}
	HV_EN;
	iolib_delay_ms(100);
	rect_fill(0, 0, 160, 128, 0x0000, 0x0000); // clear to blank screen
	write_param(DISP_ON_OFF, 1);
	iolib_delay_ms(10);

}

void
plot(unsigned char x, unsigned char y, unsigned int lcol)
{
	int i, xx, yy;
	write_param(MX1_ADDR, x);
	write_param(MX2_ADDR, x);
	write_param(MY1_ADDR, 127-y);
	write_param(MY2_ADDR, 127-y);
	
	write_param(MEM_PTR_X, x);
	write_param(MEM_PTR_Y, 127-y);
	write_idx(DDRAM_DATA_ACCESS_PORT);
	write_16(lcol);
}

void
line(unsigned char x, unsigned char y, unsigned char x2, unsigned char y2,
						unsigned int lcol)
{
	int dx, dy;
	int sx, sy, err, e2;
	
	if (x>x2)
		dx=(int)(x-x2);
	else
		dx=(int)(x2-x);
	if (y>y2)
		dy=(int)(y-y2);
	else
		dy=(int)(y2-y);

  if (x<x2)
  	sx=1;
  else
  	sx=-1;
  
  if (y<y2)
		sy=1;
	else
		sy=-1;
		
  err=dx-dy;
 
  while(1)
 	{
  	plot(x, y, lcol);
    if ((x==x2) && (y==y2))
    	break;
    e2=2*err;
    if (e2>-dy)
    {
    	err=err-dy;
       x=x+(unsigned char)sx;
    }
    if ((x==x2) && (y==y2))
    { 
      plot(x, y, lcol);
      break;
    }
    if (e2<dx)
    { 
      err=err+dx;
       y=y+(unsigned char)sy; 
    }
  } // end while(1)
	
}

void
rect_fill(unsigned char x, unsigned char y, unsigned char dx, unsigned char dy,
             unsigned int lcol, unsigned int fcol)
{
	int i, xx, yy;
	write_param(MX1_ADDR, x);
	write_param(MX2_ADDR, x+dx-1);
	write_param(MY1_ADDR, 128-y-dy);
	write_param(MY2_ADDR, 127-y);
	
	write_param(MEM_PTR_X, x);
	write_param(MEM_PTR_Y, 128-y-dy);
	write_idx(DDRAM_DATA_ACCESS_PORT);
	if (lcol==fcol)
	{
		for (i=0; i<(dx*dy); i++)
		{
			write_16(fcol);
		}
	}
	else
	{
		for (yy=0; yy<dy; yy++)
		{
			for (xx=0; xx<dx; xx++)
			{
				if ((xx==0) || (xx==dx-1) || (yy==0) || (yy==dy-1))
				{
					write_16(lcol);
				}
				else
				{
					write_16(fcol);
				}
			}
		}
	}
}

void
rect(unsigned char x, unsigned char y, unsigned char dx, unsigned char dy,
             unsigned int lcol)
{
	rect_fill(x, y, dx, 1, lcol, lcol);
	rect_fill(x, y+dy-1, dx, 1, lcol, lcol);
	rect_fill(x, y, 1, dy, lcol, lcol);
	rect_fill(x+dx-1, y, 1, dy, lcol, lcol);
}

unsigned char
write_text(char* text, unsigned char x, unsigned char y, unsigned int lcol,
							double xscale, double yscale, unsigned char* dy)
{
	unsigned char xx, wt, mdy;
	int i, im;
	int ci=0;
	unsigned char c;
	int nv;
	unsigned char w;
	int ipx, ipy;
	unsigned char px, py;
	unsigned char ox, oy;
	int old=0;
	int do_scale=0;
	int not_finished=1;
	mdy=0;
	
	ox=0; oy=0;
	wt=0;
	
	if ((xscale>1.01) || (xscale<1.01) || (yscale>1.01) || (yscale<1.01))
		do_scale=1;
	
	xx=x;
	while(not_finished)
	{
		c=text[ci];
		ci++;
		if (c=='\0')
			break;
		if ((c>126) || (c<32))
			continue;
		c=c-32;
		nv=simplex[c][0];
		w=simplex[c][1];
		if (nv==0)
		{
			xx=xx+w;
			wt=wt+w;
			continue;
		}
		im=2+(nv*2);
		old=0;
		for (i=2; i<im; i=i+2)
		{
			ipx=simplex[c][i];
			ipy=simplex[c][i+1];
			px=(unsigned char)ipx;
			py=(unsigned char)ipy;
			if (do_scale)
			{
				px=(unsigned char)(((double)ipx)*xscale);
				py=(unsigned char)(((double)ipy)*yscale);
			}
			if ((py!=255) && (py>mdy))
				mdy=py+1;
			if ((ipx==-1) && (ipy==-1)) // pen up
			{
				old=0;
			}
			else
			{
				if (old) // a previous point is stored in ox, oy
				{
					if (((ox+xx)>159) || ((px+xx)>159))
					{
						not_finished=0;
						break;
					}
					line(ox+xx, oy+y, px+xx, py+y, lcol);
					ox=px; oy=py;
				}
				else
				{
					ox=px; oy=py;
					old=1;
				}
			}
		}
		if (do_scale)
		{
			xx=xx+(unsigned char)(((double)w)*xscale);
			wt=wt+(unsigned char)(((double)w)*xscale);
		}
		else
		{
			xx=xx+w;
			wt=wt+w;
		}
	}

	*dy=mdy;
	return(wt+1);
}

void
read_img(unsigned int* buf, unsigned int size, unsigned char* fname)
{
	int i;
	FILE* infile;
	rgb_t rgb;
	
	// read in image from file into RAM
	infile=fopen(fname, "rb");
	if (infile==NULL)
	{
		fprintf(stderr, "%s: Error: Unable to open input file '%s'\n", fname);
		exit(1);
	}
	
	for (i=0; i<size; i++)
	{
		fread(&rgb.r, 1, 1, infile);
		fread(&rgb.g, 1, 1, infile);
		fread(&rgb.b, 1, 1, infile);
			
		buf[i]=col565(&rgb);
	}
	fclose(infile);
}

void
write_img(unsigned char x, unsigned char y, unsigned int* imgbuf,
						unsigned int dx, unsigned int dy)
{
	int ctr=0;
	int ix, iy;
	
	write_param(MX1_ADDR, x);
	write_param(MX2_ADDR, x+dx-1);
	write_param(MY1_ADDR, 127-y-(dy-1));
	write_param(MY2_ADDR, 127-y);

	write_param(MEM_PTR_X, x);
	write_param(MEM_PTR_Y, 127-y);
	write_idx(DDRAM_DATA_ACCESS_PORT);
	
	for (iy=0; iy<dy; iy++)
	{
		for (ix=0; ix<dx; ix++)
		{
			write_16(imgbuf[ctr]);
			ctr++;
		}
	}
}

// Use this to write an instruction to the display
void
write_param(unsigned char addr, unsigned char val)
{
	//if (display_dbg) printf("addr=0x%02x, val=0x%02x\n", addr, val);
	write_idx(addr);
	write_ctrl(val);
}

void
write_idx(unsigned char v)
{
	int i;
	unsigned char ibyte;
	RS_LOW;
	CS_LOW;
	SCL_LOW;
	
	ibyte=v;
	// transmit instruction byte
	for (i=0; i<8; i++)
	{
		if (ibyte & 0x80)
		{
			SDA_HIGH;
		}
		else
		{
			SDA_LOW;
		}
		ibyte=ibyte<<1;
		SCL_HIGH;		
		if (i<7)	// on last bit, don't pull SCL low
		{
			SCL_LOW;
		}
	}	
	RS_HIGH;
	CS_HIGH;
}

void
write_ctrl(unsigned char v)
{
	int i;
	unsigned char ibyte;
	RS_HIGH;
	CS_LOW;
	SCL_LOW;
	
	ibyte=v;
	// transmit instruction byte
	for (i=0; i<8; i++)
	{
		if (ibyte & 0x80)
		{
			SDA_HIGH;
		}
		else
		{
			SDA_LOW;
		}
		ibyte=ibyte<<1;
		SCL_HIGH;		
		if (i<7)	// on last bit, don't pull SCL low
		{
			SCL_LOW;
		}
	}	
	RS_HIGH;
	CS_HIGH;
}

// This writes a 16-bit RGB565 value to the display
void
write_16(unsigned short int v)
{
	int i, j;
	unsigned char* vptr;
	unsigned char ibyte[2];
	RS_HIGH;
	CS_LOW;
	SCL_LOW;

	vptr=(unsigned char*)&v;
	ibyte[1]=*vptr;
	ibyte[0]=*(vptr+1);
	
	for (j=0; j<2; j++)
	{
		// transmit instruction byte
		for (i=0; i<8; i++)
		{
			if (ibyte[j] & 0x80)
			{
				SDA_HIGH;
			}
			else
			{
				SDA_LOW;
			}
			ibyte[j]=ibyte[j]<<1;
			SCL_HIGH;		
			SCL_LOW;
		}	
	}
	SCL_HIGH;
	RS_HIGH;
	CS_HIGH;
}


	