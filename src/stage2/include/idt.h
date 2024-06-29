#pragma once
#include "libc/stdint.h"
#include "libc/stdio.h"
#include "./io.h"

typedef struct {
    uint16_t    offset_low;
    uint16_t    segment_selector;
    uint8_t     reserved; // must be 0
    uint8_t     flags;
    uint16_t    offset_high;
} __attribute__((packed)) idt_entry;

typedef struct {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed)) idtr_t;

idt_entry idt[256];
idtr_t idtr;

typedef struct {
    uint32_t eip;
    uint32_t cs;
    uint32_t eflags;
    uint32_t sp;
    uint32_t ss;
} __attribute__((packed)) interrupt_frame;


enum {
    TRAP_GATE = 0x8F,
    INTERRUPT_GATE = 0x8E
} IDT_GATE_TYPE;

__attribute__((interrupt)) void default_exception_handler(interrupt_frame *frame) {
    printe("Unhandled exception has been encountered! Halting process!");
    halt;
}

__attribute__((interrupt)) void default_exception_handler_err_code(interrupt_frame *frame, uint32_t err_code) {
    halt;
}

__attribute__((interrupt)) void default_interrupt_handler(interrupt_frame *frame) {
    printe("default_interrupt_handler() called");
    halt;
}
void set_idt_descriptor(uint8_t entry, void *isr, uint8_t flags) {
    idt_entry *idt_ent = &idt[entry];
    idt_ent->offset_low = (uint32_t)isr & 0xFFFF;
    idt_ent->segment_selector = 0x08;
    idt_ent->reserved = 0;
    idt_ent->flags = flags;
    idt_ent->offset_high = ((uint32_t)isr >> 16) & 0xFFFF;
}

void init_idt(void) {
    idtr.limit = (uint16_t)(sizeof(idt_entry)*256)-1;
    idtr.base = (uint32_t)&idt[0];
    for (uint8_t entry = 0; entry < 32; ++entry) {
        if (entry == 8 || entry == 10 || entry == 11 || entry == 12 
        || entry == 13 || entry == 14 || entry == 17 || entry == 21) { // NOTE: The code compiles without casting but tell clangd to shut up because it generates error and that annoys me :(
            set_idt_descriptor(entry, (void*)default_exception_handler_err_code, TRAP_GATE);
        } else {
            set_idt_descriptor(entry, (void*)default_exception_handler, TRAP_GATE);
        }
    }

    for (uint8_t entry = 32; entry < 255; entry++) {
        set_idt_descriptor(entry, (void*)default_interrupt_handler, INTERRUPT_GATE);
        
    }
    asm volatile ("lidt %0" : : "memory"(idtr));
    asm volatile ("sti");
}
