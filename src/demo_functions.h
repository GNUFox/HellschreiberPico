#ifndef __DEMO_FUNCTIONS_H__
#define __DEMO_FUNCTIONS_H__

#include "hardware/gpio.h"
#include <stdbool.h>

#define SCROLL_LEFT_TO_RIGHT 1
#define SCROLL_RIGHT_TO_LEFT 2

// Text stuff
void sequential_string_display(char **string_array, int size, int scaling, int inverted);
void init_scroll_text(char * string, int left_to_right);
void scroll_text(int scaling, int inverted, int sleep_time);

// shapes
void checker_board(int pos_x, int pos_y, int size_x, int size_y, bool inverted);

// interrupts
void set_seq_next(bool s);
bool get_seq_next();
void button_interrupt();

// misc
uint8_t get_random_number();


#endif