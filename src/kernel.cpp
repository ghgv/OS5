#include <stdio.h>
#include <gdt.h>
#include <common.h>
#include <string.h>
#include <kheap.h>
#include <syscall.h>
#include <drivers/keyboard.h>
#include <../arch/um/tty.h>
#include <multitask.h>
#include <paging.h>


extern uint32_t __end;//current end of data as in the loader
extern uint32_t __code;
registers_t *new_esp;
uint32_t *topend=0;
uint32_t endcode= (uint32_t)&__end;
//extern "C" void jumper();
//extern "C" void screen();
extern "C" void test_isr14();
#define _DEBUG_ 1

void impr(){
 printf("A\n");   
}

 void kernelMain(void* multiboot_structure, unsigned int magicnumber )
{
    
  /*  printf("Booting DaliOS v.0.1 30May20\n");
    printf("https://www.dali.com.co\n");
    printf("    Magic Number: %x\n",magicnumber);
    printf("    Base[0x%x]\n",0x123A4);
    printf("    Keyboard at address [0x%x]=%x\n",PS2,inb(PS2));
    printf("Ticks %X\n",getticks());
    printf("Ticks %X\n",getticks());*/

    //jumper();
    //screen();
    printf("    Code initial Address= 0x%X\n",&__code);
    printf("    Memory End Address of Kernel code= 0x%X\n",&__end); 
    printf("    Task 1 at = 0x%X\n",&impr); 
    init_descriptor_tables();
    initheap((unsigned char *)&__end,(unsigned char *)(0x1000000));
    
    //asm volatile("int $0x1");
    //asm volatile("int $0x4");
    
    
    paging();
    init_keyboard(12);
    //test_isr14();
    
   /* uint32_t *ptr = (uint32_t*)(4096*1024);
    printf("Pointer 0x%X\n",ptr);
    uint32_t do_page_fault = *ptr;*/
    
    //initialise_syscalls();
    
     
    init_multitask();
    Task_create(1,1,(void *)impr);
    
    
    uint32_t *x = (uint32_t *)malloc(0x100);
    printf("After init heap\n"); 
    asm volatile("sti");
    lsh_loop();
    //while(1);
}
