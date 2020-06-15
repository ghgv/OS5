#ifndef _TTY_H
#define _TTY_H

#include <types.h>
#include <stdio.h>
#include <stdlib.h>
#include <kheap.h>

#define LSH_RL_BUFSIZE 20
char *lsh_read_line(void);

void lsh_loop(void);

#endif
