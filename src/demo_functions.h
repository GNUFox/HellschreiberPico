#ifndef __DEMO_FUNCTIONS_H__
#define __DEMO_FUNCTIONS_H__

#include "hardware/gpio.h"
#include <stdbool.h>

#define SCROLL_LEFT_TO_RIGHT 1
#define SCROLL_RIGHT_TO_LEFT 2

// random lines stuff
#define _RANDOM_LINES_MAX 60

// scroll stuff
#define SCROLL_LEFT_TO_RIGHT 1
#define SCROLL_RIGHT_TO_LEFT 2
#define SCROLL_UP 3
#define SCROLL_DOWN 4

// zooming_boxes stuff
#define _ZOOM_BOX_WIDTH 8
#define _ZOOM_BOX_HEIGHT 4
#define _ZOOM_BOX_MAX 10
#define _ZOOM_BOX_NUM 4

// Text stuff
void sequential_string_display(char **string_array, int size, int scaling, int inverted);
void init_scroll_text(char * string, int left_to_right);
void scroll_text(int scaling, int inverted, int sleep_time);
void init_bounce_text_up_down(char * string);
void bounce_text_up_down(int scaling, int inverted, int sleep_time);
void draw_random_lines();

// shapes
void checker_board(int pos_x, int pos_y, int size_x, int size_y, bool inverted);
void init_zooming_boxes();
void zooming_boxes();


// interrupts
void set_seq_next(bool s);
bool get_seq_next();
void button_interrupt();

// misc
uint64_t get_random_number(uint64_t max);

#endif