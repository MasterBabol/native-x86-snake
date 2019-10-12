#ifndef mem
#define mem

#include "../asmfunc.h"
#include "../types.h"

#define MEMORY_TOTAL_CLUSTERS 1024*128

struct MEMORY_MASTER
{
char mem_cluster32[MEMORY_TOTAL_CLUSTERS-1];
ul mem_frees;
ul mem_allocated;
ul mem_poolstart;
ul mem_blocklimit;
ul mem_total;
};

unsigned int check_memory(unsigned int start, unsigned int end);
int init_memory (struct MEMORY_MASTER *mm, ul poolstart);
void *m4kalloc (struct MEMORY_MASTER *mm, int block);
int m4kfree (struct MEMORY_MASTER *mm, ul offset, int block);

#endif
