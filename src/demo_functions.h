#ifndef __DEMO_FUNCTIONS_H__
#define __DEMO_FUNCTIONS_H__

#define SCROLL_LEFT_TO_RIGHT 1
#define SCROLL_RIGHT_TO_LEFT 2

void sequential_string_display(char **string_array, int size, int scaling, int inverted);
void init_scroll_text(char * string, int left_to_right);
void scroll_text(int scaling, int inverted);


#endif