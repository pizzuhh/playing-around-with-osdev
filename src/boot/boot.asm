[BITS 16]
[ORG 0x7c00]


jmp main

; PUTS puts string to teletype output
; si - null terminated string
puts:
    mov bh, 0
    mov bl, 0
    mov ah, 0x0E
    .next_char:
    mov al, [si]
    cmp al, 0
    jz .done
    int 10h
    inc si
    jmp .next_char
    .done:
        ret



main:
     ; set new video mode; clear screen
    mov ah, 0x00
    mov al, 0x13
    int 0x10
    
    ; read the disk
    cld
    mov ah, 0x02
    mov al, 0x01
    mov ch, 0x00
    mov cl, 0x02
    mov dh, 0x00
    xor bx, bx
    mov es, bx
    mov bx, 0x7e00
    int 0x13
    ;jc .disk_error
    
    jmp 0x0000:0x7e00

    hlt
.disk_error:
    mov si, dsk_read_error
    call puts
    cli
    hlt

dsk_read_error:    db  "Disk Read failed :(", 0

times 510-($-$$) db 0
dw 0xAA55
