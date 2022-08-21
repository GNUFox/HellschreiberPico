
#ifndef __OSC_SCREEN_H__
#define __OSC_SCREEN_H__

#include "pico/stdlib.h"
#include <stdint.h>
#include <stdio.h>

// Screen Drawing definitions
#define HOR_MAX 80
#define VERT_MAX 28

#define V_PIN 15
#define Z_MOD_PIN 11
#define TRIG_PIN 10 

#define MULTIPLIER 0.5

void init_screen();
void clear_screen(int fill);
void fill_screen();
void box(int i, int j);
void checker_board(int i, int j);
void set_pixel(int i, int j, int on_off);
void place_char(int i, int j, char c, int double_scale, int invert);
void print_string(char* str, int pos_x, int pos_y, int double_scale, int inverted);

void output_screen_to_gpio();

int _inside_bounds(int i, int j);
int _inside_margin(int i, int j, int margin);

#define SLEEP_NOP __asm volatile ("nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n");

#endif