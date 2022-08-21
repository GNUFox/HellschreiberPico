
#include <stdio.h>
#include "pico/stdlib.h"

#include "osc_screen.h"

int main()
{   
    const uint LED_PIN = PICO_DEFAULT_LED_PIN;
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    stdio_init_all();

    init_screen();
    fill_screen();



    while (true)
    {
        output_screen_to_gpio();
        //gpio_put(LED_PIN, !gpio_get(LED_PIN));
    }
    return 0;
}
