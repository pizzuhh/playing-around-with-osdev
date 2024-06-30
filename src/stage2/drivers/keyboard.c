#include "keyboard.h"


__attribute__((interrupt)) void kbd_handler(interrupt_frame *frame) {
    uint8_t scan_code = inb(0x60);
    PIC_sendEOI(0);
}


