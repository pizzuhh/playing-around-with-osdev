/*
File used to define *helper* functions and to keep kerne.c somewhat clean.
*/

#ifndef KRN_H
#define KRN_H
#define NULL ((void *)0)
#include "drivers/ata.h"
#include "drivers/keyboard.h"
#include "include/io.h"
#include "include/libc/stdint.h"
#include "include/libc/stdio.h"
#include "include/libc/string.h"
#include "include/idt.h"
#include "include/exceptions.h"
#include "include/pic.h"
#include "include/pit.h"
#include "include/rtc.h"
#include "include/graphics.h"

/*Put this at the beginning of _kstart. Set ups the IDT and other stuff we need. */
#define KINIT\
    pit_freq = 1193;\
    init_idt();\
    /*Exceptions*/\
    set_idt_descriptor(0, div_by_zero, TRAP_GATE);\
    set_idt_descriptor(4, overflow, TRAP_GATE);\
    set_idt_descriptor(6, invalid_opcode, TRAP_GATE);\
    \
    pic_disable();\
    remap_PIC();\
    IRQ_clear_mask(2); /* PIC2 */\
    /* PIT */\
    set_idt_descriptor(0x20, timer_irq_handler, INTERRUPT_GATE); \
    IRQ_clear_mask(0);\
    /* ps2 keyboard driver*/\
    set_idt_descriptor(0x21, kbd_handler, INTERRUPT_GATE);\
    IRQ_clear_mask(1);\
    /*Real Time Clock*/\
    set_idt_descriptor(0x28, rtc_handler, INTERRUPT_GATE);\
    IRQ_clear_mask(8);\
    asm volatile ("sti");\
    set_pit_mode_frequency(0, MODE2, pit_freq);\
    enable_rtc();\

// https://wiki.osdev.org/Reboot#Short_code_to_do_a_8042_reset
void reboot(void)
{
    uint8_t good = 0x02;
    while (good & 0x02)
        good = inb(0x64);
    outb(0x64, 0xFE);
    halt;
}

#endif
