[BITS 16]
[ORG 0x7c00]

%define ENDL 0x0D, 0x0A

_start:
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

; idk if it works
; ax - lba
lba_to_chs:
    push ax
    push dx
    mov dl, 0x80
    _v_number_of_heads      dw 0x0000
    _v_number_of_cylinders  dw 0x0000
    _v_sectors_per_track    dw 0x0000
    ; get some parameters for the disk first
    mov ah, 0x08
    int 0x13
    mov [_v_number_of_heads], dh
    mov [_v_sectors_per_track], cl
    mov [_v_number_of_cylinders], ch
    xor dx, dx
    div word [_v_sectors_per_track] ; ax = LBA / sectors per track = temp
                                  ; dx = LBA % sectors per track = sector
    inc dx ; Sector
    mov cx, dx
    
    xor dx, dx
    div word [_v_number_of_heads]  ; ax = LBA / number of heads = cylinder
                                 ; dx = LBA % number of heads = heads
    mov dh, dl
    mov ch, al
    shl ah, 6
    or cl, ah
    pop ax
    mov al, al
    pop dx
    ret


main:
    mov si, msg
    call puts
    mov ah, 0x02 ; read disk
    mov al, 0x01 ; sectors to read
    mov ch, 0x00 ; cilynder 
    mov cl, 0x02 ; sector
    mov dh, 0x00 ; head
    mov dl, 0x80 ; drive  
    xor bx, bx
    mov es, bx
    mov bx, 0x7e00 ; put the contnets to 0x7e00
    int 0x13
    jc .disk_read_err
    jmp 0x0000:0x7e00
    
    .disk_read_err:
        mov si, disk_error
        call puts
        mov ah, 0
        int 0x16
        int 0x19
        cli
        hlt
    mov si, stage2_fail
    call puts
    cli
    hlt

; vars
msg     db "Loading stage 2...", 0
disk_error db ENDL, "Disk Read Error! Press any key to reboot...", 0
stage2_fail db "Cloud not load stage2!", ENDL, 0
times 510-($-$$) db 0
dw 0xaa55
