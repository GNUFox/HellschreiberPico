
#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/multicore.h"

#include "osc_screen.h"

void core1_entry()
{
    const uint LED_PIN = PICO_DEFAULT_LED_PIN;
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    while(1)
    {
        gpio_put(LED_PIN, !gpio_get(LED_PIN));
        sleep_ms(250);
        //place_char(10,0,0);
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


