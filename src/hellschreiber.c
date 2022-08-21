
#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/multicore.h"
#include "hardware/gpio.h"
#include <string.h>
#include <stdbool.h>

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

    gpio_set_irq_enabled_with_callback(BUTTON, GPIO_IRQ_EDGE_FALL, true, (gpio_irq_callback_t)button_interrupt);
    set_seq_next(false);

    bool true_pointer = true;
    repeating_timer_t timer;
    add_repeating_timer_ms(20*1000, (repeating_timer_callback_t)set_seq_next, &true_pointer, &timer);

    while(1)
    {
        clear_screen(SCREEN_EMPTY);

        bool inverted = false;
        for(int i = 0; i < 10 && !get_seq_next(); i++)
        {
            checker_board(20,5,20,10, inverted);
            sleep_ms(500);
            inverted = !inverted;
        }

        char *welcome_strings[4];
        welcome_strings[0] = "HALLO";
        welcome_strings[1] = " IM  ";
        welcome_strings[2] = " REAL";
        welcome_strings[3] = " RAUM";
        
        while(!get_seq_next())
        {
            sequential_string_display(welcome_strings, 4, SCALE_DOUBLE,NON_INVERTED);
        }

        char *str_hallo = "HALLO IM REALRAUM";
        init_scroll_text(str_hallo, SCROLL_RIGHT_TO_LEFT);
        while(!get_seq_next())
        {
            scroll_text(SCALE_DOUBLE,NON_INVERTED, 20);
        }

        char *str_hellschreiber = "HELLSCHREIBER DEMO";
        init_scroll_text(str_hellschreiber, SCROLL_RIGHT_TO_LEFT);
        while(!get_seq_next())
        {
            scroll_text(SCALE_DOUBLE,NON_INVERTED, 25);
        }
        char *str_hellschreiber2 = "ABCDEFGHIJKLMNOPQRSTUVWXYZ 1234567890";
        init_scroll_text(str_hellschreiber2, SCROLL_RIGHT_TO_LEFT);
        while(!get_seq_next())
        {
            scroll_text(SCALE_DOUBLE,NON_INVERTED, 20);
        }
        clear_screen(SCREEN_EMPTY);

        sleep_ms(500);

        
        while(1 && !get_seq_next())
        {
            clear_screen(SCREEN_EMPTY);
            print_string(" IDLE ", 2,0,SCALE_DOUBLE,inverted ? INVERTED : NON_INVERTED);
            inverted=!inverted;
            sleep_ms(1000);
        }
    }
    
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


