#include <multitask.h>


TaskType_t Tasks[100];
unsigned short AllTasks=0;
unsigned short currentPID;
unsigned short numTasks=0;

char TASK_EMPTY = 0;
char TASK_READY  = 1;

void init_multitask(){
    init_timer(1);    
    
}


int scheduler(registers_t regs ){
  /*  
    printf(" Reg DS:        0x%X\n",regs.ds);
    printf(" Reg EDI:       0x%X\n",regs.edi);
    printf(" Reg ESI:       0x%X\n",regs.esi);
    printf(" Reg EBP:       0x%X\n",regs.ebp);
    printf(" Reg EBX:       0x%X\n",regs.ebx);
    printf(" Reg EDX:       0x%X\n",regs.edx);
    printf(" Reg ECX:       0x%X\n",regs.ecx);
    printf(" Reg EAX:       0x%X\n",regs.eax);
    printf(" Reg no:        0x%X\n",regs.int_no);
    printf(" Reg err_code:  0x%X\n",regs.err_code);
    printf(" Reg EIP:       0x%X\n",regs.eip);
    printf(" Reg CS:        0x%X\n",regs.cs);
    printf(" Reg EFGS:      0x%X\n",regs.eflags);
    printf(" Reg useresp:   0x%X\n",regs.useresp);
    printf(" Reg SS:        0x%X\n",regs.ss);
    printf(" Sche. PID %i\n",currentPID);
 */
    if(AllTasks <= 0)
    {
       // printf("All Task <=0 %X %X\n",regs.esp, regs.eip);
        return regs.esp;
    }
    Tasks[currentPID].stack=(CPUState_t *)regs.esp;
    
  //  printf(" SW %i\n",currentPID);
    do{
        currentPID+=1;
        if (currentPID>AllTasks)
            currentPID=1;
        
    }while (Tasks[currentPID].status!=TASK_READY);
    printf("Switching to 0x%X\n",(int)Tasks[currentPID].stack);
    
    return (int)Tasks[currentPID].stack;
    
    
}

void Task_create(unsigned short ID, unsigned char prio, void * ptr){
    if (Tasks[ID].status != TASK_EMPTY)
        return ;
    CPUState_t *cpustate;
    
    asm volatile("cli");
    
    
    cpustate = (CPUState_t *)(malloc(sizeof(CPUState_t)));
    
    cpustate -> eax = 0;
    cpustate -> ebx = 0x0;
    cpustate -> ecx = 0;
    cpustate -> edx = 0;

    cpustate -> esi = 0;
    cpustate -> edi = 0;
    cpustate -> ebp = 0;
    
    /*
    cpustate -> gs = 0;
    cpustate -> fs = 0;
    cpustate -> es = 0;
    cpustate -> ds = 0;
    */
    
    // cpustate -> error = 0;    
   
    // cpustate -> esp = ;
    cpustate -> eip = (uint32_t)ptr;
    cpustate -> cs = 0x08;
    // cpustate -> ss = ;
    cpustate -> eflags = 0x202;
    
    Tasks[ID].status = TASK_READY;
    Tasks[ID].stack =  (CPUState_t*) cpustate;
    Tasks[ID].PID =ID;
   
    AllTasks +=1;
    printf("Task created[%i] at 0x%X\n",AllTasks,cpustate);
    asm volatile("sti");
}
