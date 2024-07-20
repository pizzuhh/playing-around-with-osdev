/*
File used to define *helper* functions and to keep kerne.c somewhat clean.
*/

#ifndef KRN_H
#define KRN_H
#define NULL ((void *)0)
#include "drivers/ata.h"
#include "drivers/keyboard.h"
#include "drivers/serial.h"
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
#include "include/memory/pmm.h"

extern uint8_t graphics_mode;


typedef struct {
    uint64_t base_address;
    uint64_t len;
    uint32_t type;
    uint32_t acpi;
} __attribute__((packed)) SMAP_mem;

uint32_t num_smap_entries = 0;
uint32_t total_mem = 0;
SMAP_mem *SMAP_entry;
const uint32_t MEMMAP_AREA = 0x50000;

/*Put this at the beginning of _kstart. Set ups the IDT and other stuff we need. */
#define KINIT\
    pit_freq = 1193;\
    init_idt();\
    /*Exceptions*/\
    set_idt_descriptor(0, div_by_zero, TRAP_GATE);\
    set_idt_descriptor(1, dbg, TRAP_GATE);\
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
    init_serial();\
    num_smap_entries = *(uint32_t*)0x8000;\
    SMAP_entry = (SMAP_mem*)0x8004;\
    SMAP_entry += num_smap_entries - 1;\
    total_mem = SMAP_entry->base_address + SMAP_entry->len - 1;\
    initialize_physical_memory_manager(MEMMAP_AREA, total_mem);\
    SMAP_entry = (SMAP_mem*)0x8004;\
    for (uint32_t i = 0; i < num_smap_entries; i++, SMAP_entry++)\
        if (SMAP_entry->type == 1)\
            initialize_memory_region(SMAP_entry->base_address, SMAP_entry->len);\
     deinitialize_memory_region(0x1000, 0x9000);/* Reserved memory for the kernel*/\
     deinitialize_memory_region(MEMMAP_AREA, max_blocks / BLOCKS_PER_BYTE);\

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
