#include "libc/stdio.h"
#include "libc/stdint.h"
#include "idt.h"

__attribute__((interrupt)) void div_by_zero(interrupt_frame *frame) {
    printf("Division by 0!\n");
    frame->eip+=2;

}

__attribute__((interrupt)) void overflow(interrupt_frame *frame) {
    asm volatile ("mov %eax, 0");
    frame->eip++;
    //halt;
}

__attribute__((interrupt)) void ivalid_opcode(interrupt_frame *frame) {
    printf("Invalid opcode\n");
    frame->eip+=2;
}
