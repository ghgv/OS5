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

#define  PCI_ADDR     0x0CF8
#define  PCI_DATA     0x0CFC
extern uint32_t __end;//current end of data as in the loader
extern uint32_t __code;
registers_t *new_esp;
uint32_t *topend=0;
uint32_t endcode= (uint32_t)&__end;
//extern "C" void jumper();
//extern "C" void screen();
extern "C" void test_isr14();
#define _DEBUG_ 1

uint32_t read_pci(const uint8_t bus, const uint8_t dev, const uint8_t func,uint8_t port, uint8_t len){
uint32_t ret;
uint32_t val =0x80000000 | (bus<<16)|(dev<<11)| (func<<8)| (port &0xFC);
outw(0x0CF8,val);
ret=inw(0x0CFC+(port&0x3));
ret &=(0xFFFFFFFF>>((4-len)*8));
return ret;
    
    
}

uint8_t pci_read_byte(const int bus, const int dev, const int func, const int port) {
  const uint32_t val = 0x80000000 |
    (bus << 16) |
    (dev << 11) |
    (func << 8) |
    (port & 0xFC);
  outw(PCI_ADDR, val);
  return inb(PCI_DATA + (port & 3));
}


uint32_t pci_read_word(const int bus, const int dev, const int func, const int port) {
  if ((port & 3) < 3) {
    const uint32_t val = 0x80000000 |
      (bus << 16) |
      (dev << 11) |
      (func << 8) |
      (port & 0xFC);
    outw(PCI_ADDR, val);
    return inw(PCI_DATA + (port & 0x3));
  } else
    return (pci_read_byte(bus, dev, func, port + 1) << 8) | pci_read_byte(bus, dev, func, port);
}


uint32_t pci_read_dword(const int bus, const int dev, const int func, const int port) {
  if ((port & 3) == 0) {
    const uint32_t val = 0x80000000 |
      (bus << 16) |
      (dev << 11) |
      (func << 8) |
      (port & 0xFC);
    outw(PCI_ADDR, val);
    return inw(PCI_DATA);
  } else
    return (pci_read_word(bus, dev, func, port + 2) << 16) | pci_read_word(bus, dev, func, port);
}

void impr(){
 printf("A\n");   
}

 void kernelMain(void* multiboot_structure, unsigned int magicnumber )
{
    
    printf("Booting DaliOS v.0.1 30May20\n");
    printf("https://www.dali.com.co\n");
    printf("    Magic Number: %x\n",magicnumber);
    printf("    Base[0x%x]\n",0x123A4);
    printf("    Keyboard at address [0x%x]=%x\n",PS2,inb(PS2));
    printf("Ticks %X\n",getticks());
    printf("Ticks %X\n",getticks());

    //jumper();
    //screen();
    printf("    Code initial Address= 0x%X\n",&__code);
    printf("    Memory End Address of Kernel code= 0x%X\n",&__end); 
    printf("    Task 1 at = 0x%X\n",&impr); 
    init_descriptor_tables();
    initheap((unsigned char *)&__end,(unsigned char *)(0x1000000));
    
    //asm volatile("int $0x1");
    //asm volatile("int $0x4");
    
    
    //paging();
    init_keyboard(12);
    //test_isr14();
    
   /* uint32_t *ptr = (uint32_t*)(4096*1024);
    printf("Pointer 0x%X\n",ptr);
    uint32_t do_page_fault = *ptr;*/
    
    //initialise_syscalls();
    
     
    init_multitask();
    //Task_create(1,1,(void *)impr);
    
    
    //uint32_t *x = (uint32_t *)malloc(0x100);
    printf("After init heap\n"); 
   /* printf("Vendor ID: 0x%X\n",read_pci(0,0x1A,0,0,sizeof(uint16_t)));
    for (int i=0; i<6; i++)
            printf("0x%X\n",pci_read_dword(0, 0x1A, 0,(i<<2)));*/
    asm volatile("sti");
    
    lsh_loop();
    //while(1);
}
