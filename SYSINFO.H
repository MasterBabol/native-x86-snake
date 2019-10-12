#ifndef sysinfo
#define sysinfo

#include "masters/memory.h"
#include "masters/task.h"
#include "masters/screen.h"
#include "masters/keyboard.h"

#include "masters/others.h"

#define SYSINFO_OFFSET 0x00200000

struct SYSINFO
{
struct MEMORY_MASTER memmaster;
struct TASK_MASTER taskmaster;
struct TIMER_MASTER timermaster;
struct SCREEN_MASTER screenmaster;
struct KEYBOARD_MASTER keymaster;
};

#endif
