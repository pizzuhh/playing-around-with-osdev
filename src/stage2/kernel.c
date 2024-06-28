#include "include/libc/stdint.h"
#include "include/libc/stdio.h"
#include "include/idt.h"

void _kstart() {
    
    terminal_initialize(vga_entry_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK));
    init_idt();
    printf("Kernel\n");
    halt;
}
