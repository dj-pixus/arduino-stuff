#ifndef VIDEO_GEN_H
#define VIDEO_GEN_H

typedef struct {
	volatile int scanLine;
	volatile int vsyncScanLine;
	volatile unsigned long frames;
	int first_frame_start_render_line;
	int first_frame_end_render_line;
	int second_frame_start_render_line;
	int second_frame_end_render_line;
	int lines_frame; 		//remove me
	uint8_t vres;
	uint8_t hres;
	uint16_t size;
	uint8_t output_delay; 	//remove me
	char vscale_const;		//combine me with status switch
	char vscale;			//combine me too.
	uint8_t * screen;
} TVout_vid;

extern TVout_vid display;

void render_setup(uint8_t mode, uint8_t x, uint8_t y, uint8_t *scrnptr);

void first_frame_vsync_lines();
void first_frame_blank_line();
void first_frame_active_line();
void second_frame_vsync_lines();
void second_frame_blank_line();
void second_frame_active_line();
void empty();

void render_line6c();
void render_line5c();
void render_line4c();
void render_line3c();
#endif
