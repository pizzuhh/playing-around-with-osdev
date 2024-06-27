#include "libc/stdint.h"
#include "libc/stdio.h"


void _kstart() {
    // works
    write_char('C', 0);
    write_char('a', 2);
    write_char('t', 4);
    // doesn't work
    write_string("cAT");
    for(;;);
}
