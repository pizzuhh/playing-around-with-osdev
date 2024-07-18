#include "./graphics.h"
uint8_t init_color = 0x00;

#define COLOR_DEPTH 1


uint8_t backbuff[WIDTH * HEIGHT * COLOR_DEPTH];

void putpixel(uint32_t x, uint32_t y, uint8_t color) {
    uint32_t offset = y * WIDTH + x;
    backbuff[offset] = color; 
}

void init_screen(uint8_t color) {
    memset(backbuff, color, WIDTH * HEIGHT * COLOR_DEPTH);
}

void draw(uint8_t icon[8][8], uint8_t color, uint32_t sx, uint32_t sy) {
    for (int y = 0; y < 8; ++y) {
        for (int x = 0; x < 8; ++x) {
            if (icon[y][x] == 1) {
                putpixel(sx + x, sy + y, color);
            }
        }
    }
}

void finish(void) {
    memcpy((uint8_t*)VGA_MEMORY, backbuff, WIDTH * HEIGHT * COLOR_DEPTH);
}
