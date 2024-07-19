#ifndef STDIO_H
#define STDIO_H
#include "stdint.h"
#include "../pit.h"
#include "../../drivers/serial.h"
#include "../../include/graphics.h"
#include "string.h"
#include <stdarg.h>

// https://wiki.osdev.org/Bare_Bones#Writing_a_kernel_in_C
enum vga_color {
	VGA_COLOR_BLACK = 0,
	VGA_COLOR_BLUE = 1,
	VGA_COLOR_GREEN = 2,
	VGA_COLOR_CYAN = 3,
	VGA_COLOR_RED = 4,
	VGA_COLOR_MAGENTA = 5,
	VGA_COLOR_BROWN = 6,
	VGA_COLOR_LIGHT_GREY = 7,
	VGA_COLOR_DARK_GREY = 8,
	VGA_COLOR_LIGHT_BLUE = 9,
	VGA_COLOR_LIGHT_GREEN = 10,
	VGA_COLOR_LIGHT_CYAN = 11,
	VGA_COLOR_LIGHT_RED = 12,
	VGA_COLOR_LIGHT_MAGENTA = 13,
	VGA_COLOR_LIGHT_BROWN = 14,
	VGA_COLOR_WHITE = 15,
};

#define vga_entry_color(fg, bg) \
	 (uint8_t)(fg | bg << 4)

#define vga_entry(uc, color) \
	(uint16_t) uc | (uint16_t) color << 8


size_t strlen(const char* str);

static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;


extern size_t terminal_row;
extern size_t terminal_column;
extern uint8_t terminal_color;
extern uint16_t* terminal_buffer;
extern uint8_t* frame_buffer;

void terminal_initialize(uint8_t color);
void terminal_clear(uint8_t color);
void terminal_setcolor(uint8_t color);
void terminal_putentryat(char c, uint8_t color, size_t col, size_t row);
void terminal_putchar(char c, uint8_t color);
void terminal_write(const char* data, size_t size, uint8_t color);
void terminal_writestring_color(const char* data, uint8_t color);
void terminal_writestring(const char* data);


// https://stackoverflow.com/questions/1735236/how-to-write-my-own-printf-in-c
void printk(const char *fmt, ...);
void prints(const char *fmt, ...);
void putchar(const char c);
void disable_cursor();
void enable_cursor(uint8_t cursor_start, uint8_t cursor_end);
void update_cursor(int x, int y);
#endif
