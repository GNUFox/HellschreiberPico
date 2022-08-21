#ifndef __HELL_CHARS_H__
#define __HELL_CHARS_H__

#define HELL_CHAR_MAX 37
#define HELL_CHAR_COL_MAX 7

extern const int hell_bitmaps[HELL_CHAR_MAX][HELL_CHAR_COL_MAX];
int convert_to_hell_table(char c);

#endif