#include "libc/stdint.h"
#include "libc/stdio.h"


void _kstart() {
    terminal_initialize(vga_entry_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK));
    printf("cat %c", 'C');
    for(;;);
}
