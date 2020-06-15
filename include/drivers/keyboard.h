#ifndef _KEYBOARD_H
#define _KEYBOARD_H

#include <stdio.h>
#include <types.h>
#include <isr.h>


static void keyboard_callback(registers_t regs);
void init_keyboard(uint32_t frequency);

#endif
