[BITS 16]
[ORG 0x7c00]

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
     ; set new video mode; clear screen
    mov ah, 0x00
    mov al, 0x13
    int 0x10
    
    ; read the disk
    cld
    mov ah, 0x02
    mov al, 0x30
    mov ch, 0x00
    mov cl, 0x02
    mov dh, 0x00
    mov bx, 0x2000
    mov es, bx
    xor bx, bx
    int 0x13
    ;jc .disk_error
    
    ; detect memory
    xor ax, ax
    mov es, ax
    memmap_: equ 0x8000
    mov di, 0x8004
    xor ebx, ebx
    xor bp, bp
    mov eax, 0xE820
    mov edx, "PAMS"
    mov [es:di + 20], dword 1
    mov ecx, 24
    int 0x15
    jc .error
    
    mov edx, "PAMS"
    cmp eax, edx
    jne .error
    test ebx, ebx
    je .error
    jmp .start
    
    .next:
        mov edx, "PAMS"
        mov ecx, 24
        mov eax, 0xE820
        int 0x15
    
    .start:
        jcxz .skip_ent
        mov ecx, [es:di + 8]
        or ecx, [es:di + 12]
        jz .skip_ent
    
    .good:
        inc bp
        add di, 24
    
    .skip_ent:
        test ebx, ebx
        jz .done
        jmp .next
    
        
    .error:
        stc
    .done:
        mov [memmap_], bp
        clc
    cli
    lgdt [_GDT]
    mov eax, cr0
    or eax, 1
    mov cr0, eax
    jmp 08h:prot_start

    hlt
.disk_error:
    mov si, dsk_read_error
    call puts
    cli
    hlt

[BITS 32]
prot_start:
    ; set up some registrers copied from old commit
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    mov esp, 0x20000
    jmp pstart

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


pstart:
    jmp 0x20000
    hlt

dsk_read_error:    db  "Disk Read failed :(", 0

times 510-($-$$) db 0
dw 0xAA55
