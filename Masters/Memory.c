#include "../sysinfo.h"

#define EFLAGS_AC_BIT		0x00040000
#define CR0_CACHE_DISABLE	0x60000000

unsigned int check_memory(unsigned int start, unsigned int end)
{
char flg486 = 0;
unsigned int eflg, cr0, i;

eflg = io_load_eflags();
eflg |= EFLAGS_AC_BIT;
io_store_eflags(eflg);
eflg = io_load_eflags();
if ((eflg & EFLAGS_AC_BIT) != 0)
{
flg486 = 1;
}
eflg &= ~EFLAGS_AC_BIT;
io_store_eflags(eflg);

if (flg486 != 0) {
cr0 = load_cr0();
cr0 |= CR0_CACHE_DISABLE;
store_cr0(cr0);
}

i = memtest(start, end);

if (flg486 != 0)
{
cr0 = load_cr0();
cr0 &= ~CR0_CACHE_DISABLE;
store_cr0(cr0);
}

return i;
}

int init_memory (struct MEMORY_MASTER *mm, ul poolstart)
{
int i;
for (i=0;i<MEMORY_TOTAL_CLUSTERS;i++) mm->mem_cluster32[i]=0;
mm->mem_allocated=0;
mm->mem_poolstart = poolstart;
ui endoff = check_memory(poolstart, 0xdfffffff);
if (endoff-poolstart == 0) return 0;
mm->mem_total = endoff - poolstart;
mm->mem_blocklimit = (endoff - poolstart) / 0x1000;
mm->mem_frees = mm->mem_blocklimit;
return 1;
}

void *m4kalloc (struct MEMORY_MASTER *mm, int block)
{
if (mm->mem_frees < block) return 0;
int counter, test, found=0, alloc;
for (counter=0;counter<mm->mem_blocklimit - block + 1;counter++)
 {
 found=1;
 for (test=0;test<block;test++)
  {
  if (getstat(mm, counter+test)==1) { found=0; break; }
  }
 if (found == 0) { counter += test; continue; }
 for (alloc=0;alloc<block;alloc++) setstat (mm, counter+alloc);
 mm->mem_allocated+=block;
 mm->mem_frees-=block;
 return (void *)(mm->mem_poolstart + counter * 0x1000);
 }
return 0;
}

int m4kfree (struct MEMORY_MASTER *mm, ul offset, int block)
{
if (mm->mem_allocated < block) return 0;
int test, found=1, freeing;
int sblock = (offset - mm->mem_poolstart) / 0x1000;
for (test=0;test<block;test++)
 {
 if (getstat(mm, sblock + test)==0) { found = 0; break; }
 }
if (found == 0) { return 0; }
for (freeing=0;freeing<block;freeing++) clrstat (mm, sblock + freeing);
mm->mem_allocated-=block;
mm->mem_frees+=block;
return 1;
}

int getstat (struct MEMORY_MASTER *mm, int count)
{
int tofindc=count/8;
int tofindb=count%8;
char thebit=(mm->mem_cluster32[tofindc] >> (8 - 1 - tofindb)) & 0x1;
return (int)thebit;
}

void setstat (struct MEMORY_MASTER *mm, int count)
{
int towritec=count/8;
int towriteb=count%8;
mm->mem_cluster32[towritec] |= (1 << (8 - 1 - towriteb));
}

void clrstat (struct MEMORY_MASTER *mm, int count)
{
int towritec=count/8;
int towriteb=count%8;
mm->mem_cluster32[towritec] &= ~(1 << (8 - 1 - towriteb));
}


