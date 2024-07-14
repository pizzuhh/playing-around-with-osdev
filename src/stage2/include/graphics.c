#include "./graphics.h"

void putpixel(uint32_t x, uint32_t y, uint32_t color) {
    uint8_t *location = (uint8_t*)VGA_MEMORY + 320 * y + x;
    *location = color; 
}

