#pragma once
#include "types.h"

extern void setcursor ( int x, int y );

extern void io_hlt();
extern void io_cli();
extern void io_sti();
extern void io_stihlt(void);

extern int io_load_eflags();
extern void io_store_eflags(int eflags);

extern void load_gdtr(int limit, int addr);
extern void load_idtr(int limit, int addr);

extern int io_in8(int port);
extern void io_out8(int port, int data);

extern int load_cr0(void);
extern void store_cr0(int cr0);
extern void load_tr(int tr);

extern unsigned int memtest (unsigned int start, unsigned int end);
extern int between ( unsigned long target, unsigned long first, unsigned long last );
extern unsigned long sizeret ( unsigned long first, unsigned long last );

extern void switchtask (int eip, int cs);
extern int getds();
extern void setds(int n);

extern void int_handler_30();
extern void int_handler_31();
extern void int_handler_79();

