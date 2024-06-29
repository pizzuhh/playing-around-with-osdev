#ifndef IDT_H
#define IDT_H
#include "libc/stdint.h"
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

extern idt_entry idt[256];
extern idtr_t idtr;

extern enum {
    TRAP_GATE = 0x8F,
    INTERRUPT_GATE = 0x8E
} IDT_GATE_TYPE;

typedef struct {
    uint32_t eip;
    uint32_t cs;
    uint32_t eflags;
    uint32_t sp;
    uint32_t ss;
} __attribute__((packed)) interrupt_frame;




__attribute__((interrupt)) void default_exception_handler(interrupt_frame *frame);
__attribute__((interrupt)) void default_exception_handler_err_code(interrupt_frame *frame, uint32_t err_code);
__attribute__((interrupt)) void default_interrupt_handler(interrupt_frame *frame);

void set_idt_descriptor(uint8_t entry, void *isr, uint8_t flags);
void init_idt(void);
#endif
