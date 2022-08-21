#include "osc_screen.h"
#include "hell_chars.h"
#include "pico/stdlib.h"

//extern uint64_t screen[HOR_MAX];
uint64_t screen[HOR_MAX];

void init_screen()
{
    for(int i = 0; i < HOR_MAX; i++)
    {
        screen[i] = 0;
    }
    gpio_init(V_PIN);
    gpio_init(TRIG_PIN);
    gpio_init(Z_MOD_PIN);

    gpio_set_dir(V_PIN, GPIO_OUT);
    gpio_set_dir(TRIG_PIN, GPIO_OUT);
    gpio_set_dir(Z_MOD_PIN, GPIO_OUT);

    gpio_set_drive_strength(V_PIN, GPIO_DRIVE_STRENGTH_12MA);
}

void fill_screen()
{
    for (int i = 0; i < HOR_MAX; i++)
    {
        for (int j = 0; j < VERT_MAX; j++)
        {
            box(i, j);
            checker_board(i,j);
        }
    }
    place_char(2,2,0);
}

void box(int i, int j)
{
  if(i == 0 || i == HOR_MAX-1 || j == 0 || j == VERT_MAX-1)
  {
    //screen[i] &= ~(1UL << j);
    set_pixel(i, j, 1);
  }
  else
  {
    //screen[i] |= 1UL << j;
    set_pixel(i, j, 0);
  }
}

void checker_board(int i, int j)
{
  if(_inside_bounds(i,j) && _inside_margin(i,j,3))
  {
    if(i % 2)
    {
      if(j % 2)
      {
        set_pixel(i, j, true);
      }
      else
      {
        set_pixel(i, j, false);
      }
    }
    else
    {
      if(j % 2)
      {
        set_pixel(i, j, false);
      }
      else
      {
        set_pixel(i, j, true);
      }
    }
  }
}

void place_char(int i, int j, int c)
{
  if(_inside_bounds(i,j))
  {
    for(int a = 0; a < HELL_CHAR_COL_MAX*2; a++)
    {
      //screen[10+a] = hell_bitmaps[0][a];
      for(int b = 0; b < HELL_CHAR_COL_MAX*2; b++)
      {
        set_pixel(a+10, b, (hell_bitmaps[0][a/2] >> b/2) & 1UL);
      }
    }
  }
}

int _inside_bounds(int i, int j)
{
    return i > 0 && j > 0 && i < HOR_MAX && j < VERT_MAX;
}

int _inside_margin(int i, int j, int margin)
{
    return i >= margin && j >= margin && i < HOR_MAX-margin && j < VERT_MAX-margin;
}

void set_pixel(int i, int j, int on_off)
{
  if(on_off)
  {
    screen[i] &= ~(1UL << j);
  }
  else
  {
    screen[i] |= 1UL << j;
  }
}

void output_screen_to_gpio()
{
  for (int screen_cnt_hor = 0; screen_cnt_hor < HOR_MAX;screen_cnt_hor++)
  {
    // Charging capacitor for vertical deflection
    gpio_put(V_PIN, true);
    busy_wait_at_least_cycles(50);
    gpio_put(V_PIN, false);

    // draw line twice (HELLSCHREIBER double spiral)
    for (int i = 0; i < 2; i++)
    {
      for (int screen_cnt_vert = VERT_MAX-1; screen_cnt_vert >= 0;screen_cnt_vert--)
      {
        //cur_line = cur_line >> 1;
        //gpio_put(Z_MOD_PIN, (cur_line & 1UL));
        gpio_put(Z_MOD_PIN, !((screen[screen_cnt_hor] >> screen_cnt_vert) & 1UL));
        busy_wait_at_least_cycles(75);
         // Dies ist die Zeit für ein Pixel (ca.). Durch den 10-maligen Bildaufbau werden die 7ms eines Hell-Pixels erzeugt
                 // Formel: delay*pixel(pro Spalte)*2(wegen DoubleRes)+20µs(Vertikalbalenkungszeit)*10(Spalten) 11.7857*7*2+20*10
                 // ~ (sollte eig. 7ms ergeben sind aber 3499µs. auf oszi sind das aber ca. 7 - 7,9 ms ==> HÄ???)
      }
    }
    // vertical blanking
    gpio_put(Z_MOD_PIN, true); // Austastlücke vertikal
  }

  // Trigger impulse
  gpio_put(TRIG_PIN, true);
  //busy_wait_us(1);
  gpio_put(TRIG_PIN, false);
}