#ifndef screeni
#define screeni
#include "../types.h"

struct SCREEN_MASTER
{
char *video_ram_ptr;
ul video_x_size, video_y_size;
ul video_mode_data;
};

void init_screen( struct SCREEN_MASTER *sm, char *vram, ul xsize, ul ysize );
void init_palette();
void set_palette(int start, int end, unsigned char *rgb);
void putfont8(char *vram, int xsize, int x, int y, char c, char *font);

#endif
