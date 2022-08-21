#include "demo_functions.h"
#include "osc_screen.h"
#include "hell_chars.h"
#include <string.h>

static int scroll_pos_x;
static int scroll_pos_x_reset;
static int scroll_length;
static char * str_scroll_text;

void sequential_string_display(char **string_array, int size, int scaling, int inverted)
{
    for(int string_index = 0;string_index < size;string_index++)
    {
        sleep_ms(1000);
        clear_screen(0);
        print_string(string_array[string_index], 2,2,scaling,inverted);
    }
}

void init_scroll_text(char * string, int left_to_right)
{
    str_scroll_text = string;
    scroll_length = strlen(string);
    switch(left_to_right)
    {
        case SCROLL_LEFT_TO_RIGHT:
            //TODO: implement
        break;
        case SCROLL_RIGHT_TO_LEFT:
            scroll_pos_x_reset = HOR_MAX;
            scroll_pos_x = scroll_pos_x_reset;
        break;
    }
}

void scroll_text(int scaling, int inverted)
{
    int len = strlen(str_scroll_text);
    if(scroll_pos_x < -scroll_length*HELL_CHAR_COL_MAX*scaling)
    {
        scroll_pos_x = scroll_pos_x_reset;
    }
    sleep_ms(50);
    clear_screen(0);
    print_string(str_scroll_text, scroll_pos_x, 2, scaling, inverted);
    scroll_pos_x--;
}