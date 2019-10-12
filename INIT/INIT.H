#pragma once
#include "../asmfunc.h"
#include "../types.h"
#include "../sysinfo.h"
#include "../masters/others.h"

// changeable
#define OFFSET_GDT		0x00100000
#define OFFSET_IDT		0x0011f800
#define LIMIT_GDT			0xffff
#define LIMIT_IDT			0x7ff
//
#define AR_DATA32_RW		0x4092
#define AR_CODE32_ER		0x409a
#define AR_INTGATE32		0x008e
#define AR_TSS32			0x0089

struct SEGMENT_DESCRIPTOR {
 short limit_low, base_low;
 char base_mid, access_right;
 char limit_high, base_high;
};
struct GATE_DESCRIPTOR {
 short offset_low, selector;
 char dw_count, access_right;
 short offset_high;
};

void	dt_init (void); // ok // 
void	dt_gdtset (struct SEGMENT_DESCRIPTOR *sd, ui limit, int base, int ar); // ok
void	dt_idtset (struct GATE_DESCRIPTOR *gd, int offset, int selector, int ar); // ok

#define PIC0_ICW1		0x0020
#define PIC0_OCW2		0x0020
#define PIC0_IMR			0x0021
#define PIC0_ICW2		0x0021
#define PIC0_ICW3		0x0021
#define PIC0_ICW4		0x0021
#define PIC1_ICW1		0x00a0
#define PIC1_OCW2		0x00a0
#define PIC1_IMR			0x00a1
#define PIC1_ICW2		0x00a1
#define PIC1_ICW3		0x00a1
#define PIC1_ICW4		0x00a1

#define PIT_CTRL	0x0043
#define PIT_CNT0	0x0040

#define PORT_KEYDAT    0x0060

int init_pic ();
int init_pit ();



