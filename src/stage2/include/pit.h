#pragma once
#include "./pic.h"
#include "./io.h"
#include "./idt.h"
#include "libc/stdint.h"
#include "libc/stdio.h"

int tick = 0;
__attribute__((interrupt)) void timer_irq_handler(interrupt_frame *frame) {
    // TODO: Implement sleep later
    PIC_sendEOI(0);
}
