#ifndef __HELL_RX_H__
#define __HELL_RX_H__

#include "hell_chars.h"

#ifdef HELL_80
  #define delay_multiplier 500
#endif
#ifdef FSK_HELL
  #define delay_multiplier 1010
#endif

#define _HELL_RX_PIN 26
#define HELL_PIXEL_DELAY 7*1170 // todo clean up


void init_hell_rx();
void receive_to_fb();

#endif
