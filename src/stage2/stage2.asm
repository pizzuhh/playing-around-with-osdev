[ORG 0x7e00]
[BITS 16]
%define ENDL 0x0D, 0x0A

_start:
    jmp main

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
    mov bh, 0x1F
    xor cx, cx
    mov dx, 0x184F
    int 10h
    mov si, msg
    call puts
    mov si, msg2
    call puts
    cli
    hlt

msg         db  "Loading kernel...", ENDL, 0
msg2         db  "What kernel. Idk how to do C yet :(", 0
read_error_msg db "Disk read error", ENDL, 0
