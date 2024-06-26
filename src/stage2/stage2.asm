[ORG 0x7e00]
[BITS 16]
%define ENDL 0x0D, 0x0A


_start:
    jmp main
halt:
    hlt
    jmp halt
; %include "./src/stage2/checka20.inc"
puts:
    mov bh, 0
    mov ah, 0x0E
.next_char:
    mov al, [si]
    cmp al, 0
    jz .done
    int 0x10
    inc si
    jmp .next_char
.done:
    ret


GDT_start:
    null_segment: 
        dd 0
        dd 0
    code_segment:
        dw 0xFFFF
        dw 0x0
        db 0x0
        db 10011010b
        db 11001111b
        db 0
    data_segment:
        dw 0xFFFF
        dw 0x0
        db 0x0
        db 10010010b
        db 11001111b
        db 0
GDT_end:

_GDT:
    dw GDT_end - GDT_start - 1
    dd GDT_start

main:
    cli
    lgdt [_GDT]
    mov eax, cr0
    or eax, 1
    mov cr0, eax
    jmp 08h:prot_start

[BITS 32]
prot_start:
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    mov esp, 0x90000
    mov al, 'X'
    mov ah, 0x0F
    mov [0xB8000], ax
    jmp halt

