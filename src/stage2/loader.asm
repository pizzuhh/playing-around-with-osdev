[BITS 32]
extern _kstart
call _kstart
; jmp $
int 0x19 ; reboot if we ever return
