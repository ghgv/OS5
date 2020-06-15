[GLOBAL test_isr14]    ; Allows the C code to call test_is14().

test_isr14:
    mov ax, 0x10      ; 0x10 is the offset in the GDT to our data segment
    mov ds, ax        ; Load all data segment selectors
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
ret
