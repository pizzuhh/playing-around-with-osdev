#pragma once
#include "./pic.h"
#include "./io.h"
#include "./idt.h"
#include "libc/stdint.h"
#include "libc/stdio.h"

__attribute__((interrupt)) void timer_irq_handler(interrupt_frame *frame) {
    uint32_t t = 0;
    printf("Ticks: %d\n", t);
    t++;
    PIC_sendEOI(0);
}
