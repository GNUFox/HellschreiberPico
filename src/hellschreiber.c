
#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/multicore.h"
#include <string.h>

#include "osc_screen.h"
#include "hell_chars.h"
#include "demo_functions.h"

void core1_entry()
{
    const uint LED_PIN = PICO_DEFAULT_LED_PIN;
    const uint BUTTON = 16;
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    gpio_init(BUTTON);
    gpio_set_dir(BUTTON, GPIO_IN);
    gpio_set_pulls(BUTTON, true, false);


    int h_pos_cnt = -14;
    int vert_pos = 1;
    int down_up = 1;
    /*
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
    }*/

    char *welcome_strings[4];
    welcome_strings[0] = "HALLO";
    welcome_strings[1] = " IM  ";
    welcome_strings[2] = " REAL";
    welcome_strings[3] = " RAUM";
    
    for(int i = 0; i < 2; i++)
    {
        /*for(int string_index = 0;string_index <4;string_index++)
        {
            gpio_put(LED_PIN, !gpio_get(LED_PIN));
            sleep_ms(1000);
            clear_screen(0);
            print_string(welcome_strings[string_index], 2,2,1,0);
        }*/
        sequential_string_display(welcome_strings, 4, SCALE_DOUBLE,NON_INVERTED);
    }

    char str_hellschreiber[14] = "HELLSCHREIBER";
    init_scroll_text(str_hellschreiber, SCROLL_RIGHT_TO_LEFT);
    //int scroll_pos_x = HOR_MAX;
    while(gpio_get(BUTTON))
    {
        /*
        if(scroll_pos_x < -14*HELL_CHAR_COL_MAX*2)
        {
            scroll_pos_x = HOR_MAX;
        }
        sleep_ms(50);
        clear_screen(0);
        print_string(str_hellschreiber, scroll_pos_x, 2, 1, 0);
        scroll_pos_x--;*/
        scroll_text(SCALE_DOUBLE,NON_INVERTED);
    }

    print_string("IDLE", 2,2,SCALE_DOUBLE,NON_INVERTED);
    
}

int main()
{   
    stdio_init_all();
    multicore_launch_core1(core1_entry);
    const uint LED_PIN = PICO_DEFAULT_LED_PIN;
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    init_screen();
    fill_screen();


    while (true)
    {
        output_screen_to_gpio();
        //gpio_put(LED_PIN, !gpio_get(LED_PIN));
    }
    return 0;
}


