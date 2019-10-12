#ifndef others
#define others

void text_out (int x, int y, char *str, char attribute);
void cls ();

struct queue_buffer
{
char *bufferpos;
int size;
int head, tail;
int datan_in;
};

int init_queue ( struct queue_buffer *buf, char *memory, int size );
int dataput ( struct queue_buffer *buf, char data );
int dataget ( struct queue_buffer *buf, char *data );

struct TIMER_MASTER
{
ul timer_tickcount;
ul timer_totaltime;
};

void init_timer ( struct TIMER_MASTER *tim );
ul gettickcount();
void timer_int();
void delay (ul time);

int syscall_sub (int eax, int ebx, int ecx, int edx);

#endif
