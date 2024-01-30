#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "hell_rx.h"

#include "osc_screen.h"

static int RXZeile;
static int RXSpalte;

void init_hell_rx()
{
    adc_init();
    adc_gpio_init(_HELL_RX_PIN);
    adc_select_input(0);
}

void receive_to_fb()
{
    int RX = adc_read();
    if (RX > 200)
    {
        RX = 1;
    }
    else
    {
        RX = 0;
    }

    //screen[RXSpalte][RXZeile] = RX;            // Speichern im array
    set_pixel(RXSpalte+3, RXZeile+3, RX);
    set_pixel(RXSpalte+3, RXZeile+3+14, RX);

    RXZeile++;
    if (RXZeile >= 14)
    {
        RXZeile = 0;
        RXSpalte++;
        if (RXSpalte >= 80-3)
        {
            RXSpalte = 0;
        }
    }
    sleep_us(HELL_PIXEL_DELAY/2);
}