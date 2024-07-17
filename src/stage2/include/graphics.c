#include "./graphics.h"
uint8_t init_color = 0x00;

void putpixel(uint32_t x, uint32_t y, uint32_t color) {
    uint8_t *location = (uint8_t*)VGA_MEMORY + 320 * y + x;
    *location = color; 
}

void init_screen(uint8_t color) {
    init_color = color;
    for (int y = 0; y <= HEIGHT; ++y) {
        for (int x = 0; x <= WIDTH; ++x) {
            putpixel(x, y, color);
        }
    }
}
void draw(uint8_t (*icon)[8], uint8_t color, uint32_t sx, uint32_t sy) {
    for (int y = 0; y <= 7; ++y) {
        for (int x = 0; x <= 7; ++x) {
            uint8_t *location = (uint8_t*)VGA_MEMORY + 320 * (sy + y) + (sx + x);
            *location = (icon[y][x] == 1)  ? color : 0x00;
        }
    }
}
