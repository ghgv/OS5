[GLOBAL multitask_switch]    ; Allows the C code to call idt_flush().

extern MULTITASKING_SCHEDULER

multitask_switch:
    
    
   ; call MULTITASKING_SCHEDULER
     
    ret
