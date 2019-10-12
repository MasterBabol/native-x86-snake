#ifndef kbd
#define kbd

#include "../asmfunc.h"
#include "../types.h"
#include "others.h"
#include "../init/init.h"

struct KEYBOARD_MASTER
{
char isinput;
char key_queue_data[255];
struct queue_buffer key_queue;
char keyinput_enabled;
short key_recent;
};

void init_keyboard( struct KEYBOARD_MASTER *km );
char getch();
int kbhit ();
//void flushkeybuf();
//void getstr( char *dest );

void key_int();

#endif
