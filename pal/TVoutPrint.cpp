#include <math.h>
#include <avr/pgmspace.h>

#include "TVout.h"

void TVout::select_font(const unsigned char * f) {
	font = f;
}
void TVout::print_char(uint8_t x, uint8_t y, unsigned char c) {
        if(y>88)
          return;
	c -= pgm_read_byte(font+2);
	bitmap(x,y,font,(c*pgm_read_byte(font+1))+3,pgm_read_byte(font),pgm_read_byte(font+1));
}
