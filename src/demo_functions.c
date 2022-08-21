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

// sequence stuff
static bool seq_next;

void sequential_string_display(char **string_array, int size, int scaling, int inverted)
{
    for(int string_index = 0;string_index < size;string_index++)
    {
        sleep_ms(1000);
        clear_screen(inverted);
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

void scroll_text(int scaling, int inverted, int sleep_time)
{
    int len = strlen(str_scroll_text);
    if(scroll_pos_x < -scroll_length*HELL_CHAR_COL_MAX*scaling)
    {
        scroll_pos_x = scroll_pos_x_reset;
    }
    sleep_ms(sleep_time);
    clear_screen(inverted);
    print_string(str_scroll_text, scroll_pos_x, 2, scaling, inverted);
    scroll_pos_x--;
}

void checker_board(int pos_x, int pos_y, int size_x, int size_y, bool inverted)
{
    bool t = !inverted;
    bool f = inverted;
    for (int i = 0; i < HOR_MAX; i++)
    {
        for (int j = 0; j < VERT_MAX; j++)
        {
            if(_inside_bounds(i,j) && _inside_margin(i,j,3))
            {
                if((i%6)>2)
                {
                    if((j%6)>2)
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
                    if((j%6)>2)
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


/*
void up_down_A()
{
    int h_pos_cnt = -14;
    int vert_pos = 1;
    int down_up = 1;
    while(1)
    {
        if(h_pos_cnt > HOR_MAX) h_pos_cnt = -14;

        if(down_up == 1)
        {
            if(vert_pos < 10) vert_pos++;
            if(vert_pos == 10)down_up = 0;
        }
        else
        {
            if(vert_pos > 0) vert_pos--;
            if(vert_pos == 0) down_up = 1;
        }

        gpio_put(LED_PIN, !gpio_get(LED_PIN));
        sleep_ms(80);
        clear_screen(0);
        place_char(h_pos_cnt,vert_pos,'a', 1, 0);
        h_pos_cnt++;
    }
}*/


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
    busy_wait_ms(200);
    /*uint8_t random = 0;
    for(int i = 0; i < 8; i++)
    {
        random |= rosc_hw->randombit << i;
    }
    printf("Button pressed %d\n", random);*/

    set_seq_next(true);
}