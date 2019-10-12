#include "init.h"


void dt_init (void) {
struct SEGMENT_DESCRIPTOR *gdt = (struct SEGMENT_DESCRIPTOR *) OFFSET_GDT;
struct GATE_DESCRIPTOR    *idt = (struct GATE_DESCRIPTOR    *) OFFSET_IDT;
int i;
 for (i = 0; i <= LIMIT_GDT / 8; i++) {
   dt_gdtset(gdt + i, 0, 0, 0);
 }
dt_gdtset(gdt + 1, 0xffffffff, 0x00000000, AR_CODE32_ER); // 0x08 커널코드 셀렉터
dt_gdtset(gdt + 2, 0xffffffff, 0x00000000, AR_DATA32_RW); // 0x10 커널데이터 셀렉터
dt_gdtset(gdt + 3, 0xffffffff, 0x00000000, AR_CODE32_ER); 
dt_gdtset(gdt + 4, 0xffffffff, 0x00000000, AR_DATA32_RW); 


struct SYSINFO *si = (struct SYSINFO *)SYSINFO_OFFSET;
struct TASK_MASTER *tm = (struct TASK_MASTER *)&(si->taskmaster);
dt_gdtset(gdt + 5, 103, &(tm->dummytss), AR_TSS32); // #1 task partition
dt_gdtset(gdt + 6, 103, &(tm->dummytss), AR_TSS32); // #2 task partition

load_gdtr(LIMIT_GDT, OFFSET_GDT);
 for (i = 0; i <= LIMIT_IDT / 8; i++) {
   dt_idtset(idt + i, 0, 0, 0);
 }
load_idtr(LIMIT_IDT, OFFSET_IDT);
dt_idtset(idt + 0x30, (int) int_handler_30, 1 * 8, AR_INTGATE32); // 타이머 인터럽트
dt_idtset(idt + 0x31, (int) int_handler_31, 1 * 8, AR_INTGATE32); // 키보드 인터럽트

dt_idtset(idt + 0x79, (int) syscall_sub, 1 * 8, AR_INTGATE32); // 편의를 위한 시스콜
}

void dt_gdtset(struct SEGMENT_DESCRIPTOR *sd, unsigned int limit, int base, int ar) {
if (limit > 0xfffff) {
 ar |= 0x8000; /* G_bit = 1 */
 limit /= 0x1000;
}
sd->limit_low    = limit & 0xffff;
sd->base_low     = base & 0xffff;
sd->base_mid     = (base >> 16) & 0xff;
sd->access_right = ar & 0xff;
sd->limit_high   = ((limit >> 16) & 0x0f) | ((ar >> 8) & 0xf0);
sd->base_high    = (base >> 24) & 0xff;
}

void dt_idtset(struct GATE_DESCRIPTOR *gd, int offset, int selector, int ar) {
gd->offset_low   = offset & 0xffff;
gd->selector     = selector;
gd->dw_count     = (ar >> 8) & 0xff;
gd->access_right = ar & 0xff;
gd->offset_high  = (offset >> 16) & 0xffff;
}


int init_pic()
{
io_out8(PIC0_IMR,  0xff  );
io_out8(PIC1_IMR,  0xff  );

io_out8(PIC0_ICW1, 0x11  );
io_out8(PIC0_ICW2, 0x30  ); /* IRQ0-7 */
io_out8(PIC0_ICW3, 1 << 2); /* PIC1는 IRQ2에서 접속 */
io_out8(PIC0_ICW4, 0x01  );

io_out8(PIC1_ICW1, 0x11  );
io_out8(PIC1_ICW2, 0x38  ); /* IRQ8-15 */
io_out8(PIC1_ICW3, 2     );
io_out8(PIC1_ICW4, 0x01  );

io_out8(PIC0_IMR,  0xfb  ); /* 11111011 PIC1 이외는 모두 금지 */
io_out8(PIC1_IMR,  0xff  );

return 1;
}


int init_pit()
{
io_out8(PIT_CTRL, 0x34);
io_out8(PIT_CNT0, 0x9c);
io_out8(PIT_CNT0, 0x2e);
// 타이머 주기 = 0.01초
return 1;
}


