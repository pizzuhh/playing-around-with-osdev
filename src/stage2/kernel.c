#include "libc/stdint.h"
#include "libc/stdio.h"
#include "libc/test.h"

void _kstart() {
    terminal_initialize();
    terminal_writestring("Hello world\n");
    terminal_writestring("cat\n");
    terminal_writestring("dog\n");
    for(;;);
}
