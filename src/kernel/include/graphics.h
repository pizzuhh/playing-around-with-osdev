#ifndef GRAPHICS_H
#define GRAPHICS_H
#include "libc/stdint.h"
#include "libc/string.h"
#define VGA_MEMORY  0xA0000
#define HEIGHT      200
#define WIDTH       320

extern unsigned char g_8x8_font[2048];
extern uint8_t init_color;
extern uint8_t default_color[2];
extern uint32_t curx, cury;
void init_screen(uint8_t bg_color, uint8_t fg_color);
void clear_screen(void);
void putpixel(uint32_t x, uint32_t y, uint8_t color);
void draw(uint8_t (*icon)[8], uint8_t color, uint32_t sx, uint32_t sy);
void fill_rect(uint32_t sx, uint32_t sy, uint32_t ex, uint32_t ey, uint8_t color);
void v_putchar(char c);
void write_char(uint8_t chr, uint8_t color, uint32_t x, uint32_t y);
void write_string(char *str, uint8_t color);
void scroll(uint8_t direction);
void finish(void);
#endif