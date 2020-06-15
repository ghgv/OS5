#ifndef MULTITASK_H
#define MULTITASK_H

#include <timer.h>
#include <stdio.h>
#include <kheap.h>
#include <isr.h>

typedef struct CPUState
{
        uint32_t ds;
        
        uint32_t edi;    
        uint32_t esi;    
        uint32_t ebp;    
        uint32_t esp;    
        uint32_t ebx;    
        uint32_t edx;    
        uint32_t ecx;    
        uint32_t eax;    
        /*
        common::uint32_t gs;
        common::uint32_t fs;
        common::uint32_t es;
        common::uint32_t ds;
        */
        uint32_t int_no;
        uint32_t error;

        uint32_t eip;
        uint32_t cs;
        uint32_t eflags;
        uint32_t useresp;
        uint32_t ss;        
} CPUState_t __attribute__((packed));

typedef struct TaskType{
        unsigned short PID;
        CPUState_t* stack;
        char status;
    
}TaskType_t;



int scheduler(registers_t regs );
void init_multitask();
void Task_create(unsigned short ID, unsigned char prio, void * ptr);

#endif
