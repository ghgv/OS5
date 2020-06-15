// timer.c -- Initialises the PIT, and handles clock updates.
// Written for JamesM's kernel development tutorials.

#include <timer.h>
#include <isr.h>
#include <stdio.h>
#include <multitask.h>

uint32_t tick = 0;
uint32_t stack;
//extern "C" void multitask_switch();
extern uint32_t new_esp;

static int timer_callback(registers_t regs)
{ 
   tick++;
   int return_stack;
   registers_t *ptr;
  /* printf("\n Reg ESP:    0x%X\n",regs.esp);
   printf(" Reg No:       %i\n",regs.int_no);
   printf(" Reg ESP:    0x%X\n",regs.esp);*/
   
   return_stack=(int) scheduler(regs);
   ptr=(registers_t *)return_stack;
   printf("Returning ESP:    0x%X EIP: 0x%X\n",return_stack,ptr->eip);
  // printf("Tock: %i\n",tick);
   
   new_esp=(int) return_stack;
   
   return (int) return_stack;
}

void init_timer(uint32_t frequency)
{
   // Firstly, register our timer callback.
   
    register_interrupt_handler(IRQ0, (isr_t )&timer_callback);

   // The value we send to the PIT is the value to divide it's input clock
   // (1193180 Hz) by, to get our required frequency. Important to note is
   // that the divisor must be small enough to fit into 16-bits.
   u32int divisor = 1193180 / frequency;
   divisor = 110 ;

   // Send the command byte.
   outb(0x43, 0x36);

   // Divisor has to be sent byte-wise, so split here into upper/lower bytes.
   u8int l = (u8int)(divisor & 0xFF);
   u8int h = (u8int)( (divisor>>8) & 0xFF );

   // Send the frequency divisor.
   outb(0x40, l);
   outb(0x40, h);
}
