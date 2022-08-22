
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

#include "verticaldef.pio.h"
#include "readfbzmod.pio.h"

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

//#define MAIN_RUN
#define DEBUG_RUN
#define DEBUG_PIO_OR_CPU

void pio_irq_function()
{
    printf("Hello from interrupt %d\n", get_random_number());
}

int main()
{
    // overlock
    //set_sys_clock_khz(100*1000, true);


    stdio_init_all();
    printf("NEW START\n");

    init_screen();
    clear_screen(SCREEN_EMPTY);
    //print_string("TEST", 0, 0, SCAL, NON_INVERTED);

    #ifdef MAIN_RUN
    multicore_launch_core1(core1_entry);
    const uint LED_PIN = PICO_DEFAULT_LED_PIN;
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    init_screen();
    fill_screen();

    output_screen_to_gpio(); // endless loop
    #endif

    // Testing Vertical deflection only (because of high jitter)
    #ifdef DEBUG_RUN



    #ifdef DEBUG_PIO_OR_CPU
    multicore_launch_core1(core1_entry);


    float pio_0_freq = 4.7*16666; // TODO: adjust for real operation
    //float pio_1_freq = 4.7*16666;//4.7*16666/x; // TODO: adjust for real operation
    float pio_1_freq = 4.7*16666*3;
    PIO pio_0 = pio0;
    PIO pio_1 = pio1;

    uint sm_0 = pio_claim_unused_sm(pio_0, true);
    uint sm_1 = pio_claim_unused_sm(pio_1, true);

    uint offset_0 = pio_add_program(pio_0, &verticaldef_program);
    uint offset_1 = pio_add_program(pio_1, &readfbzmod_program);

    float div_0 = (float)clock_get_hz(clk_sys) / pio_0_freq;
    float div_1 = (float)clock_get_hz(clk_sys) / pio_1_freq;
    //float div = 

    //verticaldef_program_init(pio_0, sm_0, offset_0, V_PIN, div_0);
    readfbzmod_program_init(pio_1, sm_1, offset_1, Z_MOD_PIN, V_PIN, div_1);

    //pio_sm_set_enabled(pio_0, sm_0, true);
    pio_sm_set_enabled(pio_1, sm_1, true);

    //uint32_t line = 0b01010101010101010101010101010101;
    uint32_t line = 0b00110101010101010101010101010100;
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
    #else
    while(1)
    {
        gpio_put(V_PIN, true);
        busy_wait_at_least_cycles(50*x);
        gpio_put(V_PIN, false);
        busy_wait_at_least_cycles(6720*x);
    }
    #endif
    #endif

    return 0;
}




