#include "asmfunc.h"

#include "init/init.h"

#include "sysinfo.h"

// 진입점
void kernel_main ()
{
dt_init();
init_pic();
io_sti();
init_pit();

struct SYSINFO *si = (struct SYSINFO *) SYSINFO_OFFSET;
struct MEMORY_MASTER *mm = (struct MEMORY_MASTER *) &(si->memmaster);
struct TASK_MASTER *tm = (struct TASK_MASTER *) &(si->taskmaster);
struct TIMER_MASTER *tim = (struct TIMER_MASTER *) &(si->timermaster);
struct SCREEN_MASTER *sm = (struct SCREEN_MASTER *) &(si->screenmaster);
struct KEYBOARD_MASTER *km = (struct KEYBOARD_MASTER *) &(si->keymaster);

init_timer(tim);
init_memory (mm, 0x1000000);
init_keyboard (km);
init_tasks (tm);
io_out8(PIC0_IMR,  0xf8  ); /* 11111000  PIC TIMER KEYBOARD ENABLE */
// 여기 부터 스레드가 실행되는데, 실행되고나면 kernel_main은 잊혀진다.
// 잠시 공백기간이 있으나 곧 사라짐
}



