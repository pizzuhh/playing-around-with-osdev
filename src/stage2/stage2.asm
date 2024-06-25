[ORG 0x7e00]
[BITS 16]
%define ENDL 0x0D, 0x0A


_start:
    jmp main
%include "./src/stage2/checka20.inc"
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

main:
    mov ah, 0x00
    mov al, 0x03
    int 0x10

    xor al, al
    mov ah, 0x06
    mov bh, 0x0F
    xor cx, cx
    mov dx, 0x184F
    int 10h
    
    call check_a20
    cmp al, 0
    jz .no_a20
    mov si, a_20_on
    call puts
    mov si, msg
    call puts
    cli
    hlt
.no_a20:
    mov si, a_20_off
    call puts


msg         db  "Loading kernel...", ENDL, 0
a_20_on     db  "A20 is ON", ENDL, 0
a_20_off     db  "A20 is OFF", ENDL, 0
read_error_msg db "Disk read error", ENDL, 0
