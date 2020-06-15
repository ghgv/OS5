#include <paging.h>
#include <kheap.h>
#include <stdio.h>

extern uint32_t endcode;
extern uint32_t gdt_entries;
page_directory_t kernel_directory;
void paging()
{
    
         
    unsigned long *page_directory	= (unsigned long *) kmalloc_a(0x1000,0);
    unsigned long *page_table		= (unsigned long *) kmalloc_a(3*0x1000,0);
    //printf("Directory P 0x%X \n",page_directory);
    //printf("Directory Table 0x%X \n",page_table);
 
   
    
    unsigned long address=0; // holds the physical address of where a page is
    unsigned int i;

    // map the first 4MB of memory
    for(i=0; i<3*1024; i++)
    {
        page_table[i] = address | 3; // attribute set to: supervisor level, read/write, present(011 in binary)
       // printf("%X\n",page_table[i]);
        address = address + 4096; // 4096 = 4kb
    };
    // fill the first entry of the page directory
    page_directory[0] = (unsigned long)page_table; 
    // attribute set to: supervisor level, read/write, present(011 in binary)
    page_directory[0] = page_directory[0] | 3;
    // fill the rest of the page directory
    page_directory[1] = (unsigned long) &page_table[2*1024] ;
    page_directory[1] = page_directory[1] | 3;
    for(i=2; i<1024; i++)
    {
        page_directory[i] = 0 | 2; // attribute set to: supervisor level, read/write, not present(010 in binary)
    };
    
    register_interrupt_handler(14, page_fault);
    register_interrupt_handler(13, general_protection);
   // page_directory[1],page_table[1024]);
    switch_page_directory((page_directory_t *)page_directory);
};

void switch_page_directory(page_directory_t *dir)
{
   //current_directory = dir;
   asm volatile("mov %0, %%cr3":: "r"(dir));
   uint32_t cr0;
   asm volatile("mov %%cr0, %0": "=r"(cr0));
   cr0 |= 0x80000000; // Enable paging!
   asm volatile("mov %0, %%cr0":: "r"(cr0));
  
}


void page_fault(registers_t regs)
{
   // A page fault has occurred.
   // The faulting address is stored in the CR2 register.
   uint32_t faulting_address;
   asm volatile("mov %%cr2, %0" : "=r" (faulting_address));

   int present   = !(regs.err_code & 0x1); // Page not present
   int rw = regs.err_code & 0x2;           // Write operation?
   int us = regs.err_code & 0x4;           // Processor was in user-mode?
   int reserved = regs.err_code & 0x8;     // Overwritten CPU-reserved bits of page entry?
   int id = regs.err_code & 0x10;          // Caused by an instruction fetch?

   // Output an error message.
   printf("Page fault! ( ");
   if (present) {printf("present ");}
   if (rw) {printf("read-only ");}
   if (us) {printf("user-mode ");}
   if (reserved) {printf("reserved ");}
   printf(" at )0x%X= %l\n",faulting_address,faulting_address);
   printf("0x%X %x\n",(faulting_address>>22),faulting_address&0x3FFFFF);
   //PANIC("Page fault");
   while(1);
   
}


void general_protection(registers_t regs)
{
   // A page fault has occurred.
   // The faulting address is stored in the CR2 register.
   uint32_t faulting_address;
   asm volatile("mov %%cr2, %0" : "=r" (faulting_address));

   int present   = !(regs.err_code & 0x1); // Page not present
   int rw = regs.err_code & 0x2;           // Write operation?
   int us = regs.err_code & 0x4;           // Processor was in user-mode?
   int reserved = regs.err_code & 0x8;     // Overwritten CPU-reserved bits of page entry?
   int id = regs.err_code & 0x10;          // Caused by an instruction fetch?

   // Output an error message.
   printf("General Protection Fault! ( ");
   if (present) {printf("present ");}
   if (rw) {printf("read-only ");}
   if (us) {printf("user-mode ");}
   if (reserved) {printf("reserved ");}
   printf(" at )0x%X= %l\n",faulting_address,faulting_address);
   printf("0x%X %x\n",(faulting_address>>22),faulting_address&0x3FFFFF);
   //PANIC("Page fault");
   
   
}
