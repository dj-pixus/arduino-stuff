#ifndef TVOUT_H
#define TVOUT_H

#include <stdint.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <stdlib.h>

#include "video_gen.h"
#include "hardware_setup.h"
#include "video_properties.h"

class TVout {
public:
	uint8_t * screen;

	char begin(uint8_t mode);
	char begin(uint8_t mode, uint8_t x, uint8_t y);
	char begin(uint8_t mode, uint8_t x, uint8_t y, uint8_t *framebuffer);
	void end();
        const unsigned char * font;

	unsigned char hres();
	unsigned char vres();
	char char_line();

	//flow control functions
	void delay(unsigned int x);
	void delay_frame(unsigned int x);
	unsigned long millis();

	//basic rendering functions
	void set_pixel(uint8_t x, uint8_t y, char c);
	unsigned char get_pixel(uint8_t x, uint8_t y);
	void fill(uint8_t color);
	void shift(uint8_t distance, uint8_t direction);
	void bitmap(uint8_t x, uint8_t y, const unsigned char * bmp, uint16_t i = 0, uint8_t width = 0, uint8_t lines = 0);
        void draw_rect(uint8_t x0, uint8_t y0, uint8_t w, uint8_t h, char fc);
        void draw_row(uint8_t line, uint16_t x0, uint16_t x1, uint8_t c);
        void draw_column(uint8_t row, uint16_t y0, uint16_t y1, uint8_t c);
        void draw_line(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, char c);
	void select_font(const unsigned char * f);
        void print_char(uint8_t x, uint8_t y, unsigned char c);

};

#endif
