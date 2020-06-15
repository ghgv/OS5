.set MAGIC, 0x1badb002
.set FLAGS, (1<<0 | 1<<1)
.set CHECKSUM, -(MAGIC + FLAGS)

.section .multiboot
    .long MAGIC
    .long FLAGS
    .long CHECKSUM


.section .text
.extern _Z10kernelMainPvj
.extern callConstructors
.global loader


loader:
    ;mov $kernel_stack, %esp
    ;push %eax
    push %ebx
    cli
    call _Z10kernelMainPvj
    ;mov %eax, 0xb8000
    ;mov %es,%ax
    ;mov %ax,0x4061


_stop:
    cli
    hlt
    jmp _stop


.section .bss
.space 2*1024*1024; # 2 MiB
kernel_stack:
