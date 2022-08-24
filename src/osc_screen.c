#include "osc_screen.h"
#include "hell_chars.h"
#include "pico/stdlib.h"
#include <ctype.h>
#include <math.h>

uint32_t screen[HOR_MAX];

static uint32_t current_line;

void init_screen()
{
    for(int i = 0; i < HOR_MAX; i++)
    {
        screen[i] = 1;
    }
    gpio_init(V_PIN);
    gpio_init(TRIG_PIN);
    gpio_init(Z_MOD_PIN);

    gpio_set_dir(V_PIN, GPIO_OUT);
    gpio_set_dir(TRIG_PIN, GPIO_OUT);
    gpio_set_dir(Z_MOD_PIN, GPIO_OUT);

    gpio_set_drive_strength(V_PIN, GPIO_DRIVE_STRENGTH_12MA);
}

void clear_screen(int fill)
{
  for(int i = 0; i<HOR_MAX; i++)
  {
    screen[i] = fill;
  }
}


void draw_line(int A_x, int A_y, int B_x, int B_y, int inverted)
{
  int from_x, from_y, to_x, to_y;
  if(A_x > B_x)
  {
    from_x = B_x;
    to_x = A_x;
    from_y = B_y;
    to_y = A_y;
  }
  else
  {
    from_x = A_x;
    to_x = B_x;
    from_y = A_y;
    to_y = B_y;
  }
  float m = 2;
  if (from_x != to_x)
  {
    // non vertical line, m is not infinte
    m = (float)(to_y - from_y) / (to_x - from_x);
  }
  

  int calc_y = 0;
  int calc_x = 0;
  if (m <= 1.0 && m >= -1.0)
  {
    // angle  <= +/- 45 deg.
    for(int i = from_x; i < to_x; i++)
    {
      calc_y = floor(m*i)+from_y;
      set_pixel(i, calc_y, inverted);
    }
  }
  else
  {
    // angle > +/- 45 deg
    m = 1/m;

    if(from_x == to_x) m=0; // vertical line
    if(to_y < from_y)
    {
      for(int i = to_y; i < from_y; i++)
      {
        calc_x = to_x+floor(m*i);
        set_pixel(calc_x, i, inverted);
      }
    }
    else
    {
      for(int i = from_y; i < to_y; i++)
      {
        calc_x = from_x+floor(m*i);
        set_pixel(calc_x, i, inverted);
      }
    } 
  }
}

void box(int pos_x, int pos_y, int size_x, int size_y, int border_thick, int inverted)
{
  draw_line(pos_x, pos_y, pos_x+size_x,pos_y, inverted);
  draw_line(pos_x, pos_y, pos_x,pos_y+size_y, inverted);
  draw_line(pos_x+size_x, pos_y, pos_x+size_x,pos_y+size_y, inverted);
  draw_line(pos_x, pos_y+size_y, pos_x+size_x,pos_y+size_y, inverted);
}



void place_char(int i, int j, char c, int scale, int invert)
{
  c = convert_to_hell_table(c);

  for(int a = 0; a < HELL_CHAR_COL_MAX*scale; a++)
  {
    for(int b = 0; b < HELL_CHAR_COL_MAX*scale; b++)
    {
      set_pixel(a+i, b+j, ((hell_bitmaps[c][a/scale] >> b/scale) & 1UL) ^ (invert == 0));
    }
  }
}

void print_string(char* str, int pos_x, int pos_y, int scale, int inverted)
{
  char* current_c = str;

  while(*current_c != '\0')
  {
    //if(*current_c = '\n') pos_y+=HELL_CHAR_COL_MAX*scale;
    place_char(pos_x, pos_y, *current_c, scale, inverted);
    pos_x+=HELL_CHAR_COL_MAX*scale;
    current_c++;
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
  if (i >= 0 && i < HOR_MAX) // changed bounds
  {
    if(on_off)
    {
      screen[i] &= ~(1UL << j); // TODO don't modify other pixels
    }
    else
    {
      screen[i] |= 1UL << j; // TODO don't modify other pixels
    }
  }
}

/**
 * @brief Deprecated CPU -> GPIO function, replaced by PIO
 * 
 */
void output_screen_to_gpio()
{
  while(1)
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
        //current_line = screen[screen_cnt_hor];
        for (int screen_cnt_vert = VERT_MAX-1; screen_cnt_vert >= 0;screen_cnt_vert--)
        {
          // about 120 clock cycles (120 = 75 + gpio_put)
          //current_line = current_line >> 1;
          //gpio_put(Z_MOD_PIN, !(current_line & 1UL));
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
    busy_wait_at_least_cycles(3);
    gpio_put(TRIG_PIN, false);
  }
}

void print_screen_to_terminal()
{
  for(int i = 0; i < HOR_MAX;i++)   
  {
    for(int j = VERT_MAX-1; j >=0; j--)
    {
      putc(!((screen[i] >> j) & 1UL) ? '0' : ' ', stdout);
    }
    putc('\n', stdout);
  }
}