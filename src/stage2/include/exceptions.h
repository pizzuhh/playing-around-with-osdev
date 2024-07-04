#include "libc/stdio.h"
#include "libc/stdint.h"
#include "idt.h"

__attribute__((interrupt)) void div_by_zero(interrupt_frame *frame) {
    printf("Division by 0!\n");
    frame->eip++; // move to the next instruction. Not very good but...

}

__attribute__((interrupt)) void overflow(interrupt_frame *frame) {
    asm volatile ("mov %eax, 0");
    frame->eip++;
    //halt;
}
