#include "TVout.h"
#include <string.h>

static void inline sp(unsigned char x, unsigned char y, char c);

char TVout::begin(uint8_t mode, uint8_t x, uint8_t y) {
	// check if x is divisable by 8
	if ( !(x & 0xF8))
		return 1;

	uint8_t *framebuffer = (uint8_t *)malloc((x/8) * y * sizeof(uint8_t));
	if (framebuffer == NULL)
		return 4;

	return begin(mode, x, y, framebuffer);
}

/* call this to start video output with a specified resolution.
 *
 * Arguments:
 *	mode:
 *		The video standard to follow and whether to use overlay (bitset):
 *		NTSC	=0	=_NTSC
 *		PAL		=1	=_PAL
 *		OVERLAY =2	=_OVERLAY
 *	x:
 *		Horizonal resolution must be divisable by 8.
 *	y:
 *		Vertical resolution.
 *  framebuffer:
 *    Pre-allocated buffer of at least (x/8 * y) bytes in size.
 *
 *	Returns:
 *		0 if no error.
 *		1 if x is not divisable by 8.
 */
char TVout::begin(uint8_t mode, uint8_t x, uint8_t y, uint8_t *framebuffer) {
	// check if x is divisable by 8
	if ( !(x & 0xF8))
		return 1;
	x = x/8;

	screen = framebuffer;

	render_setup(mode,x,y,screen);
	return 0;
}

/* Fill the screen with some color.
 *
 * Arguments:
 *	color:
 *		The color to fill the screen with.
 *		(see color note at the top of this file)
*/
void TVout::fill(uint8_t color) {
	switch(color) {
		case 0:
			memset(display.screen, 0, display.size);
			break;
		case 1:
			memset(display.screen, 0xFF, display.size);
			break;
		case 2:
			unsigned char *p = display.screen;
			uint16_t i = display.size;
			while (i--) {
				*p++ ^= 0xFF;
			}
			break;
	}
}




void TVout::set_pixel(uint8_t x, uint8_t y, char c) {
        if(y<96&&x<128)
	sp(x,y,c);
}

/* get the color of the pixel at x,y
 *
 * Arguments:
 *	x:
 *		The x coordinate of the pixel.
 *	y:
 *		The y coordinate of the pixel.
 *
 * Returns:
 *	The color of the pixel.
 *	(see color note at the top of this file)
 *
 * Thank you gijs on the arduino.cc forum for the non obviouse fix.
*/
unsigned char TVout::get_pixel(uint8_t x, uint8_t y) {
	if (display.screen[x/8+y*display.hres] & (0x80 >>(x&7)))
		return 1;
	return 0;
}

void TVout::bitmap(uint8_t x, uint8_t y, const unsigned char * bmp,
				   uint16_t i, uint8_t width, uint8_t lines) {

	uint8_t temp, lshift, rshift, save, xtra;
	uint16_t si = 0;
	
	rshift = x&7;
	lshift = 8-rshift;
	if (width == 0) {
		width = pgm_read_byte((uint32_t)(bmp) + i);
		i++;
	}
	if (lines == 0) {
		lines = pgm_read_byte((uint32_t)(bmp) + i);
		i++;
	}
		
	if (width&7) {
		xtra = width&7;
		width = width/8;
		width++;
	}
	else {
		xtra = 8;
		width = width/8;
	}
	
	for (uint8_t l = 0; l < lines; l++) {
		si = (y + l)*display.hres + x/8;
		if (width == 1)
			temp = 0xff >> rshift + xtra;
		else
			temp = 0;
		save = screen[si];
		screen[si] &= ((0xff << lshift) | temp);
		temp = pgm_read_byte((uint32_t)(bmp) + i++);
		screen[si++] |= temp >> rshift;
		for ( uint16_t b = i + width-1; i < b; i++) {
			save = screen[si];
			screen[si] = temp << lshift;
			temp = pgm_read_byte((uint32_t)(bmp) + i);
			screen[si++] |= temp >> rshift;
		}
		if (rshift + xtra < 8)
			screen[si-1] |= (save & (0xff >> rshift + xtra));	//test me!!!
		if (rshift + xtra - 8 > 0)
			screen[si] &= (0xff >> rshift + xtra - 8);
		screen[si] |= temp << lshift;
	}
}

void TVout::shift(uint8_t distance, uint8_t direction) {
	uint8_t * src;
	uint8_t * dst;
	uint8_t * end;
	uint8_t shift;
	uint8_t tmp;
	switch(direction) {
		case 0:
			dst = display.screen;
			src = display.screen + distance*display.hres;
			end = display.screen + display.vres*display.hres;

			while (src <= end) {
				*dst = *src;
				*src = 0;
				dst++;
				src++;
			}
			break;
		case 1:
			dst = display.screen + display.vres*display.hres;
			src = dst - distance*display.hres;
			end = display.screen;

			while (src >= end) {
				*dst = *src;
				*src = 0;
				dst--;
				src--;
			}
			break;
		case 2:
			shift = distance & 7;

			for (uint8_t line = 0; line < display.vres; line++) {
				dst = display.screen + display.hres*line;
				src = dst + distance/8;
				end = dst + display.hres-2;
				while (src <= end) {
					tmp = 0;
					tmp = *src << shift;
					*src = 0;
					src++;
					tmp |= *src >> (8 - shift);
					*dst = tmp;
					dst++;
				}
				tmp = 0;
				tmp = *src << shift;
				*src = 0;
				*dst = tmp;
			}
			break;
		case 3:
			shift = distance & 7;

			for (uint8_t line = 0; line < display.vres; line++) {
				dst = display.screen + display.hres-1 + display.hres*line;
				src = dst - distance/8;
				end = dst - display.hres+2;
				while (src >= end) {
					tmp = 0;
					tmp = *src >> shift;
					*src = 0;
					src--;
					tmp |= *src << (8 - shift);
					*dst = tmp;
					dst--;
				}
				tmp = 0;
				tmp = *src >> shift;
				*src = 0;
				*dst = tmp;
			}
			break;
	}
}

static void inline sp(uint8_t x, uint8_t y, char c) {
	if (c==1)
		display.screen[(x/8) + (y*display.hres)] |= 0x80 >> (x&7);
	else if (c==0)
		display.screen[(x/8) + (y*display.hres)] &= ~0x80 >> (x&7);
	else
		display.screen[(x/8) + (y*display.hres)] ^= 0x80 >> (x&7);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

/* Draw a line from one point to another
 *
 * Arguments:
 *	x0:
 *		The x coordinate of point 0.
 *	y0:
 *		The y coordinate of point 0.
 *	x1:
 *		The x coordinate of point 1.
 *	y1:
 *		The y coordinate of point 1.
 *	c:
 *		The color of the line.
 *		(see color note at the top of this file)
 */
/* Patched to allow support for the Arduino Leonardo */
void TVout::draw_line(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, char c) {

	if (x0 > display.hres*8 || y0 > 95 || x1 > display.hres*8 || y1 > 95)
		return;
	if (x0 == x1)
		draw_column(x0,y0,y1,c);
	else if (y0 == y1)
		draw_row(y0,x0,x1,c);
	else {
		int e;
		signed int dx,dy,j, temp;
		signed char s1,s2, xchange;
		signed int x,y;

		x = x0;
		y = y0;
	
		//take absolute value
		if (x1 < x0) {
			dx = x0 - x1;
			s1 = -1;
		}
		else if (x1 == x0) {
			dx = 0;
			s1 = 0;
		}
		else {
			dx = x1 - x0;
			s1 = 1;
		}

		if (y1 < y0) {
			dy = y0 - y1;
			s2 = -1;
		}
		else if (y1 == y0) {
			dy = 0;
			s2 = 0;
		}
		else {
			dy = y1 - y0;
			s2 = 1;
		}

		xchange = 0;   

		if (dy>dx) {
			temp = dx;
			dx = dy;
			dy = temp;
			xchange = 1;
		} 

		e = ((int)dy<<1) - dx;  
	 
		for (j=0; j<=dx; j++) {
			sp(x,y,c);
		 
			if (e>=0) {
				if (xchange==1) x = x + s1;
				else y = y + s2;
				e = e - ((int)dx<<1);
			}
			if (xchange==1)
				y = y + s2;
			else
				x = x + s1;
			e = e + ((int)dy<<1);
		}
	}
} // end of draw_line


/* Fill a row from one point to another
 *
 * Argument:
 *	line:
 *		The row that fill will be performed on.
 *	x0:
 *		edge 0 of the fill.
 *	x1:
 *		edge 1 of the fill.
 *	c:
 *		the color of the fill.
 *		(see color note at the top of this file)
*/
void TVout::draw_row(uint8_t line, uint16_t x0, uint16_t x1, uint8_t c) {
	uint8_t lbit, rbit;
	
        if(x0>127|x1>127|line>223)
          return;
	if (x0 == x1)
		set_pixel(x0,line,c);
	else {
		if (x0 > x1) {
			lbit = x0;
			x0 = x1;
			x1 = lbit;
		}
                x1++;
		lbit = 0xff >> (x0&7);
		x0 = x0/8 + display.hres*line;
		rbit = ~(0xff >> (x1&7));
		x1 = x1/8 + display.hres*line;
		if (x0 == x1) {
			lbit = lbit & rbit;
			rbit = 0;
		}
		if (c == 1) {
			screen[x0++] |= lbit;
			while (x0 < x1)
				screen[x0++] = 0xff;
			screen[x0] |= rbit;
		}
		else if (c == 0) {
			screen[x0++] &= ~lbit;
			while (x0 < x1)
				screen[x0++] = 0;
			screen[x0] &= ~rbit;
		}
		else if (c == 2) {
			screen[x0++] ^= lbit;
			while (x0 < x1)
				screen[x0++] ^= 0xff;
			screen[x0] ^= rbit;
		}
	}
} // end of draw_row


/* Fill a column from one point to another
 *
 * Argument:
 *	row:
 *		The row that fill will be performed on.
 *	y0:
 *		edge 0 of the fill.
 *	y1:
 *		edge 1 of the fill.
 *	c:
 *		the color of the fill.
 *		(see color note at the top of this file)
*/
void TVout::draw_column(uint8_t row, uint16_t y0, uint16_t y1, uint8_t c) {

	unsigned char bit;
	int byte;
	
	if (y0 == y1)
		set_pixel(row,y0,c);
	else {
		if (y1 < y0) {
			bit = y0;
			y0 = y1;
			y1 = bit;
		}
		bit = 0x80 >> (row&7);
		byte = row/8 + y0*display.hres;
		if (c == 1) {
			while ( y0 <= y1) {
				screen[byte] |= bit;
				byte += display.hres;
				y0++;
			}
		}
		else if (c == 0) {
			while ( y0 <= y1) {
				screen[byte] &= ~bit;
				byte += display.hres;
				y0++;
			}
		}
		else if (c == 2) {
			while ( y0 <= y1) {
				screen[byte] ^= bit;
				byte += display.hres;
				y0++;
			}
		}
	}
}

void TVout::draw_rect(uint8_t x0, uint8_t y0, uint8_t w, uint8_t h, char fc) {
	
	if (fc != -1) {
		for (unsigned char i = y0; i < y0+h && i<96; i++)
			draw_row(i,x0,x0+w,fc);
	}
}
