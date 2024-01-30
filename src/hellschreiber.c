
#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/multicore.h"
#include "hardware/gpio.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include <string.h>
#include <stdbool.h>

#include "osc_screen.h"
#include "hell_chars.h"
#include "demo_functions.h"

#include "hell_rx.h"

#include "verticaldef.pio.h"
#include "readfbzmod.pio.h"

#define DEMO_ALL


#define DEMO_SEQUENTIAL
//#define DEMO_BOUNCE
//#define DEMO_LINES
//#define DEMO_BOXES
//#define DEMO_HELL

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

    while(1)
    {
        add_repeating_timer_ms(20*1000, (repeating_timer_callback_t)set_seq_next, &true_pointer, &timer);
        clear_screen(SCREEN_EMPTY);

        #if defined DEMO_CHECKER_BOARD || defined DEMO_ALL
        bool inverted = false;
        for(int i = 0; i < 10 && !get_seq_next(); i++)
        {
            checker_board(20,5,20,10, 4, inverted);
            sleep_ms(250);
            inverted = !inverted;
        }
        for(int i = 0; i < 100 && !get_seq_next(); i++)
        {
            checker_board(20,5,20,10, ((int)i/2)%10 + 4, false);
            sleep_ms(40);
        }
        #endif

        #if defined DEMO_SEQUENTIAL || defined DEMO_ALL
        char *welcome_strings[4];
        welcome_strings[0] = "HALLO";
        welcome_strings[1] = " IM  ";
        welcome_strings[2] = " REAL";
        welcome_strings[3] = " RAUM";
        
        while(!get_seq_next())
        {
            sequential_string_display(welcome_strings, 4, SCALE_DOUBLE,NON_INVERTED);
        }
        #endif

        #if defined DEMO_SCROLL || defined DEMO_ALL
        char *str_hallo = "Herzlich willkommen IM REALRAUM";
        init_scroll_text(str_hallo, SCROLL_RIGHT_TO_LEFT);
        while(!get_seq_next())
        {
            scroll_text(SCALE_NORM,NON_INVERTED, 20);
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
        #endif

        #if defined DEMO_IDLE || defined DEMO_ALL
        while(!get_seq_next())
        {
            clear_screen(SCREEN_EMPTY);
            print_string(" IDLE ", 2,2,SCALE_DOUBLE,inverted ? INVERTED : NON_INVERTED);
            inverted=!inverted;
            sleep_ms(1000);
        }
        #endif

        #if defined DEMO_BOUNCE || defined DEMO_ALL
        clear_screen(SCREEN_EMPTY);
        print_string("OWO", 0,3,SCALE_FULL, NON_INVERTED);
        while(!get_seq_next())
        {
        }
        clear_screen(SCREEN_EMPTY);
        init_bounce_text_up_down("OWO WHAT IS THIS");
        while(!get_seq_next())
        {
            bounce_text_up_down(SCALE_NORM, NON_INVERTED, 20);
        }
        #endif

        #if defined DEMO_LINES || defined DEMO_ALL
        clear_screen(SCREEN_EMPTY);
        while(!get_seq_next())
        {
            draw_random_lines();
            sleep_ms(40);
        }
        #endif

        #if defined DEMO_BOXES || defined DEMO_ALL
        clear_screen(SCREEN_EMPTY);
        init_zooming_boxes();
        
        while(!get_seq_next())
        {
            zooming_boxes();
            sleep_ms(60);
        }
        #endif

        #if defined DEMO_HELL || defined DEMO_ALL
        cancel_repeating_timer(&timer);
        clear_screen(SCREEN_EMPTY);
        init_hell_rx();
        while(!get_seq_next())
        {
            receive_to_fb();
        }
        #endif
    }
    
}

//#define RUN_CPU_VS_PIO

int main()
{
    // overlock
    //set_sys_clock_khz(100*1000, true);


    stdio_init_all();
    printf("NEW START\n");

    init_screen();
    clear_screen(SCREEN_EMPTY);
    multicore_launch_core1(core1_entry);

    #ifdef RUN_CPU_VS_PIO

    // RUN with CPU GPIO
    output_screen_to_gpio(); // endless loop

    #else
    
    // RUN with PIO
    //float pio_1_freq = 4.7*16666;//4.7*16666/x; // TODO: adjust for real operation
    float pio_1_freq = 4.7*16666*5;
    PIO pio_1 = pio1;

    uint sm_1 = pio_claim_unused_sm(pio_1, true);

    uint offset_1 = pio_add_program(pio_1, &readfbzmod_program);

    float div_1 = (float)clock_get_hz(clk_sys) / pio_1_freq;

    readfbzmod_program_init(pio_1, sm_1, offset_1, Z_MOD_PIN, V_PIN, div_1);

    pio_sm_set_enabled(pio_1, sm_1, true);

    while(1)
    {
        // Frame Trigger
        gpio_put(TRIG_PIN, true);
        gpio_put(TRIG_PIN, false);
        for(int i = 0; i < HOR_MAX; i++)
        {
            pio_sm_put_blocking(pio_1, sm_1, screen[i]);
        }
        
    }
    #endif

    return 0;
}




