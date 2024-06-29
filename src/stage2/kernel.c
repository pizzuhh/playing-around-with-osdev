#include "include/libc/stdint.h"
#include "include/libc/stdio.h"
#include "include/idt.h"
#include "include/exceptions.h"
#include "include/pic.h"
#include "include/pit.h"

void _kstart() {
    terminal_initialize(vga_entry_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK));
    printf(":3 :3 :3 :3 :3 :3\n");
    init_idt();
    set_idt_descriptor(0, div_by_zero, TRAP_GATE);
    pic_disable();
    remap_PIC();
    set_idt_descriptor(0x20, timer_irq_handler, INTERRUPT_GATE);
    IRQ_clear_mask(0);
    asm volatile ("sti");
    set_pit_mode_frequency(0, MODE2, 1193);
    for(;;);
    halt;
}
