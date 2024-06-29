#include "include/libc/stdint.h"
#include "include/libc/stdio.h"
#include "include/idt.h"
#include "include/exceptions.h"
#include "include/pic.h"
#include "include/pit.h"

uint16_t pit_freq = 0;

void _kstart() {
    pit_freq = 1193;
    terminal_initialize(vga_entry_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK));
    init_idt();
    set_idt_descriptor(0, div_by_zero, TRAP_GATE);
    pic_disable();
    remap_PIC();
    set_idt_descriptor(0x20, timer_irq_handler, INTERRUPT_GATE);
    IRQ_clear_mask(0);
    asm volatile ("sti");
    set_pit_mode_frequency(0, MODE2, pit_freq);
    printf("Kernel");
    for(;;);
    halt;
}
