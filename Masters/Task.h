#ifndef mtasking
#define mtasking

#include "../asmfunc.h"
#include "../types.h"


struct TSS32
{
int backlink, esp0, ss0, esp1, ss1, esp2, ss2, cr3;
int eip, eflags, eax, ecx, edx, ebx, esp, ebp, esi, edi;
int es, cs, ss, ds, fs, gs;
int ldtr, iomap;
};

struct Task
{
struct TSS32 tss;
char priority, status, running, reservedT;
ul stackoffset, stacksize;
ul reserved;
struct Task *backtask;
struct Task *nexttask;
};

struct TASK_MASTER
{
ul task_endptr;
ul task_totalnum;
ul task_executing;
ul task_partition;
ul task_isswitching;
ul task_waiting;
struct TSS32 dummytss;
};

void init_tasks ();
int thread_create (struct TASK_MASTER *tm, ui offset, ul arg, char priority, ul stacksize);
void skipthisthread ();
ul getexecutingtask ( struct TASK_MASTER *tm );

void scheduler ();
void idle_task();

void waitforme();
void goahead();

#endif
