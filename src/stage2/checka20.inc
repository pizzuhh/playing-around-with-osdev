[BITS 16]
; ax = 0 - DISABLED
; ax = 1 - ENABLED



check_a20:
    pushf
    push si
    push di
    push ds 
    push es
    cli
    xor ax, ax
    mov ds, ax
    mov si, 0x0500
    not ax
    mov es, ax
    mov di, 0x0510
    mov al, [ds:si]
    mov byte [.BUFFBMB], al
    mov al, [es:di]
    mov byte [.BUFFOMB], al
    mov ah, 0x01
    mov byte [ds:si], 0
    mov byte [es:di], 1
    mov al, [ds:si]
    cmp al, [es:di]
    jne .exit
    dec ah
.exit:
    mov al, [.BUFFBMB]
    mov [ds:si], al
    mov al, [.BUFFOMB]
    mov [es:di], al
    shr ax, 8
    pop es
    pop ds
    pop di 
    pop si
    popf
    ret

.BUFFBMB db 0
.BUFFOMB db 0




