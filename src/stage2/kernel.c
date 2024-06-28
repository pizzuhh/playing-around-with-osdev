#include "include/libc/stdint.h"
#include "include/libc/stdio.h"
#include "include/idt.h"
#include "include/exceptions.h"

void _kstart() {
    terminal_initialize(vga_entry_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK));
    printf("Terminal initialized!\n");
    init_idt();
    set_idt_descriptor(0, div_by_zero, TRAP_GATE);
    printf("IDT and exceptions handlers!\n");
    printf("Kernel Entry.\n");
    halt;
}
