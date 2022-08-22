
#ifndef __OSC_SCREEN_H__
#define __OSC_SCREEN_H__

#include "pico/stdlib.h"
#include <stdint.h>
#include <stdio.h>

// Screen Drawing definitions
#define HOR_MAX 80
#define VERT_MAX 32 // uint32_t for screen line vertical

#define V_PIN 15
#define Z_MOD_PIN 11
#define TRIG_PIN 10 

#define SCALE_NORM 1
#define SCALE_DOUBLE 2
#define SCALE_FULL 4
#define NON_INVERTED 0
#define INVERTED 1

#define SCREEN_FILLED 1
#define SCREEN_EMPTY 0

#define MULTIPLIER 0.5

extern uint32_t screen[HOR_MAX];

void init_screen();
void clear_screen(int fill);
void fill_screen();
void box(int i, int j);
void set_pixel(int i, int j, int on_off);
void place_char(int i, int j, char c, int scale, int invert);
void print_string(char* str, int pos_x, int pos_y, int scale, int inverted);

void output_screen_to_gpio();

int _inside_bounds(int i, int j);
int _inside_margin(int i, int j, int margin);

#define SLEEP_NOP __asm volatile ("nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n");

#endif