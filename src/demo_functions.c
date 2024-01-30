#include "demo_functions.h"
#include "osc_screen.h"
#include "hell_chars.h"
#include <string.h>
#include <stdbool.h>
#include "hardware/gpio.h"

#include "hardware/structs/rosc.h"

// scroll stuff
static int scroll_pos_x;
static int scroll_pos_x_reset;
static int scroll_length;
static char * str_scroll_text;

// text bounce stuff
static char * str_bounce_text;
static int bounce_length;
static int bounce_pos_cnt = -14;
static int bounce_vert_pos = 1;
static int bounce_down_up = 1;

// random lines stuff
static int random_lines_count;
static int random_lines_inverted = 0;

// zooming boxes stuff
static float zooming_boxes_factors[_ZOOM_BOX_NUM];

// sequence stuff
static bool seq_next;

void sequential_string_display(char **string_array, int size, int scaling, int inverted)
{
    for(int string_index = 0;string_index < size;string_index++)
    {
        clear_screen(inverted);
        print_string(string_array[string_index], 2,2,scaling,inverted);
        sleep_ms(1000);
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
    clear_screen(SCREEN_EMPTY);
}

//TODO: implement multi text scrolling (malloc etc.)
void scroll_text(int scaling, int inverted, int sleep_time)
{
    if(scroll_pos_x < -scroll_length*HELL_CHAR_COL_MAX*scaling)
    {
        scroll_pos_x = scroll_pos_x_reset;
    }
    sleep_ms(sleep_time);
    //clear_screen(inverted);
    print_string(str_scroll_text, scroll_pos_x, 2, scaling, inverted);
    
    scroll_pos_x--;
}

void checker_board(int pos_x, int pos_y, int size_x, int size_y, int block_size, bool inverted)
{
    bool t = !inverted;
    bool f = inverted;
    for (int i = 0; i < HOR_MAX; i++)
    {
        for (int j = 0; j < VERT_MAX; j++)
        {
            if(_inside_bounds(i,j) && _inside_margin(i,j,3))
            {
                if((i%block_size)>(block_size/2)-1)
                {
                    if((j%block_size)>(block_size/2)-1)
                    {
                        set_pixel(i, j, t);
                    }
                    else
                    {
                        set_pixel(i, j, f);
                    }
                }
                else
                {
                    if((j%block_size)>(block_size/2)-1)
                    {
                        set_pixel(i, j, f);
                    }
                    else
                    {
                        set_pixel(i, j, t);
                    }
                }
            }
        }
    }
}


void init_bounce_text_up_down(char * string)
{
    str_bounce_text = string;
    bounce_length = strlen(string);
    clear_screen(SCREEN_EMPTY);
}

void bounce_text_up_down(int scaling, int inverted, int sleep_time)
{

    if(bounce_pos_cnt > HOR_MAX) bounce_pos_cnt = -bounce_length*HELL_CHAR_COL_MAX*scaling;

    if(bounce_down_up == 1)
    {
        if(bounce_vert_pos < VERT_MAX-(HELL_CHAR_COL_MAX*scaling)) bounce_vert_pos++;
        if(bounce_vert_pos == VERT_MAX-(HELL_CHAR_COL_MAX*scaling))bounce_down_up = 0;
    }
    else
    {
        if(bounce_vert_pos > 0) bounce_vert_pos--;
        if(bounce_vert_pos == 0) bounce_down_up = 1;
    }

    sleep_ms(sleep_time);
    clear_screen(0);
    print_string(str_bounce_text, bounce_pos_cnt, bounce_vert_pos, scaling,inverted);
    bounce_pos_cnt++;
}

void draw_random_lines()
{
    if(random_lines_count >= _RANDOM_LINES_MAX)
    {
        random_lines_inverted = (int)get_random_number(2);
        clear_screen(random_lines_inverted == 1 ? SCREEN_FILLED : SCREEN_EMPTY);
        random_lines_count = 0;
    }
    draw_line((int)get_random_number(80),(int)get_random_number(32),
              (int)get_random_number(80),(int)get_random_number(32), random_lines_inverted == 1 ? INVERTED : NON_INVERTED);
    random_lines_count++;
}


void init_zooming_boxes()
{
    zooming_boxes_factors[0] = 1;
    zooming_boxes_factors[1] = 3;
    zooming_boxes_factors[2] = 6;
    zooming_boxes_factors[3] = 9;
}
void zooming_boxes()
{
    clear_screen(SCREEN_EMPTY);
    for(int i = 0; i < _ZOOM_BOX_NUM; i++)
    {
        box((int)(HOR_MAX/2-(_ZOOM_BOX_WIDTH/2)*zooming_boxes_factors[i]),
            (int)(VERT_MAX/2-(_ZOOM_BOX_HEIGHT/2)*zooming_boxes_factors[i]),
            (int)_ZOOM_BOX_WIDTH*zooming_boxes_factors[i], (int)_ZOOM_BOX_HEIGHT*zooming_boxes_factors[i], 1, NON_INVERTED);
        
        if(zooming_boxes_factors[i] <= _ZOOM_BOX_MAX)
        {
            zooming_boxes_factors[i]+=0.8;
        }
        else
        {
            zooming_boxes_factors[i] = 1;
        }
        
    }
}


void set_seq_next(bool s)
{
    seq_next = s;
}

bool get_seq_next()
{
    if(seq_next)
    {
        seq_next = false;
        return true;
    }
    return seq_next;
}

void button_interrupt()
{
    busy_wait_ms(200); // debounce
    /*uint8_t random = 0;
    for(int i = 0; i < 8; i++)
    {
        random |= rosc_hw->randombit << i;
    }
    printf("Button pressed %d\n", random);*/

    set_seq_next(true);
}

uint64_t get_random_number(uint64_t max)
{
    uint64_t random = 0;
    for(int i = 0; i < 64; i++)
    {
        random |= rosc_hw->randombit << i;
    }
    return random % max;
}

