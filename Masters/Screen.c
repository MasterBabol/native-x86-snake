#include "../sysinfo.h"


void init_screen( struct SCREEN_MASTER *sm, char *vram, ul xsize, ul ysize )
{
sm->video_ram_ptr = vram;
sm->video_x_size = xsize;
sm->video_y_size = ysize;

init_palette();
}

void init_palette()
{
static unsigned char table_rgb[16 * 3] = 
 {
 0x00, 0x00, 0x00,	/*  0:흑 */
 0xff, 0x00, 0x00,	/*  1:밝은 빨강 */
 0x00, 0xff, 0x00,	/*  2:밝은 초록 */
 0xff, 0xff, 0x00,	/*  3:밝은 황색 */
 0x00, 0x00, 0xff,	/*  4:밝은 파랑 */
 0xff, 0x00, 0xff,	/*  5:밝은 보라색 */
 0x00, 0xff, 0xff,	/*  6:밝은 물색 */
 0xff, 0xff, 0xff,	/*  7:흰색 */
 0xc6, 0xc6, 0xc6,	/*  8:밝은 회색 */
 0x84, 0x00, 0x00,	/*  9:어두운 빨강 */
 0x00, 0x84, 0x00,	/* 10:어두운 초록 */
 0x84, 0x84, 0x00,	/* 11:어두운 황색 */
 0x00, 0x00, 0x84,	/* 12:어두운 파랑 */
 0x84, 0x00, 0x84,	/* 13:어두운 보라색 */
 0x00, 0x84, 0x84,	/* 14:어두운 물색 */
 0x84, 0x84, 0x84	/* 15:어두운 회색 */
 };
set_palette(0, 15, table_rgb);
return;
}

void set_palette(int start, int end, unsigned char *rgb)
{
int i;
io_cli();
io_out8(0x03c8, start);
for (i = start; i <= end; i++)
 {
 io_out8(0x03c9, rgb[0] / 4);
 io_out8(0x03c9, rgb[1] / 4);
 io_out8(0x03c9, rgb[2] / 4);
 rgb += 3;
 }
io_sti();
return;
}

void putfont8(char *vram, int xsize, int x, int y, char c, char *font)
{
int i;
char *p, d;
for (i = 0; i < 16; i++) {
p = vram + (y + i) * xsize + x;
d = font[i];
if ((d & 0x80) != 0) { p[0] = c; }
if ((d & 0x40) != 0) { p[1] = c; }
if ((d & 0x20) != 0) { p[2] = c; }
if ((d & 0x10) != 0) { p[3] = c; }
if ((d & 0x08) != 0) { p[4] = c; }
if ((d & 0x04) != 0) { p[5] = c; }
if ((d & 0x02) != 0) { p[6] = c; }
if ((d & 0x01) != 0) { p[7] = c; }
}
return;
}

