#include <common.h>
#include <stdio.h>
#include <isr.h>

extern registers_t *new_esp;
isr_t interrupt_handlers[256];

void register_interrupt_handler(uint8_t n, isr_t handler)
{
    interrupt_handlers[n] = handler;
}

// This gets called from our ASM interrupt handler stub.
extern "C" void isr_handler(registers_t regs)
{
    if (interrupt_handlers[regs.int_no] != 0)
    {
        isr_t handler = interrupt_handlers[regs.int_no];
        handler(regs);
        printf("handled ISR: %i \n",regs.int_no);
    }
    else
    {
        printf("unhandled ISR: %i\n",regs.int_no);
        while(1);
    }
}

// This gets called from our ASM interrupt handler stub.
extern "C"  int irq_handler(registers_t regs) // 64bytes
{
    // Send an EOI (end of interrupt) signal to the PICs.
    // If this interrupt involved the slave.
    uint32_t esp;
    
    if (regs.int_no >= 40)
    {
        // Send reset signal to slave.
        printf("Interrupt Number:       %i\n",regs.int_no);
        printf("Error code:             %i\n",regs.err_code);
        printf("ESP                     0x%x\n",regs.esp);
        outb(0xA0, 0x20);
        
    }
    //printf("Returned_ESP: 0x%X\n",(registers_t *) new_esp);
    // Send reset signal to master. (As well as slave, if necessary).
    outb(0x20, 0x20);

    if (interrupt_handlers[regs.int_no] != 0)
    {
       /* printf("Interrupt Number:       %i\n",regs.int_no);
        printf("    Error code_:             %i\n",regs.err_code);
        printf("    ESP                     0x%x\n",regs.esp);*/
        isr_t handler = interrupt_handlers[regs.int_no];
        
        //printf("IRQ: %i \n",regs.int_no);
        handler(regs);
       /* printf("Returned ESP: 0x%X\n",new_esp);
        printf("Returned EIP: 0x%X\n",new_esp->eip);
        printf("Returned EBX: 0x%X\n",new_esp->ebx);
        */
        if (regs.int_no ==32)
            return (int)(new_esp)-0x24;
        else {
            return (int )(regs.esp-0x24);
        }
    }

}
