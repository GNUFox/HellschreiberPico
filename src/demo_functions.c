#include "demo_functions.h"
#include "osc_screen.h"

static int scroll_pos_x;
static int scroll_length;
static char * scroll_text;

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
    scroll_text = string;
    switch(left_to_right)
    {
        case SCROLL_LEFT_TO_RIGHT:

        break;
        case SCROLL_RIGHT_TO_LEFT:

        break;
    }
}

void scroll_text(int scaling, int inverted)
{

}