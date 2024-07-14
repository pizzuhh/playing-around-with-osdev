#include "libc/stdint.h"
#define VGA_MEMORY 0xA0000

void putpixel(uint32_t x, uint32_t y, uint32_t color);
