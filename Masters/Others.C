#include "../sysinfo.h"

void text_out (int x, int y, char *str, char attribute)
{
char *vram=0xb8000 + (y * 2 * 80) + (x * 2);
for (;*str;) {
 *vram = *str;
 vram++;
 *vram = attribute;
 vram++;
 str++;
 }
}

void cls ()
{
char *vram=0xb8000;
int i=0;
for (;i<80*25*2;i++) {
  vram[i] = ' ';
  i++;
  vram[i] = 7;
 }
}

int init_queue ( struct queue_buffer *buf, char *memory, int size )
{
buf->bufferpos = memory;
buf->head=0;
buf->tail=0;
buf->datan_in=0;
buf->size = size;
return 1;
}

int dataput ( struct queue_buffer *buf, char data )
{
if (buf->datan_in >= buf->size) return 0;
*(buf->bufferpos + buf->head) = data;
if (buf->size - 1 > buf->head) {
 buf->head += 1;
 }
 else {
 buf->head = 0;
 }
buf->datan_in++;
return 1;
}

int dataget ( struct queue_buffer *buf, char *data )
{
if (buf->datan_in == 0) return 0;
*(data) = *(buf->bufferpos + buf->tail);
if (buf->size - 1 > buf->tail) {
 buf->tail += 1;
 }
 else {
 buf->tail = 0;
 }
buf->datan_in--;
return 1;
}


void init_timer ( struct TIMER_MASTER *tim )
{
tim->timer_tickcount = 0;
tim->timer_totaltime = 0;
}

void timer_int ()
{
io_out8(PIC0_OCW2, 0x60);
struct SYSINFO *si = (struct SYSINFO *) SYSINFO_OFFSET;
struct TIMER_MASTER *tim = (struct TIMER_MASTER *) &(si->timermaster);
tim->timer_tickcount += 1;
tim->timer_totaltime += 1;

scheduler ();
}

ul gettickcount ()
{
struct SYSINFO *si = (struct SYSINFO *) SYSINFO_OFFSET;
struct TIMER_MASTER *tim = (struct TIMER_MASTER *) &(si->timermaster);
return tim->timer_tickcount;
}

void delay (ul time)
{
ul tick=gettickcount() + time;
while(gettickcount() < tick) {
 skipthisthread();
 }
}

int syscall_sub (int eax, int ebx, int ecx, int edx)
{
struct SYSINFO *si = (struct SYSINFO *) SYSINFO_OFFSET;
struct MEMORY_MASTER *mm = (struct MEMORY_MASTER *) &(si->memmaster);
struct TASK_MASTER *tm = (struct TASK_MASTER *) &(si->taskmaster);
switch (eax)
 {
 case 0:
 return (thread_create (tm, (ul)ebx, (ul)ecx, 1, 16));
 }
}

