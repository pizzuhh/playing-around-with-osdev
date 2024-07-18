#include "libc/stdint.h"
#include "libc/string.h"
#define VGA_MEMORY  0xA0000
#define HEIGHT      200
#define WIDTH       320

extern uint8_t init_color;
void putpixel(uint32_t x, uint32_t y, uint8_t color);
void init_screen(uint8_t color);
void draw(uint8_t (*icon)[8], uint8_t color, uint32_t sx, uint32_t sy);
void finish(void);
