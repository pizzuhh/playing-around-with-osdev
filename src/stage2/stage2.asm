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
    mov ah, 0x00
    mov al, 0x03
    int 0x10
    ; mov si, msg_1
    ; call puts
    cli
    lgdt [_GDT]
    mov eax, cr0
    or eax, 1
    mov cr0, eax
    jmp 08h:prot_start


; msg_1:  db "Stage 2 loaded...", ENDL, "Last interupt before protected mode :D", ENDL, 0


[BITS 32]
prot_start:
    ; setup the stack and stuff
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    mov esp, 0x90000
    jmp pmain

; esi - string
; ah  - color
puts_pm:
    push ebp
    push ebx
    mov ebp, esp
    mov ebx, 0xB8000
.print_next_char:
    mov al, [esi]
    cmp al, 0
    jz .end_puts
    mov [ebx], eax
    add ebx, 2
    inc esi
    jmp .print_next_char
.end_puts:
    pop ebx
    pop ebp
    ret


pmain:
    mov ah, 0x0C
    mov esi, msg_cat
    call puts_pm
    jmp halt

msg_cat: db "Protected mode discovered! No C yet >=<", 0

times 510-($-$$) db 0xFF
dw 0xAAFF ; not necesery but mark the end of this sector
