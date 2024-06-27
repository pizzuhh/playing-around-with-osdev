#include "libc/stdint.h"
#include "libc/stdio.h"


void _kstart() {
    write_string("Welcome to the C programming language without standard library");
    for(;;);
}
