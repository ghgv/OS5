
[BITS 32]
[GLOBAL jumper]                  ; Make 'mboot' accessible from C.
[GLOBAL screen]

[GLOBAL gdt_flush]    ; Allows the C code to call gdt_flush().

gdt_flush:
    mov eax, [esp+4]  ; Get the pointer to the GDT, passed as a parameter.
    lgdt [eax]        ; Load the new GDT pointer

    mov ax, 0x10      ; 0x10 is the offset in the GDT to our data segment
    mov ds, ax        ; Load all data segment selectors
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    jmp 0x08:.flush   ; 0x08 is the offset to our code segment: Far jump!
.flush:
    ret
    
[GLOBAL idt_flush]    ; Allows the C code to call idt_flush().

idt_flush:
    mov eax, [esp+4]  ; Get the pointer to the IDT, passed as a parameter. 
    lidt [eax]        ; Load the IDT pointer.
    ret
    

;jumper:
;    JMP 0x08:reload_CS
;reload_CS:
;    mov ax,0x10
;    mov DS,AX
;    mov ES,AX
;    mov FS,AX
;    mov GS,AX
;    mov SS,AX
;    ret
;screen:
;    mov ax,0xb800 
;    mov ax, 0x5064
;    mov [0xb8000],ax ;colour + space character(0x20)
;    ret


    



