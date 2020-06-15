#ifndef _HEAP_H
#define _HEAP_H

#include <types.h>

typedef struct chunk_t{
	short header=0x0a0b;
	long size;
    //unsigned long size;
	char free=0; //0 free 1 occupied
	short foot =0x0e0e;
	unsigned char *prev=0;
	unsigned char *next=0;
}__attribute__((packed));

unsigned char *malloc(int amount);
uint32_t *kmalloc_a(long amount, uint32_t * phys);
void initheap(unsigned char * root, unsigned char *end);
unsigned char *process(chunk_t chunk);
void free(unsigned char *y);
void pointers();
#endif
