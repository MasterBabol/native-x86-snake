#include "../sysinfo.h"
#include "../init/init.h"

int settss ( struct Task *task, ul startoffset, ul arg, ul stackblocksize, ul cs, ul ds );
void task_end ();

void init_tasks ()
{
struct SYSINFO *si = (struct SYSINFO *)SYSINFO_OFFSET;
struct MEMORY_MASTER *mm = (struct MEMORY_MASTER *) &(si->memmaster);
struct TASK_MASTER *tm = (struct TASK_MASTER *) &(si->taskmaster);

struct Task *idletask = m4kalloc ( mm, 1 ); // idle task 로 사용.. 절대 터미네이트 되면 안된다.
settss ( idletask, (ul)&idle_task, 0, 16, 0x8, 0x10 );
idletask->backtask = idletask->nexttask = idletask;
idletask->running = 0;
idletask->status = 1;
idletask->priority = 1;
tm->task_endptr = (ul) idletask;
tm->task_totalnum = 1;
tm->task_executing = (ul) idletask;
load_tr (5*8);
tm->task_partition = 0;
tm->task_waiting = 0;
tm->task_isswitching = 0;
}

int thread_create (struct TASK_MASTER *tm, ui offset, ul arg, char priority, ul stacksize)
{
struct SYSINFO *si = (struct SYSINFO *)SYSINFO_OFFSET;
struct MEMORY_MASTER *mm = (struct MEMORY_MASTER *) &(si->memmaster);
struct Task *newtask = m4kalloc ( mm, 1 );
if (newtask == 0) return 0;
if (settss ( newtask, offset, arg, stacksize, 0x8, 0x10 ) == 0) { m4kfree ( mm, (ul)newtask, 1 ); return 0; }
newtask->priority = priority;
newtask->running = 0;
newtask->status = 1;
tm->task_totalnum += 1;

struct Task *endtask = (struct Task *) tm->task_endptr;
newtask->nexttask = endtask->nexttask;
newtask->backtask = endtask;

endtask->nexttask->backtask = newtask;
endtask->nexttask = newtask;
tm->task_endptr = (ul) newtask;
return 1;
}

int settss ( struct Task *task, ul startoffset, ul arg, ul stackblocksize, ul cs, ul ds )
{
int i;
task->stacksize = stackblocksize;
struct SYSINFO *si = (struct SYSINFO *) SYSINFO_OFFSET;
struct MEMORY_MASTER *mm = (struct MEMORY_MASTER *) &(si->memmaster);
ul *stacktop = (m4kalloc (mm, stackblocksize) + stackblocksize * 0x1000);
if (stacktop == 0) return 0;
stacktop-= 2;
*(stacktop) = &task_end;
*(stacktop + 1) = arg;
task->stackoffset = ((ul)stacktop);
task->tss.esp = task->tss.ebp = task->stackoffset;
task->tss.edi = task->tss.esi = task->tss.eax = task->tss.ebx = task->tss.ecx = task->tss.edx = 0;
task->tss.eflags = 0x00000202;
task->tss.eip = startoffset;
task->tss.cs = cs;
task->tss.ds = task->tss.es = task->tss.fs = task->tss.gs = task->tss.ss = ds;
return 1;
}

char a[] = {'a', 0};
void task_end ()
{
struct SYSINFO *si = (struct SYSINFO *) SYSINFO_OFFSET;
struct MEMORY_MASTER *mm = (struct MEMORY_MASTER *) &(si->memmaster);
struct TASK_MASTER *tm = (struct TASK_MASTER *) &(si->taskmaster);
tm->task_isswitching = 1;
struct Task *toeragetask = (struct Task *) getexecutingtask (tm);
toeragetask->backtask->nexttask = toeragetask->nexttask;
toeragetask->nexttask->backtask = toeragetask->backtask;
m4kfree (mm, (toeragetask->stackoffset+4) - (toeragetask->stacksize * 0x1000), toeragetask->stacksize);
if (tm->task_endptr == (ul) toeragetask) tm->task_endptr = (ul)toeragetask->nexttask;
m4kfree (mm, toeragetask, 1);
tm->task_totalnum -= 1;
tm->task_isswitching = 0;
while (1);
}

ul getexecutingtask ( struct TASK_MASTER *tm )
{
return (ul)(tm->task_executing);
}

void scheduler ()
{
struct SYSINFO *si = (struct SYSINFO *) SYSINFO_OFFSET;
struct TASK_MASTER *tm = (struct TASK_MASTER *) &(si->taskmaster);
struct SEGMENT_DESCRIPTOR *gdt = (struct SEGMENT_DESCRIPTOR *) OFFSET_GDT;

if (tm->task_isswitching == 1) return;
tm->task_isswitching = 1;
if (tm->task_waiting != 0) { tm->task_isswitching = 0; tm->task_waiting -= 1; return; }

struct Task *thistask = (struct Task *) tm->task_executing;
struct Task *nexttask = (struct Task *) thistask->nexttask;
if (nexttask->status != 1) { tm->task_isswitching = 0; return; }
tm->task_executing = nexttask;

 switch (tm->task_partition)
  {
  case 0:
    tm->task_partition = 1;
    nexttask->backtask->running = 0;
    nexttask->running = 1;
    tm->task_waiting = nexttask->priority;
    dt_gdtset(gdt + 6, 103, (ui)&(nexttask->tss), AR_TSS32); // #2 task partition
    tm->task_isswitching = 0;
    switchtask (0, 6*8);
    return;
  case 1:
    tm->task_partition = 0;
    nexttask->backtask->running = 0;
    nexttask->running = 1;
    tm->task_waiting = nexttask->priority;
    dt_gdtset(gdt + 5, 103, (ui)&(nexttask->tss), AR_TSS32); // #1 task partition
    tm->task_isswitching = 0;
    switchtask (0, 5*8);
    return;
  }
}

void skipthisthread ()
{
struct SYSINFO *si = (struct SYSINFO *) SYSINFO_OFFSET;
struct TASK_MASTER *tm = (struct TASK_MASTER *) &(si->taskmaster);
tm->task_isswitching = 1;
tm->task_waiting = 0;
tm->task_isswitching = 0;
}

void waitforme()
{
struct SYSINFO *si = (struct SYSINFO *) SYSINFO_OFFSET;
struct TASK_MASTER *tm = (struct TASK_MASTER *) &(si->taskmaster);
tm->task_isswitching = 1;
}

void goahead()
{
struct SYSINFO *si = (struct SYSINFO *) SYSINFO_OFFSET;
struct TASK_MASTER *tm = (struct TASK_MASTER *) &(si->taskmaster);
tm->task_isswitching = 0;
}

//////////////////////////////////////////////////////////////////////////////////////
// 태스크 초기화가 이루어 진후 이 밑의 idle_task 함수가 CPU를 얻는다.
// 따라서 다음 코드들을 실행하려면 여기서 thread_create를 해야한다.
//////////////////////////////////////////////////////////////////////////////////////

void idle_task()
{
text_out (0, 2, "0 Idle task : I have control.", 3);
struct SYSINFO *si = (struct SYSINFO *) SYSINFO_OFFSET;
struct TASK_MASTER *tm = (struct TASK_MASTER *) &(si->taskmaster);
delay (50);
cls();

char select = 0, cleared = 0;
while (1)
 {
  if (!cleared) { cls (); cleared = 1; }
  text_out (30, 0, "-- Select Game --", 2);
  text_out (30, 2, "1. Snake ", 3);
  select = getch();
  switch (select)
  {
  case '1':
   snakemain();
   cleared = 0;
   break;
  }
 }

while (1)
 skipthisthread();
}
